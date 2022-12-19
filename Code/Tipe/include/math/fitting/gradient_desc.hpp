#ifndef MATH_FITTING_GRADIENT_DESC_HPP
#define MATH_FITTING_GRADIENT_DESC_HPP

#include "../data_set.hpp"
#include "fitting.hpp"
#include <functional>

template <typename T, std::size_t params_count>
using GradientDescFunc = std::function<T(T, std::array<T, params_count>)>;

template <typename T, std::size_t params_count>
class GradientDesc : public FittingResult<T> {
    std::array<T, params_count> params;
    GradientDescFunc<T, params_count> func;

  public:
    GradientDesc(GradientDescFunc<T, params_count> _func,
                 std::array<T, params_count> _params)
        : params(_params), func(_func) {}
    void print();
    T calculateOutput(T input);
    void serialize(std::ostream &os);
};

template <typename T, std::size_t params_count>
class GradientDescFactory : public FittingResultFactory<T> {
    GradientDescFunc<T, params_count> func;
    T cost_func(DataSet<T> &data, std::array<T, params_count> params);
    std::array<T, params_count>
    grad_cost_func(DataSet<T> &data, std::array<T, params_count> params);

  public:
    GradientDescFactory(GradientDescFunc<T, params_count> func) : func(func){};
    std::unique_ptr<FittingResult<T>> deserialize(std::string str);
    std::unique_ptr<FittingResult<T>> getDefault();
    std::unique_ptr<FittingResult<T>> calculateFitting(DataSet<T> &data);
};

template <typename T, std::size_t params_count>
void GradientDesc<T, params_count>::print() {
    Serial.print("GradientDesc: ");
    for (auto param : params) {
        Serial.print(String(param) + " ");
    }
    Serial.println();
}

template <typename T, std::size_t params_count>
T GradientDesc<T, params_count>::calculateOutput(T input) {
    return func(input, params);
}

template <typename T, std::size_t params_count>
void GradientDesc<T, params_count>::serialize(std::ostream &os) {
    for (int i = 0; i < params_count; i++) {
        os << params[i];
        if (i != params_count - 1) {
            os << ",";
        }
    }
}

template <typename T, std::size_t params_count>
T GradientDescFactory<T, params_count>::cost_func(
    DataSet<T> &data, std::array<T, params_count> params) {
    T cost = 0;
    for (auto point : data) {
        cost += std::pow(func(point.x, params) - point.y, 2);
    }
    return cost;
}

template <typename T, std::size_t params_count>
std::array<T, params_count> add_arr(std::array<T, params_count> a,
                                    std::array<T, params_count> b) {
    std::array<T, params_count> result;
    for (int i = 0; i < params_count; i++) {
        result[i] = a[i] + b[i];
    }
    return result;
}

template <typename T, std::size_t params_count>
std::array<T, params_count> sub_arr(std::array<T, params_count> a,
                                    std::array<T, params_count> b) {
    std::array<T, params_count> result;
    for (int i = 0; i < params_count; i++) {
        result[i] = a[i] - b[i];
    }
    return result;
}

template <typename T, std::size_t params_count>
std::array<T, params_count>
GradientDescFactory<T, params_count>::grad_cost_func(
    DataSet<T> &data, std::array<T, params_count> params) {
    std::array<T, params_count> grad;
    std::array<T, params_count> deriv_eps;
    std::fill(deriv_eps.begin(), deriv_eps.end(), T());

    for (int i = 0; i < params_count; i++) {
        deriv_eps[i] = std::numeric_limits<T>::epsilon();

        grad[i] = cost_func(data, add(params, deriv_eps)) -
                  cost_func(data, sub(params, deriv_eps)) /
                      (2 * std::numeric_limits<T>::epsilon());

        deriv_eps[i] = T();
    }
    return grad;
}

template <typename T, std::size_t params_count>
std::unique_ptr<FittingResult<T>>
GradientDescFactory<T, params_count>::calculateFitting(DataSet<T> &data){

};

template <typename T, std::size_t params_count>
std::unique_ptr<FittingResult<T>>
GradientDescFactory<T, params_count>::deserialize(std::string str) {
    auto values = split(str, ',');
    std::array<T, params_count> params;
    for (int i = 0; i < params_count; i++) {
        params[i] = std::stod(values[i]);
    }
    return std::unique_ptr<GradientDesc<T, params_count>>(func, params);
};

template <typename T, std::size_t params_count>
std::unique_ptr<FittingResult<T>>
GradientDescFactory<T, params_count>::getDefault() {
    std::array<T, params_count> params;
    std::fill(params.begin(), params.end(), T());
    return std::unique_ptr<GradientDesc<T, params_count>>(func, params);
}

#endif
