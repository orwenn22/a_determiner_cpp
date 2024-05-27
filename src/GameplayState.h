#ifndef UI_FRAMEWORK_GAMEPLAYSTATE_H
#define UI_FRAMEWORK_GAMEPLAYSTATE_H

#include "engine/state/State.h"

#include <raylib.h>
#include <vector>


class MetricsCamera;
class ObjectManager;
class Player;
class Terrain;

class GameplayState : public State {
public:
    GameplayState();
    ~GameplayState() override;

    //static inline GameplayState *The() { return singleton; }

    void Update(float dt) override;
    void Draw() override;

    Player *GetCurrentPlayer();
    inline const char *GetIdentifier() override { return "GameplayState"; }
    inline Terrain *GetTerrain() { return m_terrain; }


private:
    void HandleDragCamera(float mouse_x, float mouse_y);        //Take mouse pos in pixel


    //static GameplayState *singleton;

    MetricsCamera *m_camera;
    bool m_cam_follow_mouse;
    Vector2 m_cam_mouse_offset;

    Terrain *m_terrain;
    ObjectManager *m_object_manager;

    std::vector<Player *> m_players;
    int m_current_player;
};


#endif //UI_FRAMEWORK_GAMEPLAYSTATE_H
