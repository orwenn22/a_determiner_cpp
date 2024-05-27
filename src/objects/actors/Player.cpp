#include "Player.h"

#include "engine/metrics/Graphics.h"
#include "engine/object/ObjectManager.h"
#include "engine/util/Math.h"
#include "engine/util/VectorOps.h"
#include "GameplayState.h"
#include "Terrain.h"


Player::Player(Vector2 position, int team, GameplayState *gameplay_state, float mass) : KinematicObject(position, 1.f, 1.f, mass) {
    m_team = team;
    m_gameplay_state = gameplay_state;

    m_strength = 100.f;
    m_throw_angle = 0.f;

    m_energy = 30;
    //m_down_vector = {-1.f, 0.f};

    m_block_default_sprite = false;
    m_use_small_hitbox = false;
}

void Player::Update(float dt) {
    if(!Grounded()) m_enable_physics = true;

    //TODO : update selected action

    if(m_enable_physics) UpdatePhysics(dt);

}

void Player::Draw() {
    //Metrics::DrawRectangle(GetRectangle(), RED);
    KinematicObject::DrawHitbox();
}

/**
 * This is not reliable for when we need extremely accurate collisions.
 * It is meant to be used to detect if the floor below the player was updated while its physics is disabled,
 * so we can re-enable it.
 * @return true if the player is grounded, false otherwise
 */
bool Player::Grounded() {
    float old_x = m_position.x;
    float old_y = m_position.y;

    //m_position = m_position + m_down_vector * 0.02f;
    m_position.x += m_down_vector.x * 0.02f;
    m_position.y += m_down_vector.y * 0.02f;

    bool r = m_gameplay_state->GetTerrain()->CheckCollisionRec(GetRectangle(), true);       // || CollideWithSolidObject();

    m_position = {old_x, old_y};
    return r;
}

bool Player::CollideWithSolidObject() {
    for(int solid_type : m_solid_types) {
        auto col = Manager()->GetCollisions(this, solid_type);
        if(!col.empty()) return true;
    }
    return false;
}

bool Player::IsPlaying() {
    if(m_gameplay_state == nullptr) {
        return false;   // Maybe returning true is better for testing ?
    }
    return m_gameplay_state->GetCurrentPlayer() == this;
}

Rectangle Player::GetRectangle() {
    if(!m_use_small_hitbox) return EntityObject::GetRectangle();

    float old_w = m_width;
    float old_h = m_height;
    m_width *= 0.8f;
    m_height *= 0.8f;
    Rectangle r = EntityObject::GetRectangle();
    m_width = old_w;
    m_height = old_h;
    return r;
}


//////////////////////////////////
//// PRIVATE

void Player::UpdatePhysics(float dt) {
    Terrain *t = m_gameplay_state->GetTerrain();
    if(t == nullptr) return;

    bool collided = false;
    bool going_down = (sign<float, int>(m_velocity.x) == sign<float, int>(m_down_vector.x)
            && sign<float, int>(m_velocity.y) == sign<float, int>(m_down_vector.y));

    // Horizontal
    ProcessPhysicsX(dt);
    if(t->CheckCollisionRec(GetRectangle(), true) || CollideWithSolidObject()) {    // && m_velocity.x != 0.f ?
        m_use_small_hitbox = false;
        collided = true;
        while(t->CheckCollisionRec(GetRectangle(), true) || CollideWithSolidObject()) {
            m_position.x -= 0.01f * sign<float,float>(m_velocity.x);
        }
        m_velocity.x = 0;
    }

    // Vertical
    ProcessPhysicsY(dt);
    if(t->CheckCollisionRec(GetRectangle(), true) || CollideWithSolidObject()) {    // && m_velocity.y != 0.f ?
        m_use_small_hitbox = false;
        collided = true;
        while(t->CheckCollisionRec(GetRectangle(), true) || CollideWithSolidObject()) {
            m_position.y -= 0.01f * sign<float,float>(m_velocity.y);
        }
        m_velocity.y = 0;
    }

    if(collided && going_down) {
        m_velocity.x = 0;
        m_velocity.y = 0;
        m_enable_physics = false;
        if(IsPlaying()) {
            //ShowActionWidgets();
        }
    }
}
