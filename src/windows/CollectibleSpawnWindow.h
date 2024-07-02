#ifndef A_DETERMINER_COLLECTIBLESPAWNWINDOW_H
#define A_DETERMINER_COLLECTIBLESPAWNWINDOW_H

#include "engine/windows/Window.h"

class GameplayState;

class CollectibleSpawnWindow : public Window {
public:
    CollectibleSpawnWindow(int x, int y, GameplayState *gameplay_state);
    ~CollectibleSpawnWindow() override;

private:
    GameplayState *m_gameplay_state;
};


#endif //A_DETERMINER_COLLECTIBLESPAWNWINDOW_H
