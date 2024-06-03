#ifndef UI_FRAMEWORK_COLLECTIBLESPAWNWINDOW_H
#define UI_FRAMEWORK_COLLECTIBLESPAWNWINDOW_H

#include "engine/windows/Window.h"

class GameplayState;

class CollectibleSpawnWindow : public Window {
public:
    CollectibleSpawnWindow(int x, int y, GameplayState *gameplay_state);
    ~CollectibleSpawnWindow() override;

private:
    GameplayState *m_gameplay_state;
};


#endif //UI_FRAMEWORK_COLLECTIBLESPAWNWINDOW_H
