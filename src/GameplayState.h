#ifndef UI_FRAMEWORK_GAMEPLAYSTATE_H
#define UI_FRAMEWORK_GAMEPLAYSTATE_H

#include "engine/state/State.h"

#include <raylib.h>
#include <vector>


class EntityObject;
class MetricsCamera;
class Label;
class ObjectManager;
class Player;
class Terrain;
class WidgetManager;
class WindowManager;

class GameplayState : public State {
public:
    GameplayState();
    ~GameplayState() override;

    //static inline GameplayState *The() { return singleton; }

    void Update(float dt) override;
    void Draw() override;

    Player *GetCurrentPlayer();
    int GetPlayerCount();
    Player *GetPlayer(int index);
    void PlacePlayer(Vector2 pos, int team, bool check_start_pos = true);
    void NextPlayerTurn();
    void KillPlayer(Player *p);

    void ShowActionWidgets();
    void HideActionWidgets();
    void ForceUpdateWidgets();

    void SpawnRandomItem();

    inline const char *GetIdentifier() override { return "GameplayState"; }
    inline Terrain *GetTerrain() { return m_terrain; }
    inline MetricsCamera *GetCamera() { return m_camera; }
    inline bool PlacingPlayers() { return m_current_player == -1; }      //Return true if we are still placing players, false otherwise
    inline bool IsShowingActions() { return m_show_action_widgets; }

    void SetSpawnedObject(EntityObject *spawned_object);
    void UpdateSpawnedObject(Vector2 mouse_pos_meter);


private:
    void HandleDragCamera(float mouse_x, float mouse_y);        //Take mouse pos in pixel
    void UpdateHotbarText();

    void CheckForVictory();


    //static GameplayState *singleton;

    MetricsCamera *m_camera;
    bool m_cam_follow_mouse;
    Vector2 m_cam_mouse_offset;

    Terrain *m_terrain;
    ObjectManager *m_object_manager;

    std::vector<Player *> m_players;
    int m_current_player;
    int m_players_per_team;
    int m_team_count;
    std::vector<Rectangle> m_start_regions;

    WidgetManager *m_overlay;
    Label *m_hotbar_text;
    WidgetManager *m_action_widgets;
    bool m_show_action_widgets;

    WindowManager *m_windows;
    EntityObject *m_spawned_object;
    bool m_preview_spawned_object;
};


#endif //UI_FRAMEWORK_GAMEPLAYSTATE_H
