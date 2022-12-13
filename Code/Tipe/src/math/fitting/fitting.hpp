#pragma once
#include "../data_set.hpp"
#include <memory>
#include <iostream>

template <typename T>
class FittingResult
{

public:
    virtual void print() = 0;
    virtual T calculateOutput(T input) = 0;
    virtual std::string serialize() = 0;
};

template <typename T>
class FittingResultFactory
{
    virtual std::unique_ptr<FittingResult<T>> calculateFitting(DataSet<T> data) = 0;
    virtual std::unique_ptr<FittingResult<T>> deserialize(std::string str) = 0;
};