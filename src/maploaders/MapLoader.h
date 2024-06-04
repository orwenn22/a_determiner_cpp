#ifndef UI_FRAMEWORK_MAPLOADER_H
#define UI_FRAMEWORK_MAPLOADER_H

#include <string>

class GameplayState;

class MapLoader {
public:
    MapLoader();
    virtual ~MapLoader();

    virtual bool IsCorrectFormat(std::string path) = 0;
    virtual GameplayState *LoadMap(std::string path) = 0;
};


#endif //UI_FRAMEWORK_MAPLOADER_H
