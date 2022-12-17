#pragma once
#include "../data_set.hpp"
#include "fitting.hpp"
#include <math.h>
#include <vector>

template <typename T> class LinearRegression : public FittingResult<T> {
    T slope;
    T intercept;
    T r;

  public:
    LinearRegression(T slope, T intercept, T r)
        : slope(slope), intercept(intercept), r(r){};
    void print();
    static FittingResult<T> &calculateFitting(DataSet<T> data) = 0;
    T calculateOutput(T input);
    friend std::ostream &operator<<(std::ostream &stream,
                                    const LinearRegression &fitting);
    static FittingResult<T> &parse(std::string str) = 0
};

template <typename T> void LinearRegression<T>::print() {
    Serial.println("Linear Regression:");
    Serial.println("Slope: " + String(slope));
    Serial.println("Intercept: " + String(intercept));
    Serial.println("R: " + String(r));
}

template <typename T> FittingResult<T> calculateFitting(DataSet<T> data) {
    T sx = data.accumulate([](DataPoint<T> dp) { return dp.x; });
    T sy = data.accumulate([](DataPoint<T> dp) { return dp.y; });
    T sxx = data.accumulate([](DataPoint<T> dp) { return dp.x * dp.x; });
    T syy = data.accumulate([](DataPoint<T> dp) { return dp.y * dp.y; });
    T sxy = data.accumulate([](DataPoint<T> dp) { return dp.x * dp.y; });

    auto dataSetSize = data.size();

    T slope = (dataSetSize * sxy - sx * sy) / (dataSetSize * sxx - sx * sx);
    T intercept = (sy - slope * sx) / dataSetSize;

    T r = (dataSetSize * sxy - sx * sy) /
          sqrt((dataSetSize * sxx - sx * sx) * (dataSetSize * syy - sy * sy));

    return LinearRegression<T>(slope, intercept, r);
}

template <typename T> T LinearRegression<T>::calculateOutput(T input) {
    return slope * input + intercept;
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const LinearRegression<T> &fitting) {
    os << fitting.slope << "," << fitting.intercept << "," << fitting.r;
    return os;
}

template <typename T> FittingResult<T> &parse(std::string str) {
    auto values = split(line, ',');
    FittingResult<T> fittingResult = new FittingResult<T>();
    fittingResult.slope = std::stof(values[0]);
    fittingResult.intercept = std::stof(values[1]);
    fittingResult.r = std::stof(values[2]);
    return fittingResult;
}
