#ifndef MATH_FITTING_FITTING_HPP
#define MATH_FITTING_FITTING_HPP

#include "../data_set.hpp"
#include <iostream>
#include <memory>

template <typename T>
class FittingResult {

  public:
    virtual void print() = 0;
    virtual T calculateOutput(T input) = 0;
    virtual void serialize(std::ostream &os) = 0;
};

template <typename T>
class FittingResultStats {
  public:
    virtual void print() = 0;
};

template <typename T>
class FittingResultFactory {
  public:
    virtual std::unique_ptr<FittingResult<T>> deserialize(std::string str) = 0;
    virtual std::unique_ptr<FittingResult<T>> getDefault() = 0;
    virtual std::unique_ptr<FittingResult<T>>
    calculateFitting(DataSet<T> &data) = 0;
    virtual std::unique_ptr<FittingResultStats<T>>
    getLastCalculationStats() = 0;
};

#endif