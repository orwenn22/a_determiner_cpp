#ifndef UI_FRAMEWORK_MAPLOADER_H
#define UI_FRAMEWORK_MAPLOADER_H

#include "engine/util/ErrorOr.h"
#include <string>

class GameplayState;

class MapLoader {
public:
    MapLoader();
    virtual ~MapLoader();

    virtual bool IsCorrectFormat(std::string path) = 0;
    virtual ErrorOr<GameplayState *> LoadMap(std::string path) = 0;
};


#endif //UI_FRAMEWORK_MAPLOADER_H
