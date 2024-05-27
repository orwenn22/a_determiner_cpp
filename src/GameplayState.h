#ifndef UI_FRAMEWORK_GAMEPLAYSTATE_H
#define UI_FRAMEWORK_GAMEPLAYSTATE_H

#include "engine/state/State.h"

#include <raylib.h>
#include <vector>


class MetricsCamera;
class ObjectManager;
class Player;
class Terrain;
class WidgetManager;

class GameplayState : public State {
public:
    GameplayState();
    ~GameplayState() override;

    //static inline GameplayState *The() { return singleton; }

    void Update(float dt) override;
    void Draw() override;

    void PlacePlayer(Vector2 pos, int team, bool check_start_pos = true);
    void NextPlayerTurn();
    void KillPlayer(Player *p);

    void ShowActionWidgets();
    void HideActionWidgets();

    Player *GetCurrentPlayer();
    inline const char *GetIdentifier() override { return "GameplayState"; }
    inline Terrain *GetTerrain() { return m_terrain; }
    inline bool PlacingPlayers() { return m_current_player == -1; }      //Return true if we are still placing players, false otherwise


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
    int m_players_per_team;

    WidgetManager *m_action_widgets;
    bool m_show_action_widgets;
};


#endif //UI_FRAMEWORK_GAMEPLAYSTATE_H
