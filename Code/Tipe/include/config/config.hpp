#ifndef CONFIG_CONFIG_HPP
#define CONFIG_CONFIG_HPP

#include "../io/adc_mux.hpp"
#include "../math/fitting/fitting.hpp"
#include "LittleFS.h"
#include <array>
#include <ostream>
#include <sstream>
#include <string>

template <typename T, std::size_t size>
class Config {
    std::array<std::unique_ptr<FittingResult<T>>, size> fittingResult;
    std::array<DataSet<T>, size> dataSet;
    std::unique_ptr<FittingResultFactory<T>> fittingResultFactory;

  public:
    Config() = delete;
    Config(std::unique_ptr<FittingResultFactory<T>> fittingResultFactory);
    void serialize(std::ostream &os);
    void deserialize(std::istream &is);

    void print();
    void setToDefault();

    std::unique_ptr<FittingResult<T>> getFittingResultAt(std::size_t index);
    void calculateFittingResultAt(std::size_t index);
    std::unique_ptr<FittingResultStats<T>>
    getFittingResultStatsAt(std::size_t index);
    void printFittingResultAt(std::size_t index);
    void extendDatasetAt(DataSet<T> dataset, std::size_t index);
    void setDatasetAt(DataSet<T> dataset, std::size_t index);
    DataSet<T> getDatasetAt(std::size_t index);
    void convertToWeight(std::array<T, size> readings,
                         std::array<T, size> &weight);
};

template <typename T, std::size_t size>
Config<T, size>::Config(
    std::unique_ptr<FittingResultFactory<T>> fittingResultFactory)
    : fittingResultFactory(std::move(fittingResultFactory)) {
    setToDefault();
}

template <typename T, std::size_t size>
void Config<T, size>::serialize(std::ostream &os) {
    os << std::to_string(size) << '\n';
    for (size_t i = 0; i < size; i++) {
        fittingResult[i]->serialize(os);
        os << '\n';
    }
    for (size_t i = 0; i < size; i++) {
        os << std::to_string(dataSet[i].size()) << '\n';
        for (size_t j = 0; j < dataSet[i].size(); j++) {
            auto point = dataSet[i].at(j);
            os << std::to_string(point.x) << ',' << std::to_string(point.y)
               << '\n';
        }
    }
}

template <typename T, std::size_t size>
std::unique_ptr<FittingResult<T>>
Config<T, size>::getFittingResultAt(size_t index) {
    return this->fittingResult[index];
}

std::vector<std::string> split(std::string str, char delimiter) {
    std::vector<std::string> result;
    std::string token;
    std::stringstream ss(str);
    // NOLINTNEXTLINE(bugprone-infinite-loop)
    while (std::getline(ss, token, delimiter)) {
        result.push_back(token);
    }
    return result;
}

template <typename T, std::size_t size>
void Config<T, size>::deserialize(std::istream &is) {
    std::string line;
    std::getline(is, line);
    for (size_t i = 0; i < size; i++) {
        std::getline(is, line);
        fittingResult[i].reset(
            fittingResultFactory->deserialize(line).release());
    }
    for (size_t i = 0; i < size; i++) {
        std::getline(is, line);
        auto size_d = std::stoi(line);
        for (size_t j = 0; j < size_d; j++) {
            std::getline(is, line);
            auto values = split(line, ',');
            auto point =
                DataPoint<T>{std::stof(values[0]), std::stof(values[1])};
            dataSet[i].appendDataPoint(point);
        }
    }
}

template <typename T, std::size_t size>
void Config<T, size>::print() {
    std::stringstream ss;
    serialize(ss);
    Serial.println(ss.str().c_str());
}

template <typename T, std::size_t size>
void Config<T, size>::setToDefault() {
    for (size_t i = 0; i < size; i++) {
        fittingResult[i].reset(fittingResultFactory->getDefault().release());
        dataSet[i].clear();
    }
}

template <typename T, std::size_t size>
void Config<T, size>::calculateFittingResultAt(std::size_t index) {
    fittingResult[index].reset(
        fittingResultFactory->calculateFitting(dataSet[index]).release());
}

template <typename T, std::size_t size>
std::unique_ptr<FittingResultStats<T>>
Config<T, size>::getFittingResultStatsAt(std::size_t index) {
    return this->fittingResultFactory->getLastCalculationStats();
}

template <typename T, std::size_t size>
void Config<T, size>::printFittingResultAt(std::size_t index) {
    this->fittingResult[index]->print();
}

template <typename T, std::size_t size>
void Config<T, size>::setDatasetAt(DataSet<T> dataset, std::size_t index) {
    this->dataSet[index] = dataset;
}

template <typename T, std::size_t size>
DataSet<T> Config<T, size>::getDatasetAt(std::size_t index) {
    return this->dataSet[index];
}

template <typename T, std::size_t size>
void Config<T, size>::extendDatasetAt(DataSet<T> dataset, std::size_t index) {
    this->dataSet[index].extend(dataset);
}

template <typename T, std::size_t size>
void Config<T, size>::convertToWeight(std::array<T, size> readings,
                                      std::array<T, size> &weight) {
    for (std::size_t i = 0; i < size; i++) {
        weight[i] = fittingResult[i]->calculateOutput(readings[i]);
    }
}

#endif