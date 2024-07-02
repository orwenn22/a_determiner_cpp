#ifndef A_DETERMINER_BULLET_H
#define A_DETERMINER_BULLET_H

#include "engine/object/KinematicObject.h"

class GameplayState;

class Bullet : public KinematicObject {
public:
    Bullet(Vector2 pos, GameplayState *gameplay_state, KinematicObject *thrower, bool enable_gravity = true);

    void Update(float dt) override;
    void Draw() override;


private:
    GameplayState *m_gameplay_state;
    KinematicObject *m_thrower;
    float m_power;
};


#endif //A_DETERMINER_BULLET_H
