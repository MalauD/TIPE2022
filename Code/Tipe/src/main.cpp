#include "./calibration/interface.hpp"
#include <Arduino.h>

CalInterface interface;

void setup()
{
    Serial.begin(115200);
    interface.start();
}

void loop()
{
}
