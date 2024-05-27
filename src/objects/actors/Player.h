#ifndef UI_FRAMEWORK_PLAYER_H
#define UI_FRAMEWORK_PLAYER_H

#include "engine/object/KinematicObject.h"

#include <vector>

class Action;
class GameplayState;
class Widget;

class Player : public KinematicObject {
public:
    Player(Vector2 position, int team, GameplayState *gameplay_state, float mass = 10.f);
    ~Player() override;

    void Update(float dt) override;
    void Draw() override;

    bool Grounded();
    bool CollideWithSolidObject();

    bool IsPlaying();
    void SkipTurn();

    std::vector<Widget *> GetActionWidgets();

    Rectangle GetRectangle() override;

    inline void SetCurrentAction(int new_action) { m_current_action = new_action; }
    inline int GetCurrentAction() { return m_current_action; }

private:
    void UpdatePhysics(float dt);

    GameplayState *m_gameplay_state;
    int m_team;

    // This is the launch force intensity in newton (not really in reality, but we will pretend it is)
    float m_strength;

    // Throw angle when the player is shooting
    float m_throw_angle;

    // Some actions cost points. The points are stored in this.
    int m_energy;
    int m_current_action;
    std::vector<Action *> m_actions;

    std::vector<int> m_solid_types;

    // This can be set to true by the current action in case we need to draw a custom sprite
    bool m_block_default_sprite;

    // If set to true the hitbox is a bit smaller
    bool m_use_small_hitbox;
};


#endif //UI_FRAMEWORK_PLAYER_H
