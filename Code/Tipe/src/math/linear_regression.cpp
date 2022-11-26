#include "linear_regression.hpp"

template <typename T>
DataSet<T>::DataSet()
{
}

template <typename T>
T DataSet<T>::accumulate(T (*func)(DataPoint<T>))
{
    T result = 0;
    for (auto dataPoint : this->data)
    {
        result += func(dataPoint);
    }
    return result;
}

template <typename T>
void DataSet<T>::appendDataPoint(DataPoint<T> dataPoint)
{
    this->data.push_back(dataPoint);
}

template <typename T>
LinearRegressionResult<T> linearRegression(DataSet<T> ds)
{
    T sx = ds.accumulate([](DataPoint<T> dp)
                         { return dp.x; });
    T sy = ds.accumulate([](DataPoint<T> dp)
                         { return dp.y; });
    T sxx = ds.accumulate([](DataPoint<T> dp)
                          { return dp.x * dp.x; });
    T syy = ds.accumulate([](DataPoint<T> dp)
                          { return dp.y * dp.y; });
    T sxy = ds.accumulate([](DataPoint<T> dp)
                          { return dp.x * dp.y; });

    T slope = (ds.size * sxy - sx * sy) / (ds.size * sxx - sx * sx);
    T intercept = (sy - slope * sx) / ds.size;

    T r = (ds.size * sxy - sx * sy) / sqrt((ds.size * sxx - sx * sx) * (ds.size * syy - sy * sy));

    return {slope, intercept, r};
}