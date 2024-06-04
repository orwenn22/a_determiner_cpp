#ifndef UI_FRAMEWORK_LEGACYMAPLOADER_H
#define UI_FRAMEWORK_LEGACYMAPLOADER_H

#include "MapLoader.h"

class LegacyMapLoader : public MapLoader {
public:
    LegacyMapLoader();
    ~LegacyMapLoader() override;

    bool IsCorrectFormat(std::string path) override;
    GameplayState *LoadMap(std::string path) override;
};


#endif //UI_FRAMEWORK_LEGACYMAPLOADER_H
