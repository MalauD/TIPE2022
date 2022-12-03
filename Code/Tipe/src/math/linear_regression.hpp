#pragma once
#include <math.h>
#include <vector>

template <typename T> struct DataPoint
{
    T x;
    T y;
};

template <typename T> class DataSet
{
    std::vector<DataPoint<T>> data;

  public:
    DataSet();
    size_t size();
    void appendDataPoint(DataPoint<T> dataPoint);
    DataPoint<T> at(size_t index);
    T accumulate(T (*func)(DataPoint<T>));
};

template <typename T> struct LinearRegressionResult
{
    T slope;
    T intercept;
    T r;
};

template <typename T> LinearRegressionResult<T> linearRegression(DataSet<T> ds);

//
// Implementation
//

template <typename T> DataSet<T>::DataSet()
{
}

template <typename T> size_t DataSet<T>::size()
{
    return data.size();
}

template <typename T> T DataSet<T>::accumulate(T (*func)(DataPoint<T>))
{
    T result = 0;
    for (auto dataPoint : data)
    {
        result += func(dataPoint);
    }
    return result;
}

template <typename T> void DataSet<T>::appendDataPoint(DataPoint<T> dataPoint)
{
    data.push_back(dataPoint);
}

template <typename T> DataPoint<T> DataSet<T>::at(size_t index)
{
    return data[index];
}

template <typename T> LinearRegressionResult<T> linearRegression(DataSet<T> ds)
{
    T sx = ds.accumulate([](DataPoint<T> dp) { return dp.x; });
    T sy = ds.accumulate([](DataPoint<T> dp) { return dp.y; });
    T sxx = ds.accumulate([](DataPoint<T> dp) { return dp.x * dp.x; });
    T syy = ds.accumulate([](DataPoint<T> dp) { return dp.y * dp.y; });
    T sxy = ds.accumulate([](DataPoint<T> dp) { return dp.x * dp.y; });

    auto dataSetSize = ds.size();

    T slope = (dataSetSize * sxy - sx * sy) / (dataSetSize * sxx - sx * sx);
    T intercept = (sy - slope * sx) / dataSetSize;

    T r = (dataSetSize * sxy - sx * sy) / sqrt((dataSetSize * sxx - sx * sx) * (dataSetSize * syy - sy * sy));

    return {slope, intercept, r};
}