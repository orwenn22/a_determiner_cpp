#ifndef A_DETERMINER_MAPLOADER_H
#define A_DETERMINER_MAPLOADER_H

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


#endif //A_DETERMINER_MAPLOADER_H
