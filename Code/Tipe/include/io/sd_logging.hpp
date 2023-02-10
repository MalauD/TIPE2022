#ifndef IO_SD_LOGGING_HPP
#define IO_SD_LOGGING_HPP

#include "./adc_mux.hpp"
#include "Arduino.h"

class SDLogging {
    unsigned long lastLogTime = 0;

  public:
    SDLogging() = default;
    void begin();
    template <typename T, std::size_t size>
    void logWeights(AdcMuxReading<size> reading, Config<T, size> &config);
    void logMpuReading(MpuReading &reading);

    unsigned long getLogIntervalMicros() {
        auto now = micros();
        auto interval = now - lastLogTime;
        lastLogTime = now;
        return interval;
    };
};

void SDLogging::begin() {
    Serial1.begin(115200);
    Serial.println("SD card initialization done.");
    Serial1.print("##########");
}

template <typename T, std::size_t size>
void SDLogging::logWeights(AdcMuxReading<size> reading,
                           Config<T, size> &config) {
    std::array<T, size> weight;
    std::array<T, size> volt = reading.template getValuesInVolt<T>();
    config.convertToWeight(volt, weight);

    auto interval = static_cast<uint16_t>(getLogIntervalMicros());
    Serial1.write((uint8_t *)&interval, sizeof(interval));

    std::array<int16_t, size> weightInt;
    for (size_t i = 0; i < size; i++) {
        weightInt[i] = static_cast<int16_t>(weight[i]);
    }
    for (size_t i = 0; i < size; i++) {
        Serial1.write((uint8_t *)&weightInt[i], sizeof(weightInt[i]));
    }
}

void SDLogging::logMpuReading(MpuReading &reading) {
    Serial1.write((uint8_t *)&reading, sizeof(reading));
}

#endif