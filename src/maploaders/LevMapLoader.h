#ifndef A_DETERMINER_LEVMAPLOADER_H
#define A_DETERMINER_LEVMAPLOADER_H

#include "MapLoader.h"

class EditorLevel;

class LevMapLoader : public MapLoader {
public:
    LevMapLoader();
    ~LevMapLoader() override;

    bool IsCorrectFormat(std::string path) override;
    ErrorOr<GameplayState *> LoadMap(std::string path) override;
    ErrorOr<GameplayState *> LoadMap(EditorLevel *editor_level);
};


#endif //A_DETERMINER_LEVMAPLOADER_H
