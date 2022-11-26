#include "adc_mux.hpp"

int16_t AdcMuxReading::getAdcValueByAddr(AdcAddr addr)
{
    switch (addr)
    {
    case ADC1:
        return adc1 >> 0;
    case ADC2:
        return adc1 >> 16;
    case ADC3:
        return adc1 >> 32;
    case ADC4:
        return adc1 >> 48;
    }
    return -1;
}

AdcMuxReading::AdcMuxReading(int16_t adc1, int16_t adc2, int16_t adc3, int16_t adc4)
{
    this->adc1 = adc1;
    this->adc1 |= (int64_t)adc2 << 16;
    this->adc1 |= (int64_t)adc3 << 32;
    this->adc1 |= (int64_t)adc4 << 48;
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
    return AdcMuxReading(adc1, adc2, adc3, adc4);
}

AdcMuxReading AdcMux::read()
{
    int16_t adc1 = adc[0].getLastConversionResults();
    int16_t adc2 = adc[1].getLastConversionResults();
    int16_t adc3 = adc[2].getLastConversionResults();
    int16_t adc4 = adc[3].getLastConversionResults();
    return AdcMuxReading(adc1, adc2, adc3, adc4);
}