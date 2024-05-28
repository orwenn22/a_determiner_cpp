#include "Bullet.h"

#include "engine/metrics/Graphics.h"
#include "engine/object/ObjectManager.h"
#include "Explosion.h"
#include "GameplayState.h"
#include "Player.h"
#include "Terrain.h"
#include "../ObjectIDs.h"


Bullet::Bullet(Vector2 pos, GameplayState *gameplay_state, KinematicObject *thrower, bool enable_gravity) : KinematicObject(pos, .25f, .25f, 5.f) {
    m_enable_physics = true;
    m_enable_gravity = enable_gravity;
    m_gameplay_state = gameplay_state;
    m_power = 2.f;
    m_thrower = thrower;
}

void Bullet::Update(float dt) {
    bool need_explosion = false;
    ProcessPhysics(dt);

    if(m_gameplay_state->GetTerrain()->CheckCollisionRec(GetRectangle())) {
        need_explosion = true;
    }

    auto wall_collisions = Manager()->GetCollisions(this, TypeID_Wall);
    for(EntityObject *wall : wall_collisions) {
        need_explosion = true;
        Manager()->DestroyObject(wall);
    }

    auto player_collisions = Manager()->GetCollisions(this, TypeID_Player);
    for(EntityObject *player : player_collisions) {
        if(player == m_thrower) continue;
        m_gameplay_state->KillPlayer((Player *)player);
        need_explosion = true;
    }

    if(need_explosion) {
        Manager()->AddObject(new Explosion(m_position, m_power, m_gameplay_state));
        Manager()->DestroyObject(this);
    }
}

void Bullet::Draw() {
    Metrics::DrawCircle(m_position, m_width/2.f, WHITE);
}
