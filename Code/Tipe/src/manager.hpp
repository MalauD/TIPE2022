#include "./calibration/interface.hpp"
#include "./config/config.hpp"

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