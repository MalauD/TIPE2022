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

enum GradientDescCompletion {
    GRADIENT_DESC_CONVERGED,
    GRADIENT_DESC_MAX_ITERATIONS,
};

std::string gradientDescCompletionToString(GradientDescCompletion completion);

template <typename T>
class GradientDescStats : public FittingResultStats<T> {
    GradientDescCompletion completion;
    T sse;
    T r2;
    T rmse;
    long iterations;

  public:
    GradientDescStats()
        : completion(GRADIENT_DESC_MAX_ITERATIONS), sse(T()), r2(T()),
          rmse(T()), iterations(T()) {}
    GradientDescStats(GradientDescCompletion _completion, T _sse, T _r2,
                      T _rmse, long _iterations)
        : completion(_completion), sse(_sse), r2(_r2), rmse(_rmse),
          iterations(_iterations) {}
    void print();
};

template <typename T, std::size_t params_count>
class GradientDescSettings {
    T learning_rate;
    T tolerance;
    long max_iterations;
    std::array<T, params_count> initial_params;

  public:
    GradientDescSettings(T _learning_rate, T _tolerance, long _max_iterations,
                         std::array<T, params_count> _initial_params)
        : learning_rate(_learning_rate), tolerance(_tolerance),
          max_iterations(_max_iterations), initial_params(_initial_params) {}
    T getLearningRate() { return learning_rate; }
    T getTolerance() { return tolerance; }
    long getMaxIterations() { return max_iterations; }
    std::array<T, params_count> getInitialParams() { return initial_params; }
};

template <typename T, std::size_t params_count>
class GradientDescFactory : public FittingResultFactory<T> {
    GradientDescFunc<T, params_count> func;
    GradientDescSettings<T, params_count> settings;
    GradientDescStats<T> last_stats;

    T cost_func(DataSet<T> &data, std::array<T, params_count> params);
    std::array<T, params_count>
    grad_cost_func(DataSet<T> &data, std::array<T, params_count> params);

  public:
    GradientDescFactory() = default;
    GradientDescFactory(GradientDescFunc<T, params_count> _func,
                        GradientDescSettings<T, params_count> _settings)
        : func(_func), settings(_settings){};
    std::unique_ptr<FittingResult<T>> deserialize(std::string str);
    std::unique_ptr<FittingResult<T>> getDefault();
    std::unique_ptr<FittingResult<T>> calculateFitting(DataSet<T> &data);
    std::unique_ptr<FittingResultStats<T>> getLastCalculationStats();
};

std::string gradientDescCompletionToString(GradientDescCompletion completion) {
    switch (completion) {
    case GRADIENT_DESC_CONVERGED:
        return "Converged";
    case GRADIENT_DESC_MAX_ITERATIONS:
        return "Max Iterations";
    default:
        return "Unknown";
    }
}

template <typename T>
void GradientDescStats<T>::print() {
    Serial.println("GradientDescStats: ");
    Serial.println("Completion: " +
                   String(gradientDescCompletionToString(completion).c_str()));
    Serial.println("SSE: " + String(sse));
    Serial.println("R^2: " + String(r2));
    Serial.println("RMSE: " + String(rmse));
    Serial.println("Iterations: " + String(iterations));
}

template <typename T, std::size_t params_count>
void GradientDesc<T, params_count>::print() {
    Serial.println("GradientDesc: ");
    for (auto param : params) {
        Serial.println(String(param));
    }
    Serial.println();
}

template <typename T, std::size_t params_count>
T GradientDesc<T, params_count>::calculateOutput(T input) {
    return func(input, params);
}

template <typename T, std::size_t params_count>
void GradientDesc<T, params_count>::serialize(std::ostream &os) {
    for (std::size_t i = 0; i < params_count; i++) {
        os << params[i];
        if (i != params_count - 1) {
            os << ",";
        }
    }
}

template <typename T, std::size_t params_count>
T GradientDescFactory<T, params_count>::cost_func(
    DataSet<T> &data, std::array<T, params_count> params) {
    T cost = data.accumulate([&](DataPoint<T> dp) {
        return std::pow(func(dp.x, params) - dp.y, 2);
    });
    return cost / (2 * data.size());
}

template <typename T, std::size_t params_count>
std::array<T, params_count> add_arr(std::array<T, params_count> a,
                                    std::array<T, params_count> b,
                                    T scale = T()) {
    std::array<T, params_count> result;
    for (std::size_t i = 0; i < params_count; i++) {
        result[i] = a[i] + b[i] * scale;
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
    T h = std::pow(std::numeric_limits<T>::epsilon(), T(1.0 / 3.0));
    for (std::size_t i = 0; i < params_count; i++) {
        deriv_eps[i] = h;
        grad[i] = (cost_func(data, add_arr(params, deriv_eps)) -
                   cost_func(data, add_arr(params, deriv_eps, T(-1.0)))) /
                  (2 * h);

        deriv_eps[i] = T();
    }
    return grad;
}

template <typename T, std::size_t params_count>
std::unique_ptr<FittingResult<T>>
GradientDescFactory<T, params_count>::calculateFitting(DataSet<T> &data) {
    std::array<T, params_count> params;
    auto initial_params = settings.getInitialParams();
    std::copy(initial_params.begin(), initial_params.end(), params.begin());

    T learning_rate = settings.getLearningRate();

    long iterations = 0;
    GradientDescCompletion completion = GRADIENT_DESC_CONVERGED;
    while (true) {
        auto grad = grad_cost_func(data, params);
        for (std::size_t i = 0; i < params_count; i++) {
            params[i] -= learning_rate * grad[i];
        }

        T norm_grad = 0;
        for (std::size_t i = 0; i < params_count; i++) {
            norm_grad += std::pow(grad[i], 2);
        }

        if (norm_grad < settings.getTolerance()) {
            break;
        }
        if (iterations > settings.getMaxIterations()) {
            completion = GRADIENT_DESC_MAX_ITERATIONS;
            break;
        }

        iterations++;
    }
    T cost = cost_func(data, params);

    T rmse = std::sqrt(cost / data.size());
    T sse = cost;
    T r2 = 1 - (sse / (data.size() * std::pow(data.std(), 2)));

    last_stats = GradientDescStats<T>(completion, sse, r2, rmse, iterations);

    return std::make_unique<GradientDesc<T, params_count>>(func, params);
};

template <typename T, std::size_t params_count>
std::unique_ptr<FittingResult<T>>
GradientDescFactory<T, params_count>::deserialize(std::string str) {
    auto values = split(str, ',');
    std::array<T, params_count> params;
    for (std::size_t i = 0; i < params_count; i++) {
        params[i] = std::stod(values[i]);
    }
    return std::make_unique<GradientDesc<T, params_count>>(func, params);
};

template <typename T, std::size_t params_count>
std::unique_ptr<FittingResult<T>>
GradientDescFactory<T, params_count>::getDefault() {
    std::array<T, params_count> params;
    std::fill(params.begin(), params.end(), T());
    return std::make_unique<GradientDesc<T, params_count>>(func, params);
}

template <typename T, std::size_t params_count>
inline std::unique_ptr<FittingResultStats<T>>
GradientDescFactory<T, params_count>::getLastCalculationStats() {
    return std::make_unique<GradientDescStats<T>>(last_stats);
}

#endif
