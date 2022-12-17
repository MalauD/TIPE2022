#pragma once
#include "../config/config.hpp"
#include "../io/adc_mux.hpp"
#include "../math/fitting/fitting.hpp"
#include <Arduino.h>

template <typename T, std::size_t size> class CalInterface {
    AdcMux<size> adc;
    DataSet<T> dataSet;

  public:
    CalInterface(){};
    void start(Config<T, size> &config);
};