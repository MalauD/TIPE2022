#pragma once
#include "../io/adc_mux.hpp"
#include "../math/linear_regression.hpp"
#include <Arduino.h>

class CalInterface
{
  AdcMux adc;
  DataSet<float> dataSet;

public:
  CalInterface() : adc(AdcMux()){};
  void start(Config *config);
};