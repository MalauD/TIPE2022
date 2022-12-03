#include "./calibration/interface.hpp"
#include "./config/config.hpp"
#include <FunctionalInterrupt.h>

enum ManagerMenus
{
    CALIBRATION = 1,
    LOAD_CONFIG,
    SAVE_CONFIG,
    MEASUREMENT_SD,
    MEASUREMENT_SERIAL,
};

class Manager
{
    CalInterface interface;
    ConfigManager configManager;
    Config *config;

    ManagerMenus menu();

  public:
    Manager();
    void run();
};

Manager::Manager()
{
    interface = CalInterface();
    configManager = ConfigManager();
    config = configManager.getConfig();
}

ManagerMenus Manager::menu()
{
    long choice = 0;
    while (choice <= 0 && choice >= 6)
    {
        Serial.println("Menu:");
        Serial.println("1. Start calibration");
        Serial.println("2. Load config");
        Serial.println("3. Save config");
        Serial.println("4. Start measurement (SD card)");
        Serial.println("5. Start measurement (Serial port)");
        while (!Serial.available())
            ;
        choice = Serial.parseInt();
    }

    return static_cast<ManagerMenus>(choice);
}

void measureSerial(Config *config)
{
    Serial.println("Press any key to start measurement (Serial).");
    while (!Serial.available())
        ;
    Serial.read();
    Serial.println("Starting measurement...");
    AdcMux adc;
    adc.begin();
    adc.set_gain(GAIN_ONE);
    adc.set_rate(RATE_ADS1115_860SPS);
    adc.start_adc_reading(ADS1X15_REG_CONFIG_MUX_SINGLE_0, true);
    auto cb = [&adc, config]() {
        auto reading = adc.read();
        };
    attachInterrupt(digitalPinToInterrupt(0), cb, FALLING);
}

void Manager::run()
{
    configManager.begin();
    while (true)
    {
        switch (menu())
        {
        case CALIBRATION:
            interface.start();
            break;
        case LOAD_CONFIG:
            config = configManager.getConfig();
            break;
        case SAVE_CONFIG:
            configManager.saveConfig(*config);
            break;
        case MEASUREMENT_SD:
            break;
        case MEASUREMENT_SERIAL:
            break;
        default:
            break;
        }
    }
}