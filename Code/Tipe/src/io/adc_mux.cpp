#include "adc_mux.hpp"

int16_t AdcMuxReading::getAdcValueByAddr(AdcAddr addr)
{
    return values[addr - ADC1];
}

int16_t AdcMuxReading::getAdcValueByIndex(size_t index)
{
    return values[index];
}

float AdcMuxReading::getAdcValueByAddrInVolts(AdcAddr addr)
{
    float fsRange;
    switch (gain)
    {
    case GAIN_TWOTHIRDS:
        fsRange = 6.144f;
        break;
    case GAIN_ONE:
        fsRange = 4.096f;
        break;
    case GAIN_TWO:
        fsRange = 2.048f;
        break;
    case GAIN_FOUR:
        fsRange = 1.024f;
        break;
    case GAIN_EIGHT:
        fsRange = 0.512f;
        break;
    case GAIN_SIXTEEN:
        fsRange = 0.256f;
        break;
    default:
        fsRange = 0.0f;
    }
    return getAdcValueByAddr(addr) * (fsRange / 32768);
}

float AdcMuxReading::getAdcValueByIndexInVolts(size_t index)
{
    return getAdcValueByAddrInVolts(static_cast<AdcAddr>((int)ADC1 + index));
}

AdcMuxReading::AdcMuxReading(int16_t adc1, int16_t adc2, int16_t adc3, int16_t adc4, adsGain_t new_gain)
{
    values[0] = adc1;
    values[1] = adc2;
    values[2] = adc3;
    values[3] = adc4;
    gain = new_gain;
}

void AdcMuxReading::print()
{
    for (int i = 0; i < 4; i++)
    {
        Serial.print("ADC" + String(i + 1) + ": " + String(getAdcValueByIndexInVolts(i)) + "V (" + String(getAdcValueByIndex(i)) + ") ");
    }
    Serial.println();
}

AdcMux::AdcMux()
{
    adc_count = 4;
    adc = (Adafruit_ADS1115 *)malloc(sizeof(Adafruit_ADS1115) * adc_count);
    for (int i = 0; i < adc_count; i++)
    {
        adc[i] = Adafruit_ADS1115();
    }
};

void AdcMux::set_gain(adsGain_t new_gain)
{
    for (int i = 0; i < adc_count; i++)
    {
        adc[i].setGain(new_gain);
    }
    gain = new_gain;
}

void AdcMux::set_rate(uint16_t rate)
{
    for (int i = 0; i < adc_count; i++)
    {
        adc[i].setDataRate(rate);
    }
}

void AdcMux::begin()
{
    adc[0].begin(AdcAddr::ADC1);
    adc[1].begin(AdcAddr::ADC2);
    adc[2].begin(AdcAddr::ADC3);
    adc[3].begin(AdcAddr::ADC4);
    set_gain(GAIN_TWOTHIRDS);
}

void AdcMux::start_adc_reading(uint8_t mux, bool continuous)
{
    for (int i = 0; i < adc_count; i++)
    {
        adc[i].startADCReading(mux, continuous);
    }
}

AdcMuxReading AdcMux::one_shot_reading(uint8_t channel)
{
    int16_t adc1 = adc[0].readADC_SingleEnded(channel);
    int16_t adc2 = adc[1].readADC_SingleEnded(channel);
    int16_t adc3 = adc[2].readADC_SingleEnded(channel);
    int16_t adc4 = adc[3].readADC_SingleEnded(channel);
    return AdcMuxReading(adc1, adc2, adc3, adc4, gain);
}

AdcMuxReading AdcMux::read()
{
    int16_t adc1 = adc[0].getLastConversionResults();
    int16_t adc2 = adc[1].getLastConversionResults();
    int16_t adc3 = adc[2].getLastConversionResults();
    int16_t adc4 = adc[3].getLastConversionResults();
    return AdcMuxReading(adc1, adc2, adc3, adc4, gain);
}

void AdcMux::continuous_reading(uint8_t mux, std::function<void(AdcMuxReading)> callback)
{
    for (int i = 0; i < adc_count; i++)
    {
        adc[i].startADCReading(mux, true);
    }
    while (true)
    {
        callback(read());
        delayMicroseconds(10);
    }
}

void AdcMuxReading::convert_to_weight(Config *config, float *weight)
{
    for (int i = 0; i < ADC_MAX_COUNT; i++)
    {
        auto linearRegressionResult = config->getLinearRegressionResult(i);
        weight[i] =
            (1 / getAdcValueByIndexInVolts(i)) * linearRegressionResult.slope + linearRegressionResult.intercept;
    }
}