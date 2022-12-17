#pragma once
#include "../io/adc_mux.hpp"
#include "../math/fitting/fitting.hpp"
#include "LittleFS.h"
#include <array>
#include <ostream>
#include <sstream>
#include <string>

template <typename T, std::size_t size> class Config {
    std::array<std::unique_ptr<FittingResult<T>>, size> fittingResult;
    std::array<DataSet<T>, size> dataSet;

  public:
    void serialize(std::ostream &os);
    void deserialize(std::istream &is);

    void print();
    static Config<T, size> getDefaultConfig();

    std::unique_ptr<FittingResult<T>> getFittingResultAt(std::size_t index);
    void setFittingResultAt(std::unique_ptr<FittingResult<T>> result,
                            std::size_t index);
    void extendDatasetAt(DataSet<T> dataset, std::size_t index);
    void setDatasetAt(DataSet<T> dataset, std::size_t index);
    DataSet<T> getDatasetAt(std::size_t index);
    void convertToWeight(std::array<T, size> readings,
                         std::array<T, size> weight);
};

template <typename T, std::size_t config_size> class ConfigManager {
  public:
    ConfigManager();
    int begin();
    Config<T, config_size> getConfig();
    void saveConfig(Config<T, config_size> &config);
};
