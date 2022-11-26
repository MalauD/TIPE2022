#pragma once
#include <Adafruit_ADS1X15.h>

enum AdcAddr
{
    ADC1 = 0x48,
    ADC2 = 0x49,
    ADC3 = 0x4A,
    ADC4 = 0x4B
};

class AdcMuxReading
{
    int64_t adc1;

  public:
    AdcMuxReading(int16_t adc1, int16_t adc2, int16_t adc3, int16_t adc4);
    int16_t getAdcValueByAddr(AdcAddr addr);
};

class AdcMux
{
  private:
    Adafruit_ADS1115 *adc;
    uint8_t adc_count;

  public:
    AdcMux();
    void set_gain(adsGain_t gain);
    void set_rate(uint16_t rate);
    void begin();
    void start_adc_reading(uint8_t mux, bool continuous);
    AdcMuxReading one_shot_reading(uint8_t channel);
    AdcMuxReading read();
};