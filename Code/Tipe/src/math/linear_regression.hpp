#pragma once
#include <vector>

template <typename T>
struct DataPoint
{
    T x;
    T y;
};

template <typename T>
class DataSet
{
    std::vector<DataPoint<T>> data;

public:
    DataSet();
    void appendDataPoint(DataPoint<T> dataPoint);
    T accumulate(T (*func)(DataPoint<T>));
};

template <typename T>
struct LinearRegressionResult
{
    T slope;
    T intercept;
    T r;
};

template <typename T>
LinearRegressionResult<T> linearRegression(DataSet<T> ds);