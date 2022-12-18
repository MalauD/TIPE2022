#ifndef IO_ADC_MUX_HPP
#define IO_ADC_MUX_HPP

#include "../config/config.hpp"
#include "Adafruit_ADS1X15.h"
#include "adc_mux_reading.hpp"

template <std::size_t size>
class AdcMux {
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
    void continuous_reading(uint8_t channel,
                            std::function<void(AdcMuxReading<size>)> callback);
};

template <std::size_t size>
AdcMux<size>::AdcMux() {
    for (auto &a : adc) {
        a = Adafruit_ADS1115();
    }
};

template <std::size_t size>
void AdcMux<size>::set_gain(adsGain_t new_gain) {
    for (auto &a : adc) {
        a.setGain(new_gain);
    }
    gain = new_gain;
}

template <std::size_t size>
void AdcMux<size>::set_rate(uint16_t rate) {
    for (auto &a : adc) {
        a.setDataRate(rate);
    }
}

template <std::size_t size>
void AdcMux<size>::begin() {
    for (std::size_t i = 0; i < size; i++) {
        adc[i].begin(AdcAddr::ADC1 + i);
    }
    set_gain(GAIN_TWOTHIRDS);
}

template <std::size_t size>
void AdcMux<size>::start_adc_reading(uint8_t mux, bool continuous) {
    for (auto &a : adc) {
        a.startADCReading(mux, continuous);
    }
}

template <std::size_t size>
AdcMuxReading<size> AdcMux<size>::one_shot_reading(uint8_t channel) {
    std::array<int64_t, size> readings;
    for (std::size_t i = 0; i < size; i++) {
        readings[i] = adc[i].readADC_SingleEnded(channel);
    }
    return AdcMuxReading(readings, gain);
}

template <std::size_t size>
AdcMuxReading<size> AdcMux<size>::read() {
    std::array<int64_t, size> readings;
    for (std::size_t i = 0; i < size; i++) {
        readings[i] = adc[i].getLastConversionResults();
    }
    return AdcMuxReading(readings, gain);
}

template <std::size_t size>
void AdcMux<size>::continuous_reading(
    uint8_t channel, std::function<void(AdcMuxReading<size>)> callback) {
    for (auto &a : adc) {
        a.startADCReading(MUX_BY_CHANNEL[channel], true);
    }
    pinMode(5, INPUT);

    volatile bool new_data = false;

    attachInterrupt(
        digitalPinToInterrupt(14), [&]() { new_data = true; }, FALLING);
    while (true) {
        if (new_data) {
            new_data = false;
            callback(read());
        }
    }
}

#endif