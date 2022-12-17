#include "adc_mux.hpp"

template <std::size_t size>
int16_t AdcMuxReading<size>::getAdcValueByAddr(AdcAddr addr)
{
    return values[addr - ADC1];
}

template <std::size_t size>
int16_t AdcMuxReading<size>::getAdcValueByIndex(size_t index)
{
    return values[index];
}

template <std::size_t size>
float AdcMuxReading<size>::getAdcValueByAddrInVolts(AdcAddr addr)
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

template <std::size_t size>
float AdcMuxReading<size>::getAdcValueByIndexInVolts(size_t index)
{
    return getAdcValueByAddrInVolts(static_cast<AdcAddr>((int)ADC1 + index));
}

template <std::size_t size>
AdcMuxReading<size>::AdcMuxReading(std::array<int64_t, size> values, adsGain_t new_gain)
{
    for (int i = 0; i < size; i++)
    {
        this->values[i] = values[i];
    }
    gain = new_gain;
}

template <std::size_t size>
void AdcMuxReading<size>::print()
{
    Serial.println("Gain: " + String(gain));
    for (int i = 0; i < 4; i++)
    {
        Serial.print("ADC" + String(i + 1) + ": " + String(getAdcValueByIndexInVolts(i), 4) + "V (" +
                     String(getAdcValueByIndex(i)) + ") ");
    }
    Serial.println();
}

template <std::size_t size>
std::array<float, size> AdcMuxReading<size>::getValuesInVolt()
{
    std::array<float, size> f_array;
    std::copy(values.begin(), values.end(), values.begin());
    return f_array;
}

template <std::size_t size>
AdcMux<size>::AdcMux()
{
    for (int i = 0; i < size; i++)
    {
        adc[i] = Adafruit_ADS1115();
    }
};

template <std::size_t size>
void AdcMux<size>::set_gain(adsGain_t new_gain)
{
    for (int i = 0; i < size; i++)
    {
        adc[i].setGain(new_gain);
    }
    gain = new_gain;
}

template <std::size_t size>
void AdcMux<size>::set_rate(uint16_t rate)
{
    for (int i = 0; i < size; i++)
    {
        adc[i].setDataRate(rate);
    }
}

template <std::size_t size>
void AdcMux<size>::begin()
{
    for (int i = 0; i < size; i++)
    {
        adc[i].begin(AdcAddr::ADC1 + i);
    }
    set_gain(GAIN_TWOTHIRDS);
}

template <std::size_t size>
void AdcMux<size>::start_adc_reading(uint8_t mux, bool continuous)
{
    for (int i = 0; i < size; i++)
    {
        adc[i].startADCReading(mux, continuous);
    }
}

template <std::size_t size>
AdcMuxReading<size> AdcMux<size>::one_shot_reading(uint8_t channel)
{
    std::array<int64_t, size> readings;
    for (int i = 0; i < size; i++)
    {
        readings[i] = adc[i].readADC_SingleEnded(channel);
    }
    return AdcMuxReading(readings, gain);
}

template <std::size_t size>
AdcMuxReading<size> AdcMux<size>::read()
{
    std::array<int64_t, size> readings;
    for (int i = 0; i < size; i++)
    {
        readings[i] = adc[i].getLastConversionResults();
    }
    return AdcMuxReading(readings, gain);
}

template <std::size_t size>
void AdcMux<size>::continuous_reading(uint8_t channel, std::function<void(AdcMuxReading<size>)> callback)
{
    for (int i = 0; i < size; i++)
    {
        adc[i].startADCReading(MUX_BY_CHANNEL[channel], true);
    }
    pinMode(5, INPUT);

    volatile bool new_data = false;

    attachInterrupt(
        digitalPinToInterrupt(14), [&]()
        { new_data = true; },
        FALLING);
    while (true)
    {
        if (new_data)
        {
            new_data = false;
            callback(read());
        }
    }
}
