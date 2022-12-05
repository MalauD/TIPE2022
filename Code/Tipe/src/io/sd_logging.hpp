#pragma once
#include "Arduino.h"
#include "./adc_mux.hpp"

class SDLogging
{
public:
    SDLogging(){};
    void begin();
    void logWeights(AdcMuxReading reading, Config *config);
};

void SDLogging::begin()
{
    Serial1.begin(57600);
    Serial.println("SD card initialization done.");
}

void SDLogging::logWeights(AdcMuxReading reading, Config *config)
{
    float weight[ADC_MAX_COUNT];
    reading.convert_to_weight(config, weight);
    Serial1.println(millis() + "," + String(weight[0]) + "," + String(weight[1]) + "," + String(weight[2]) + "," + String(weight[3]));
}