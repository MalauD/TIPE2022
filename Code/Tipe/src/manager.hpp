#include "./calibration/interface.hpp"
#include "./config/config.hpp"

class Manager
{
    CalInterface interface;
    ConfigManager configManager;

  public:
    Manager();
    void run();
};

Manager::Manager()
{
    interface = CalInterface();
    configManager = ConfigManager();
}

void Manager::run()
{
    interface.start();
    configManager.begin();

    
}