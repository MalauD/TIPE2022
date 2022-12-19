#ifndef MATH_FITTING_GRADIENT_DESC_HPP
#define MATH_FITTING_GRADIENT_DESC_HPP

#include "../data_set.hpp"
#include "fitting.hpp"
#include <functional>

template <typename T, T... Params>
using GradientDescFunc = std::function<T(T, Params...)>;

template <typename T, T... Params>
class GradientDesc : public FittingResult<T> {
    std::array<T, sizeof...(Params)> params;
    GradientDescFunc func;

  public:
    GradientDesc(GradientDescFunc _func,
                 std::array<T, sizeof...(Params)> _params)
        : params(_params), func(_func) {}
    void print();
    T calculateOutput(T input);
    void serialize(std::ostream &os);
};

template <typename T, T... Params>
class GradientDescFactory : public FittingResultFactory<T> {
    GradientDescFunc func;

  public:
    GradientDescFactory(GradientDescFunc func) : func(func){};
    std::unique_ptr<FittingResult<T>> deserialize(std::string str);
    std::unique_ptr<FittingResult<T>> getDefault();
    std::unique_ptr<FittingResult<T>> calculateFitting(DataSet<T> &data);
};

template <typename T, T... Params>
void GradientDesc<T, Params...>::print() {
    Serial.print("GradientDesc: ");
    for (auto param : params) {
        Serial.print(String(param) + " ");
    }
    Serial.println();
}

template <typename T, T... Params>
T GradientDesc<T, Params...>::calculateOutput(T input) {
    return func(input, params...);
}

template <typename T, T... Params>
void GradientDesc<T, Params...>::serialize(std::ostream &os) {
    for (int i = 0; i < sizeof...(Params); i++) {
        os << params[i];
        if (i != sizeof...(Params) - 1) {
            os << ",";
        }
    }
}

template <typename T, T... Params>
std::unique_ptr<FittingResult<T>>
GradientDescFactory<T, Params...>::deserialize(std::string str) {
    auto values = split(str, ',');
    std::array<T, sizeof...(Params)> params;
    for (int i = 0; i < sizeof...(Params); i++) {
        params[i] = std::stod(values[i]);
    }
    return std::unique_ptr<GradientDesc<T>>(params);
};

template <typename T, T... Params>
std::unique_ptr<FittingResult<T>>
GradientDescFactory<T, Params...>::getDefault() {
    std::array<T, sizeof...(Params)> params;
    std::fill(params.begin(), params.end(), 0);
    return std::unique_ptr<GradientDesc<T>>(params);
};

#endif
