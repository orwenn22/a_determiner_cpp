#ifndef UI_FRAMEWORK_EXPLOSION_H
#define UI_FRAMEWORK_EXPLOSION_H

#include "engine/object/EntityObject.h"

class GameplayState;
class KinematicObject;

class Explosion : public EntityObject {
public:
    Explosion(Vector2 pos, float radius, GameplayState *gameplay_state);

    void Update(float dt) override;
    void Draw() override;


private:
    void ExplodeNearbyEnemies(float dt);
    void ExplodeTerrain();
    int GetCurrentAnimationFrame();

    void ApplyForceToOther(KinematicObject *obj, float dt);

    GameplayState *m_gameplay_state;
    float m_radius;
    float m_explosion_power;
    float m_anim_timer;
    float m_anim_duraton;
    bool m_exploded_terrain;
    bool m_exploded_enemies;
};


#endif //UI_FRAMEWORK_EXPLOSION_H
