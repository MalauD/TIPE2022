#ifndef MATH_DATA_SET_HPP
#define MATH_DATA_SET_HPP

#include <vector>

template <typename T>
class DataPoint {
  public:
    DataPoint(T x, T y) : x(x), y(y) {}
    T x;
    T y;
};

template <typename T>
class DataSet {
    std::vector<DataPoint<T>> data;

  public:
    DataSet() = default;
    std::size_t size();
    void appendDataPoint(DataPoint<T> dataPoint);
    void extend(DataSet<T> dataSet);
    void clear();
    DataPoint<T> at(std::size_t index);
    T accumulate(T (*func)(DataPoint<T>));
    DataSet<T> map(DataPoint<T> (*func)(DataPoint<T>));
};

template <typename T>
std::size_t DataSet<T>::size() {
    return data.size();
}

template <typename T>
T DataSet<T>::accumulate(T (*func)(DataPoint<T>)) {
    T result = 0;
    for (auto dataPoint : data) {
        result += func(dataPoint);
    }
    return result;
}

template <typename T>
DataSet<T> DataSet<T>::map(DataPoint<T> (*func)(DataPoint<T>)) {
    DataSet<T> result;
    for (auto dataPoint : data) {
        result.appendDataPoint(func(dataPoint));
    }
    return result;
}

template <typename T>
void DataSet<T>::appendDataPoint(DataPoint<T> dataPoint) {
    data.push_back(dataPoint);
}

template <typename T>
void DataSet<T>::extend(DataSet<T> dataSet) {
    data.insert(data.end(), dataSet.data.begin(), dataSet.data.end());
}

template <typename T>
void DataSet<T>::clear() {
    data.clear();
}

template <typename T>
DataPoint<T> DataSet<T>::at(std::size_t index) {
    return data[index];
}

#endif