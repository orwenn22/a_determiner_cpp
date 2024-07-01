#ifndef UI_FRAMEWORK_LEVMAPLOADER_H
#define UI_FRAMEWORK_LEVMAPLOADER_H

#include "MapLoader.h"

class LevMapLoader : public MapLoader {
public:
    LevMapLoader();
    ~LevMapLoader() override;

    bool IsCorrectFormat(std::string path) override;
    ErrorOr<GameplayState *> LoadMap(std::string path) override;
};


#endif //UI_FRAMEWORK_LEVMAPLOADER_H
