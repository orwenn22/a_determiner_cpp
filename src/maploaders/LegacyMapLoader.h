#ifndef A_DETERMINER_LEGACYMAPLOADER_H
#define A_DETERMINER_LEGACYMAPLOADER_H

#include "MapLoader.h"

class LegacyMapLoader : public MapLoader {
public:
    LegacyMapLoader();
    ~LegacyMapLoader() override;

    bool IsCorrectFormat(std::string path) override;
    ErrorOr<GameplayState *> LoadMap(std::string path) override;
};


#endif //A_DETERMINER_LEGACYMAPLOADER_H
