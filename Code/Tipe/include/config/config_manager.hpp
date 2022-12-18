#ifndef CONFIG_CONFIG_MANAGER_HPP
#define CONFIG_CONFIG_MANAGER_HPP

#include "./config.hpp"

template <typename T, std::size_t config_size>
class ConfigManager {
  public:
    ConfigManager() = default;
    int begin();
    void retreiveConfig(Config<T, config_size> &config);
    void saveConfig(Config<T, config_size> &config);
};

template <typename T, std::size_t size>
int ConfigManager<T, size>::begin() {
    if (!LittleFS.begin()) {
        Serial.println("Failed to start file system");
        return 1;
    }
    Serial.println("File system started.");
    return 0;
}

template <typename T, std::size_t size>
void ConfigManager<T, size>::retreiveConfig(Config<T, size> &config) {
    File file = LittleFS.open("/config.txt", "r");
    if (!file) {
        Serial.println("Failed to open file for reading");
        return;
    }
    std::stringstream ss;
    while (file.available()) {
        ss << (char)file.read();
    }
    Serial.println("File content:");
    Serial.println(ss.str().c_str());
    config.deserialize(ss);
    file.close();
}

template <typename T, std::size_t size>
void ConfigManager<T, size>::saveConfig(Config<T, size> &config) {
    File file = LittleFS.open("/config.txt", "w");
    if (!file) {
        Serial.println("Failed to open file for writing");
        return;
    }
    std::stringstream ss;
    config.serialize(ss);
    file.print(ss.str().c_str());
    file.close();
}
#endif