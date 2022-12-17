#pragma once
#include "../data_set.hpp"
#include <iostream>
#include <memory>

template <typename T>
class FittingResult {

  public:
    virtual void print() = 0;
    virtual T calculateOutput(T input) = 0;
    virtual std::string serialize() = 0;
};

template <typename T>
class FittingResultFactory {
  public:
    virtual std::unique_ptr<FittingResult<T>>
    calculateFitting(DataSet<T> data) = 0;
    virtual std::unique_ptr<FittingResult<T>> deserialize(std::string str) = 0;
    virtual std::unique_ptr<FittingResult<T>> getDefault() = 0;
};