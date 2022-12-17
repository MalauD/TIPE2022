#pragma once
#include "Arduino.h"
#include "./adc_mux.hpp"

class SDLogging
{
    unsigned long lastLogTime = 0;

public:
    SDLogging(){};
    void begin();
    template <typename T, std::size_t size>
    void logWeights(AdcMuxReading<size> reading, Config<T, size> &config);
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

template <typename T, std::size_t size>
void SDLogging::logWeights(AdcMuxReading<size> reading, Config<T, size> &config)
{
    std::array<T, size> weight;
    config.convertToWeight(reading.getValuesInVolt(), weight);
    std::ostringstream ss;
    ss << getLogIntervalMicros() << ",";
    for (std::size_t i = 0; i < size; i++)
    {
        ss << weight[i];
        if (i < size - 1)
        {
            ss << ",";
        }
    }
    Serial1.println(ss.str().c_str());
}