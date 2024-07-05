#ifndef A_DETERMINER_EXPLOSION_H
#define A_DETERMINER_EXPLOSION_H

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

    void ApplyPropulsionToOther(KinematicObject *obj, float dt);

    GameplayState *m_gameplay_state;
    float m_radius;
    float m_explosion_power;
    float m_anim_timer;
    float m_anim_duraton;
    bool m_exploded_terrain;
    bool m_exploded_enemies;
};


#endif //A_DETERMINER_EXPLOSION_H
