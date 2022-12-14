#pragma once
#include "../io/adc_mux.hpp"
#include "../math/fitting/fitting.hpp"
#include "../config/config.hpp"
#include <Arduino.h>

template <typename T, std::size_t size>
class CalInterface
{
  AdcMux adc;
  DataSet<T> dataSet;

public:
  CalInterface() : adc(AdcMux()){};
  void start(std::unique_ptr<Config<T, size>> config);
};