#include "config.hpp"

template <typename T, std::size_t size>
void Config<T, size>::serialize(std::ostream &os)
{
    os << std::to_string(size) << '\n';
    for (size_t i = 0; i < size; i++)
    {
        os << fittingResult[i].serialize() << '\n';
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

template <typename T, std::size_t size>
std::unique_ptr<FittingResult<T>> Config<T, size>::getFittingResultAt(size_t index)
{
    return this->fittingResult[index];
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

template <typename T, std::size_t size>
void Config<T, size>::deserialize(std::istream &is)
{
    std::string line;
    std::getline(is, line);
    for (size_t i = 0; i < size; i++)
    {
        std::getline(is, line);
        fittingResult[i] = FittingResultFactory<T>::deserialize(line);
    }
    for (size_t i = 0; i < size; i++)
    {
        std::getline(is, line);
        auto size_d = std::stoi(line);
        for (size_t j = 0; j < size_d; j++)
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

template <typename T, std::size_t size>
void Config<T, size>::print()
{
    std::stringstream ss;
    serialize(ss);
    Serial.println(ss.str().c_str());
}

template <typename T, std::size_t size>
ConfigManager<T, size>::ConfigManager()
{
}

template <typename T, std::size_t size>
int ConfigManager<T, size>::begin()
{
    // Serial.println("Formatting file system...");
    //  if (!LittleFS.format())
    //{
    //      Serial.println("File system format failed.");
    //      return 1;
    //  }
    if (!LittleFS.begin())
    {
        Serial.println("Failed to start file system");
        return 1;
    }
    Serial.println("File system started.");
    return 0;
}

template <typename T, std::size_t size>
std::unique_ptr<Config<T, size>> ConfigManager<T, size>::getConfig()
{
    auto config = std::make_unique<Config<T, size>>(new Config<T, size>());
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
    Serial.println("File content:");
    Serial.println(ss.str().c_str());
    config->deserialize(ss);
    file.close();
    return config;
}

template <typename T, std::size_t size>
std::unique_ptr<Config<T, size>> Config<T, size>::getDefaultConfig()
{
    auto config = std::make_unique<Config<T, size>>(new Config<T, size>());
    for (size_t i = 0; i < size; i++)
    {
        config->fittingResult[i] = FittingResultFactory<T>::getDefault();
    }
    return config;
}

template <typename T, std::size_t size>
void ConfigManager<T, size>::saveConfig(std::unique_ptr<Config<T, size>> config)
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

template <typename T, std::size_t size>
void Config<T, size>::setFittingResultAt(std::unique_ptr<FittingResult<T>> result, std::size_t index)
{
    this->fittingResult[index] = result;
}

template <typename T, std::size_t size>
void Config<T, size>::setDatasetAt(DataSet<T> dataset, std::size_t index)
{
    this->dataSet[index] = dataset;
}

template <typename T, std::size_t size>
DataSet<T> Config<T, size>::getDatasetAt(std::size_t index)
{
    return this->dataSet[index];
}

template <typename T, std::size_t size>
void Config<T, size>::extendDatasetAt(DataSet<T> dataset, std::size_t index)
{
    this->dataSet[index].extend(dataset);
}

template <typename T, std::size_t size>
void Config<T, size>::convertToWeight(std::array<T, size> readings, std::array<T, size> weight)
{
    for (std::size_t i = 0; i < size; i++)
    {
        weight[i] = fittingResult[i]->calculateOutput(readings[i])
    }
}