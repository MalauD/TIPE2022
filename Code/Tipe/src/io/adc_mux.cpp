#include "adc_mux.hpp"

int16_t AdcMuxReading::getAdcValueByAddr(AdcAddr addr)
{
    switch (addr)
    {
    case ADC1:
        return adc >> 0;
    case ADC2:
        return adc >> 16;
    case ADC3:
        return adc >> 32;
    case ADC4:
        return adc >> 48;
    }
    return -1;
}

float AdcMuxReading::getAdcValueByAddrInVolts(AdcAddr addr)
{
    float fsRange;
    switch (this->gain)
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
    return this->getAdcValueByAddr(addr) * (fsRange / 32768);
}

AdcMuxReading::AdcMuxReading(int16_t adc1, int16_t adc2, int16_t adc3, int16_t adc4, adsGain_t gain)
{
    this->adc = adc1;
    this->adc |= (int64_t)adc2 << 16;
    this->adc |= (int64_t)adc3 << 32;
    this->adc |= (int64_t)adc4 << 48;
    this->gain = gain;
}

AdcMux::AdcMux()
{
    this->adc_count = 4;
    this->adc = (Adafruit_ADS1115 *)malloc(sizeof(Adafruit_ADS1115) * this->adc_count);
    for (int i = 0; i < adc_count; i++)
    {
        this->adc[i] = Adafruit_ADS1115();
    }
};

void AdcMux::set_gain(adsGain_t gain)
{
    for (int i = 0; i < adc_count; i++)
    {
        adc[i].setGain(gain);
    }
    this->gain = gain;
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
    this->set_gain(GAIN_TWOTHIRDS);
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
    return AdcMuxReading(adc1, adc2, adc3, adc4, this->gain);
}

AdcMuxReading AdcMux::read()
{
    int16_t adc1 = adc[0].getLastConversionResults();
    int16_t adc2 = adc[1].getLastConversionResults();
    int16_t adc3 = adc[2].getLastConversionResults();
    int16_t adc4 = adc[3].getLastConversionResults();
    return AdcMuxReading(adc1, adc2, adc3, adc4, this->gain);
}