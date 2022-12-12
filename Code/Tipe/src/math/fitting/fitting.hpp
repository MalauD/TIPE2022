#pragma once
#include "../data_set.hpp"
#include <iostream>

template <typename T>
class FittingResult
{

public:
    virtual void print() = 0;
    static virtual FittingResult<T> calculateFitting(DataSet<T> data) = 0;
    virtual T calculateOutput(T input) = 0;
    virtual friend std::ostream &operator<<(std::ostream &stream, const FittingResult &fitting) = 0;
};