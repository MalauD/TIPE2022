#include "config.hpp"

void Config::serialize(std::ostream &os)
{
    os << std::to_string(size) << '\n';
    for (size_t i = 0; i < size; i++)
    {
        os << std::to_string(linearRegressionResult[i].slope) << ','
           << std::to_string(linearRegressionResult[i].intercept) << ',' << std::to_string(linearRegressionResult[i].r)
           << '\n';
    }
    for (size_t i = 0; i < size; i++)
    {
        os << std::to_string(dataSet[i].size()) << '\n';
        for (size_t j = 0; j < dataSet[i].size(); j++)
        {
            auto point = dataSet[i].at(j);
            os << std::to_string(point.x) << ',' << std::to_string(point.y) << '\n';
        }
    }
}

LinearRegressionResult<float> Config::getLinearRegressionResult(size_t index)
{
    return this->linearRegressionResult[index];
}

std::vector<std::string> split(std::string str, char delimiter)
{
    std::vector<std::string> result;
    std::string token;
    std::stringstream ss(str);
    while (std::getline(ss, token, delimiter))
    {
        result.push_back(token);
    }
    return result;
}

void Config::deserialize(std::istream &is)
{
    std::string line;
    std::getline(is, line);
    size = std::stoi(line);
    linearRegressionResult = new LinearRegressionResult<float>[size];
    dataSet = new DataSet<float>[size];
    for (size_t i = 0; i < size; i++)
    {
        std::getline(is, line);
        auto values = split(line, ',');
        linearRegressionResult[i].slope = std::stof(values[0]);
        linearRegressionResult[i].intercept = std::stof(values[1]);
        linearRegressionResult[i].r = std::stof(values[2]);
    }
    for (size_t i = 0; i < size; i++)
    {
        std::getline(is, line);
        auto size = std::stoi(line);
        for (size_t j = 0; j < size; j++)
        {
            std::getline(is, line);
            auto values = split(line, ',');
            DataPoint<float> point;
            point.x = std::stof(values[0]);
            point.y = std::stof(values[1]);
            dataSet[i].appendDataPoint(point);
        }
    }
}

ConfigManager::ConfigManager()
{
}

int ConfigManager::begin()
{
    Serial.println("Formatting file system...");
    if (!LittleFS.format())
    {
        Serial.println("File system format failed.");
        return 1;
    }
    if (!LittleFS.begin())
    {
        Serial.println("Failed to start file system");
        return 1;
    }
    Serial.println("File system formatted.");
    return 0;
}

Config *ConfigManager::getConfig()
{
    Config *config = new Config();
    File file = LittleFS.open("/config.txt", "r");
    if (!file)
    {
        Serial.println("Failed to open file for reading");
        return config;
    }
    std::stringstream ss;
    while (file.available())
    {
        ss << (char)file.read();
    }
    config->deserialize(ss);
    file.close();
    return config;
}

void ConfigManager::saveConfig(Config &config)
{
    File file = LittleFS.open("/config.txt", "w");
    if (!file)
    {
        Serial.println("Failed to open file for writing");
        return;
    }
    std::stringstream ss;
    config.serialize(ss);
    file.print(ss.str().c_str());
    file.close();
}