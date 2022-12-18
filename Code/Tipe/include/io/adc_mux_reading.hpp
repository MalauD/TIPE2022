#ifndef IO_ADC_MUX_READING_HPP
#define IO_ADC_MUX_READING_HPP

#include "Adafruit_ADS1X15.h"
#include "adc_addr.hpp"
#include <array>
#include <cstdint>

template <std::size_t size>
class AdcMuxReading {
    std::array<int64_t, size> values;
    adsGain_t gain;

  public:
    AdcMuxReading(std::array<int64_t, size> values, adsGain_t gain);
    void print();
    std::array<float, size> getValuesInVolt();
    int16_t getAdcValueByAddr(AdcAddr addr);
    int16_t getAdcValueByIndex(std::size_t index);
    float getAdcValueByIndexInVolts(std::size_t addr);
    float getAdcValueByAddrInVolts(AdcAddr addr);
};

template <std::size_t size>
int16_t AdcMuxReading<size>::getAdcValueByAddr(AdcAddr addr) {
    return values[addr - ADC1];
}

template <std::size_t size>
int16_t AdcMuxReading<size>::getAdcValueByIndex(std::size_t index) {
    return values[index];
}

template <std::size_t size>
float AdcMuxReading<size>::getAdcValueByAddrInVolts(AdcAddr addr) {
    float fsRange;
    switch (gain) {
    case GAIN_TWOTHIRDS:
        fsRange = 6.144f;
        break;
    case GAIN_ONE:
        fsRange = 4.096f;
        break;
    case GAIN_TWO:
        fsRange = 2.048f;
        break;
    case GAIN_FOUR:
        fsRange = 1.024f;
        break;
    case GAIN_EIGHT:
        fsRange = 0.512f;
        break;
    case GAIN_SIXTEEN:
        fsRange = 0.256f;
        break;
    default:
        fsRange = 0.0f;
    }
    return getAdcValueByAddr(addr) * (fsRange / 32768);
}

template <std::size_t size>
float AdcMuxReading<size>::getAdcValueByIndexInVolts(std::size_t index) {
    return getAdcValueByAddrInVolts(static_cast<AdcAddr>((int)ADC1 + index));
}

template <std::size_t size>
AdcMuxReading<size>::AdcMuxReading(std::array<int64_t, size> values,
                                   adsGain_t new_gain) {
    std::copy(values.begin(), values.end(), this->values.begin());
    gain = new_gain;
}

template <std::size_t size>
void AdcMuxReading<size>::print() {
    Serial.println("Gain: " + String(gain));
    for (std::size_t i = 0; i < size; i++) {
        Serial.print("ADC" + String(i + 1) + ": " +
                     String(getAdcValueByIndexInVolts(i), 4) + "V (" +
                     String(getAdcValueByIndex(i)) + ") ");
    }
    Serial.println();
}

template <std::size_t size>
std::array<float, size> AdcMuxReading<size>::getValuesInVolt() {
    std::array<float, size> f_array;
    for (std::size_t i = 0; i < size; i++) {
        f_array[i] = getAdcValueByIndexInVolts(i);
    }
    return f_array;
}

#endif