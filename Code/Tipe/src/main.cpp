#include "manager.hpp"
#include "math/fitting/gradient_desc.hpp"
#include "math/fitting/linear_regression.hpp"
#include <Arduino.h>

void setup() {
    Serial.begin(115200);
    auto func = [](double x, std::array<double, 3> coef) {
        return coef[0] / std::pow(x, coef[1]) + coef[2];
    };
    GradientDescSettings<double, 3> settings(0.01, 1e-3, 5000,
                                             {6000, 0.7, -2800});

    auto factory_grad =
        std::make_unique<GradientDescFactory<double, 3>>(func, settings);

    auto factory = std::make_unique<LinearRegressionFactory<double>>();
    Manager<double, 4> manager{std::move(factory_grad)};
    manager.run();
}

void loop() {}
