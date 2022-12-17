#include "manager.hpp"
#include <Arduino.h>

Manager<float, 4> manager;

void setup() {
    Serial.begin(115200);
    manager.run();
}

void loop() {}
