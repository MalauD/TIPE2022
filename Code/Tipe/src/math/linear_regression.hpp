#pragma once

template <typename T> struct DataPoint
{
    T x;
    T y;
};

template <typename T> class DataSet
{
    DataPoint<T> *data;
    int size;

  public:
    DataSet(DataPoint<T> *data, int size);
    T accumulate(T (*func)(DataPoint<T>));
};

template <typename T> struct LinearRegressionResult
{
    T slope;
    T intercept;
    T r;
};

template <typename T> LinearRegressionResult<T> linearRegression(DataSet<T> ds);