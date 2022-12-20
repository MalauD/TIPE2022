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

  public:
    LinearRegression(T slope, T intercept)
        : slope(slope), intercept(intercept) {}
    void print();
    T calculateOutput(T input);
    void serialize(std::ostream &os);
};

template <typename T>
class LinearRegressionStats : public FittingResultStats<T> {
    T r;
    T r2;
    T rmse;

  public:
    LinearRegressionStats(T r, T r2, T rmse) : r(r), r2(r2), rmse(rmse) {}
    void print();
};

template <typename T>
class LinearRegressionFactory : public FittingResultFactory<T> {
    LinearRegressionStats<T> last_stats;

  public:
    LinearRegressionFactory() : last_stats(0, 0, 0) {}
    std::unique_ptr<FittingResult<T>> deserialize(std::string str);
    std::unique_ptr<FittingResult<T>> getDefault();
    std::unique_ptr<FittingResult<T>> calculateFitting(DataSet<T> &data);
    std::unique_ptr<FittingResultStats<T>> getLastCalculationStats();
};

template <typename T>
void LinearRegressionStats<T>::print() {
    Serial.println("Linear Regression Stats:");
    Serial.println("R: " + String(r, 3));
    Serial.println("R^2: " + String(r2, 3));
    Serial.println("RMSE: " + String(rmse, 3));
}

template <typename T>
void LinearRegression<T>::print() {
    Serial.println("Linear Regression:");
    Serial.println("Slope: " + String(slope));
    Serial.println("Intercept: " + String(intercept));
}

template <typename T>
std::unique_ptr<FittingResult<T>>
LinearRegressionFactory<T>::calculateFitting(DataSet<T> &data_inv) {
    auto data = data_inv.map([](DataPoint<T> dp) {
        return DataPoint<T>{1 / dp.x, dp.y};
    });
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
    T r2 = r * r;
    T rmse = std::sqrt(data.accumulate([slope, intercept](DataPoint<T> dp) {
        return std::pow(dp.y - (slope * dp.x + intercept), 2);
    }) / dataSetSize);

    last_stats = LinearRegressionStats<T>(r, r2, rmse);

    return std::make_unique<LinearRegression<T>>(slope, intercept);
}

template <typename T>
std::unique_ptr<FittingResultStats<T>>
LinearRegressionFactory<T>::getLastCalculationStats() {
    return std::make_unique<LinearRegressionStats<T>>(last_stats);
}

template <typename T>
T LinearRegression<T>::calculateOutput(T input) {
    return slope * (1 / input) + intercept;
}

template <typename T>
void LinearRegression<T>::serialize(std::ostream &os) {
    os << slope << "," << intercept;
}

template <typename T>
std::unique_ptr<FittingResult<T>>
LinearRegressionFactory<T>::deserialize(std::string str) {
    auto values = split(str, ',');
    return std::make_unique<LinearRegression<T>>(std::stof(values[0]),
                                                 std::stof(values[1]));
}

template <typename T>
std::unique_ptr<FittingResult<T>> LinearRegressionFactory<T>::getDefault() {
    return std::make_unique<LinearRegression<T>>(0, 0);
}

#endif