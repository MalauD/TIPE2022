#pragma once
#include "Arduino.h"
#include "./adc_mux.hpp"

class SDLogging
{
    unsigned long lastLogTime = 0;

public:
    SDLogging(){};
    void begin();
    void logWeights(AdcMuxReading reading, Config *config);
    unsigned long getLogIntervalMicros()
    {
        auto now = micros();
        auto interval = now - lastLogTime;
        lastLogTime = now;
        return interval;
    };
};

void SDLogging::begin()
{
    Serial1.begin(115200);
    Serial.println("SD card initialization done.");
}

void SDLogging::logWeights(AdcMuxReading reading, Config *config)
{
    float weight[ADC_MAX_COUNT];
    reading.convert_to_weight(config, weight);
    Serial1.println(String(getLogIntervalMicros()) + "," + String(weight[0], 0) + "," + String(weight[1], 0) + "," + String(weight[2], 0) + "," + String(weight[3], 0));
}