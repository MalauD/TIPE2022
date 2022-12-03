#pragma once
#include "../config/config.hpp"
#include <Adafruit_ADS1X15.h>

constexpr int ADC_MAX_COUNT = 4;

enum AdcAddr
{
  ADC1 = 0x48,
  ADC2 = 0x49,
  ADC3 = 0x4A,
  ADC4 = 0x4B
};

class AdcMuxReading
{
  int64_t values[ADC_MAX_COUNT];
  adsGain_t gain;

public:
  AdcMuxReading(int16_t adc1, int16_t adc2, int16_t adc3, int16_t adc4, adsGain_t gain);
  void print();
  int16_t getAdcValueByAddr(AdcAddr addr);
  int16_t getAdcValueByIndex(size_t index);
  float getAdcValueByIndexInVolts(size_t addr);
  float getAdcValueByAddrInVolts(AdcAddr addr);
};

class AdcMux
{
private:
  Adafruit_ADS1115 *adc;
  uint8_t adc_count;
  adsGain_t gain;

public:
  AdcMux();
  void set_gain(adsGain_t gain);
  void set_rate(uint16_t rate);
  void begin();
  void start_adc_reading(uint8_t mux, bool continuous);
  AdcMuxReading one_shot_reading(uint8_t channel);
  AdcMuxReading read();
  float *convert_to_weight(AdcMuxReading reading, Config *config);
};