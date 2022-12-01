#pragma once
#include "../math/linear_regression.hpp"
#include "LittleFs.h"
#include <ostream>
#include <sstream>
#include <string>

class Config
{
    LinearRegressionResult<float> *linearRegressionResult;
    DataSet<float> *dataSet;
    size_t size;

  public:
    void serialize(std::ostream &os);
    void deserialize(std::istream &is);
};

class ConfigManager
{
  public:
    ConfigManager();
    int begin();
    Config *getConfig();
    void saveConfig(Config &config);
};
