#pragma once
#include "Adafruit_ADS1X15.h"
#include "../config/config.hpp"

enum AdcAddr
{
  ADC1 = 0x48,
  ADC2 = 0x49,
  ADC3 = 0x4A,
  ADC4 = 0x4B
};

template <std::size_t size>
class AdcMuxReading
{
  std::array<int64_t, size> values;
  adsGain_t gain;

public:
  AdcMuxReading(std::array<int64_t, size> values, adsGain_t gain);
  void print();
  std::array<float, size> getValuesInVolt();
  int16_t getAdcValueByAddr(AdcAddr addr);
  int16_t getAdcValueByIndex(size_t index);
  float getAdcValueByIndexInVolts(size_t addr);
  float getAdcValueByAddrInVolts(AdcAddr addr);
};

template <std::size_t size>
class AdcMux
{
private:
  std::array<Adafruit_ADS1115, size> adc;
  adsGain_t gain;

public:
  AdcMux();
  void set_gain(adsGain_t gain);
  void set_rate(uint16_t rate);
  void begin();
  void start_adc_reading(uint8_t mux, bool continuous);
  AdcMuxReading<size> one_shot_reading(uint8_t channel);
  AdcMuxReading<size> read();
  void continuous_reading(uint8_t channel, std::function<void(AdcMuxReading<size>)> callback);
};