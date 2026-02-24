#include "Config.h"

AppConfig Config::load(const std::string& path) {
    // TODO Session 7: parse JSON using nlohmann/json
    return AppConfig{};
}

void Config::save(const AppConfig& config,
                  const std::string& path) {
    // TODO Session 7: serialize AppConfig to JSON file
}
