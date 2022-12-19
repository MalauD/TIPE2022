#include "manager.hpp"
#include "math/fitting/gradient_desc.hpp"
#include "math/fitting/linear_regression.hpp"
#include <Arduino.h>

void setup() {
    Serial.begin(115200);
    auto factory = std::make_unique<LinearRegressionFactory<float>>();
    Manager<float, 4> manager{std::move(factory)};
    manager.run();
}

void loop() {}
