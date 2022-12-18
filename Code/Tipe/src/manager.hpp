#include "./calibration/interface.hpp"
#include "./config/config.hpp"
#include "./io/sd_logging.hpp"
#include "Arduino.h"
#include <FunctionalInterrupt.h>
#include <memory>

enum ManagerMenus {
    CALIBRATION = 1,
    LOAD_CONFIG,
    SAVE_CONFIG,
    VIEW_CONFIG,
    LOAD_DEFAULT_CONFIG,
    MEASUREMENT_SD,
    MEASUREMENT_SERIAL,
    MEASUREMENT_RAW
};

template <typename T, std::size_t size>
class Manager {
    CalInterface<T, size> interface;
    ConfigManager<T, size> configManager;
    Config<T, size> config;
    SDLogging sdLogging;

    ManagerMenus menu();
    void measureSerial(bool logRaw);
    void measureSD();

  public:
    Manager() = delete;
    Manager(std::unique_ptr<FittingResultFactory<T>> fittingType)
        : config(Config<T, size>{std::move(fittingType)}) {}
    void run();
};

template <typename T, std::size_t size>
ManagerMenus Manager<T, size>::menu() {
    long choice = 0;
    Serial.println("Menu:");
    Serial.println("1. Start calibration");
    Serial.println("2. Load config");
    Serial.println("3. Save config");
    Serial.println("4. View current config");
    Serial.println("5. Load default config");
    Serial.println("6. Start measurement (SD card)");
    Serial.println("7. Start measurement (Serial port)");
    Serial.println("8. Start measurement (Raw)");
    while (choice <= 0 || choice >= 9) {
        while (!Serial.available())
            ;
        choice = Serial.parseInt();
    }

    return static_cast<ManagerMenus>(choice);
}

template <typename T, std::size_t size>
void Manager<T, size>::measureSerial(bool logRaw) {
    Serial.println("Press any key to start measurement (Serial).");
    while (!Serial.available())
        ;
    Serial.read();
    Serial.println("Starting measurement...");
    AdcMux<size> adc;
    adc.begin();
    adc.set_gain(GAIN_ONE);
    adc.set_rate(RATE_ADS1115_860SPS);

    // adc.start_adc_reading(ADS1X15_REG_CONFIG_MUX_SINGLE_0, false);

    int t = millis();
    int measure_count = 0;

    int measure_rate = 0;

    std::array<float, size> weight;

    auto cb = [&](AdcMuxReading<size> reading) {
        config.convertToWeight(reading.getValuesInVolt(), weight);
        if (measure_count % 10 == 0) {
            if (logRaw) {
                reading.print();
            } else {
                Serial.println("Weight: " + String(weight[0]) + "g, " +
                               String(weight[1]) + "g, " + String(weight[2]) +
                               "g, " + String(weight[3]) +
                               "g, Rate: " + String(measure_rate) + "Hz");
            }
        }

        measure_count++;
        if (millis() - t > 1000) {
            measure_rate = measure_count;
            t = millis();
            measure_count = 0;
        }
    };

    adc.continuous_reading(0, cb);
}

template <typename T, std::size_t size>
void Manager<T, size>::measureSD() {
    Serial.println("Starting measurement...");
    AdcMux<size> adc;
    adc.begin();
    adc.set_gain(GAIN_ONE);
    adc.set_rate(RATE_ADS1115_860SPS);

    int t = millis();
    int measure_count = 0;

    int measure_rate = 0;

    auto cb = [&](AdcMuxReading<size> reading) {
        sdLogging.logWeights(reading, config);
        if (measure_count % 10 == 0) {
            Serial.println("Rate: " + String(measure_rate) + "Hz");
        }

        measure_count++;
        if (millis() - t > 1000) {
            measure_rate = measure_count;
            t = millis();
            measure_count = 0;
        }
    };

    adc.continuous_reading(0, cb);
}

template <typename T, std::size_t size>
void Manager<T, size>::run() {
    configManager.begin();
    sdLogging.begin();
    configManager.retreiveConfig(config);
    Wire.setClock(400000);
    while (true) {
        switch (menu()) {
        case CALIBRATION:
            interface.start(config);
            break;
        case LOAD_CONFIG:
            configManager.retreiveConfig(config);
            break;
        case SAVE_CONFIG:
            configManager.saveConfig(config);
            break;
        case VIEW_CONFIG:
            config.print();
            break;
        case LOAD_DEFAULT_CONFIG:
            config.setToDefault();
            break;
        case MEASUREMENT_SD:
            measureSD();
            break;
        case MEASUREMENT_SERIAL:
            measureSerial(false);
            break;
        case MEASUREMENT_RAW:
            measureSerial(true);
            break;
        default:
            break;
        }
        delay(100);
    }
}