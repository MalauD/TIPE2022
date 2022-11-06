#include "linear_regression.hpp"

template <typename T> DataSet<T>::DataSet(DataPoint<T> *data, int size)
{
    this->data = data;
    this->size = size;
}

template <typename T> T DataSet<T>::accumulate(T (*func)(DataPoint<T>))
{
    T result = 0;
    for (int i = 0; i < size; i++)
    {
        result += func(data[i]);
    }
    return result;
}

template <typename T> LinearRegressionResult<T> linearRegression(DataSet<T> ds)
{
    T sx = ds.accumulate([](DataPoint<T> dp) { return dp.x; });
    T sy = ds.accumulate([](DataPoint<T> dp) { return dp.y; });
    T sxx = ds.accumulate([](DataPoint<T> dp) { return dp.x * dp.x; });
    T syy = ds.accumulate([](DataPoint<T> dp) { return dp.y * dp.y; });
    T sxy = ds.accumulate([](DataPoint<T> dp) { return dp.x * dp.y; });

    T slope = (ds.size * sxy - sx * sy) / (ds.size * sxx - sx * sx);
    T intercept = (sy - slope * sx) / ds.size;

    T r = (ds.size * sxy - sx * sy) / sqrt((ds.size * sxx - sx * sx) * (ds.size * syy - sy * sy));

    return {slope, intercept, r};
}