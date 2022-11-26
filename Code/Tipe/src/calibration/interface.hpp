#pragma once
#include "../io/adc_mux.hpp"
#include <Arduino.h>

class CalInterface
{
    AdcMux adc;

  public:
    CalInterface() : adc(AdcMux()){};
    void start();
};