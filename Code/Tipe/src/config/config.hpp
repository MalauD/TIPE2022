#pragma once
#include "../math/linear_regression.hpp"
#include "LittleFS.h"
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

  void print();
  static Config *getDefaultConfig();

  LinearRegressionResult<float> getLinearRegressionResult(size_t index);
  void setLinearRegressionResultAtIndex(LinearRegressionResult<float> result, size_t index);
  void extendDatasetAtIndex(DataSet<float> dataset, size_t index);
  void setDatasetAtIndex(DataSet<float> dataset, size_t index);
  DataSet<float> getDatasetAtIndex(size_t index);
};

class ConfigManager
{
public:
  ConfigManager();
  int begin();
  Config *getConfig();
  void saveConfig(Config &config);
};
