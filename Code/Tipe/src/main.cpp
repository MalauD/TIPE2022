#include "manager.hpp"
#include <Arduino.h>

Manager manager;

void setup()
{
    Serial.begin(115200);
    manager.run();
}

void loop()
{
}
