#ifndef A_DETERMINER_CONFIG_H
#define A_DETERMINER_CONFIG_H

#include <string>

namespace Config {
    extern bool enable_external;

    std::string GetKeybindConfigPath();
    std::string GetExternalFolderPath();

};  //namespace Config

#endif //A_DETERMINER_CONFIG_H
