#include "manager.hpp"
#include "math/fitting/gradient_desc.hpp"
#include "math/fitting/linear_regression.hpp"
#include <Arduino.h>

void setup() {
    Serial.begin(115200);
    auto func = [](float x, std::array<float, 3> coef) {
        return coef[0] / std::pow(x, 2) + coef[1] / x + coef[2];
    };
    GradientDescSettings<float> settings(0.1, 1e-5, 10000);

    auto factory_grad =
        std::make_unique<GradientDescFactory<float, 3>>(func, settings);

    auto factory = std::make_unique<LinearRegressionFactory<float>>();
    Manager<float, 4> manager{std::move(factory_grad)};
    manager.run();
}

void loop() {}
