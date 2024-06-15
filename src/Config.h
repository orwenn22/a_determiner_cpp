#ifndef UI_FRAMEWORK_CONFIG_H
#define UI_FRAMEWORK_CONFIG_H

#include <string>

namespace Config {
    extern bool enable_external;

    std::string GetKeybindConfigPath();
    std::string GetExternalFolderPath();

};  //namespace Config

#endif //UI_FRAMEWORK_CONFIG_H
