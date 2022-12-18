#ifndef MATH_FITTING_LINEAR_REGRESSION_HPP
#define MATH_FITTING_LINEAR_REGRESSION_HPP

#include "../data_set.hpp"
#include "fitting.hpp"
#include <cmath>
#include <vector>

template <typename T>
class LinearRegression : public FittingResult<T> {
    T slope;
    T intercept;
    T r;

  public:
    LinearRegression(T slope, T intercept, T r)
        : slope(slope), intercept(intercept), r(r) {}
    void print();
    T calculateOutput(T input);
    void serialize(std::ostream &os);
};

template <typename T>
class LinearRegressionFactory : public FittingResultFactory<T> {
  public:
    std::unique_ptr<FittingResult<T>> deserialize(std::string str);
    std::unique_ptr<FittingResult<T>> getDefault();
    std::unique_ptr<FittingResult<T>> calculateFitting(DataSet<T> &data);
};

template <typename T>
void LinearRegression<T>::print() {
    Serial.println("Linear Regression:");
    Serial.println("Slope: " + String(slope));
    Serial.println("Intercept: " + String(intercept));
    Serial.println("R: " + String(r));
}

template <typename T>
std::unique_ptr<FittingResult<T>>
LinearRegressionFactory<T>::calculateFitting(DataSet<T> &data) {
    T sx = data.accumulate([](DataPoint<T> dp) { return dp.x; });
    T sy = data.accumulate([](DataPoint<T> dp) { return dp.y; });
    T sxx = data.accumulate([](DataPoint<T> dp) { return dp.x * dp.x; });
    T syy = data.accumulate([](DataPoint<T> dp) { return dp.y * dp.y; });
    T sxy = data.accumulate([](DataPoint<T> dp) { return dp.x * dp.y; });

    auto dataSetSize = data.size();

    T slope = (dataSetSize * sxy - sx * sy) / (dataSetSize * sxx - sx * sx);
    T intercept = (sy - slope * sx) / dataSetSize;

    T r = (dataSetSize * sxy - sx * sy) /
          std::sqrt((dataSetSize * sxx - sx * sx) *
                    (dataSetSize * syy - sy * sy));

    return std::make_unique<LinearRegression<T>>(slope, intercept, r);
}

template <typename T>
T LinearRegression<T>::calculateOutput(T input) {
    return slope * (1 / input) + intercept;
}

template <typename T>
void LinearRegression<T>::serialize(std::ostream &os) {
    os << slope << "," << intercept << "," << r;
}

template <typename T>
std::unique_ptr<FittingResult<T>>
LinearRegressionFactory<T>::deserialize(std::string str) {
    auto values = split(str, ',');
    return std::make_unique<LinearRegression<T>>(
        std::stof(values[0]), std::stof(values[1]), std::stof(values[2]));
}

template <typename T>
std::unique_ptr<FittingResult<T>> LinearRegressionFactory<T>::getDefault() {
    return std::make_unique<LinearRegression<T>>(0, 0, 0);
}

#endif