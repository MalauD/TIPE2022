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
    T accumulate(std::function<T(DataPoint<T> &)> func);
    DataSet<T> map(std::function<DataPoint<T>(DataPoint<T> &)> func);
    DataSet<T> normalize();
    T std();
    T mean();
    void print();
};

template <typename T>
std::size_t DataSet<T>::size() {
    return data.size();
}

template <typename T>
T DataSet<T>::accumulate(std::function<T(DataPoint<T> &)> func) {
    T result = 0;
    for (auto dataPoint : data) {
        result += func(dataPoint);
    }
    return result;
}

template <typename T>
DataSet<T> DataSet<T>::map(std::function<DataPoint<T>(DataPoint<T> &)> func) {
    DataSet<T> result;
    for (auto dataPoint : data) {
        result.appendDataPoint(func(dataPoint));
    }
    return result;
}

template <typename T>
DataSet<T> DataSet<T>::normalize() {
    DataSet<T> result;
    T mean = this->mean();
    T std = this->std();
    for (auto dataPoint : data) {
        result.appendDataPoint(
            DataPoint<T>(dataPoint.x, (dataPoint.y - mean) / std));
    }
    return result;
}

template <typename T>
T DataSet<T>::std() {
    T mean = this->mean();
    T sum = accumulate([&mean](DataPoint<T> dataPoint) {
        return std::pow(dataPoint.y - mean, 2);
    });
    return std::sqrt(sum / size());
}

template <typename T>
T DataSet<T>::mean() {
    return accumulate([](DataPoint<T> dataPoint) { return dataPoint.y; }) /
           size();
}

template <typename T>
void DataSet<T>::print() {
    for (auto dataPoint : data) {
        Serial.print(dataPoint.x);
        Serial.print(" ");
        Serial.println(dataPoint.y);
    }
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