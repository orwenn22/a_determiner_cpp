#include "Config.h"

#include "engine/util/CWD.h"

namespace Config {

std::string GetKeybindConfigPath() {
#if defined(__EMSCRIPTEN__)
    return "/idbfs/.keybinds";      //FIXME : not working
#else
    return GetCWD() + "/.keybinds";
#endif
}

};
