#include "Player.h"

#include "actions/Action.h"
#include "actions/JumpAction.h"
#include "engine/metrics/Graphics.h"
#include "engine/object/ObjectManager.h"
#include "engine/util/Math.h"
#include "engine/util/VectorOps.h"
#include "engine/widgets/Button.h"
#include "GameplayState.h"
#include "GlobalResources.h"
#include "Teams.h"
#include "Terrain.h"
#include "../ObjectIDs.h"

Player::Player(Vector2 position, int team, GameplayState *gameplay_state, float mass) : KinematicObject(position, 1.f, 1.f, mass) {
    RegisterType(TypeID_Player);

    m_team = team;
    m_gameplay_state = gameplay_state;

    m_strength = 100.f;
    m_throw_angle = 0.f;

    m_energy = 30;
    m_current_action = -1;
    m_actions.push_back(new JumpAction);        //TODO : put actual actions

    m_block_default_sprite = false;
    m_use_small_hitbox = false;

    m_solid_types.push_back(TypeID_Wall);
}

Player::~Player() {
    for(Action *a : m_actions) delete a;
    m_actions.clear();
}

void Player::Update(float dt) {
    if(!Grounded()) m_enable_physics = true;

    if(m_current_action >= 0 && m_current_action < m_actions.size()) m_actions[m_current_action]->OnUpdate(this, dt);

    if(m_enable_physics) UpdatePhysics(dt);

}

void Player::Draw() {
    m_block_default_sprite = false;
    if(m_current_action >= 0 && m_current_action < m_actions.size()) m_actions[m_current_action]->OnDraw(this);

    if(!m_block_default_sprite) {
        if(m_enable_physics) {
            float flip_factor = (m_velocity.x < 0) ? (-1.f) : 1.f;
            Metrics::DrawSpriteRotEx(Res::player_in_jump_sprite, {0.f, 37.f, 32.f*flip_factor, 37.f},
                                     {m_position.x, m_position.y+0.15625f/2.f}, {1.f, 1.15625f},       //37/32 * 1 = 1.15625
                                     0.f, WHITE);
            Metrics::DrawSpriteRotEx(Res::player_in_jump_sprite, {32.f, 37.f, 32.f*flip_factor, 37.f},
                                     {m_position.x, m_position.y+0.15625f/2.f}, {1.f, 1.15625f},       //37/32 * 1 = 1.15625
                                     0.f, s_team_colors[m_team]);
        }
        else {
            Metrics::DrawSpriteRotEx(Res::player_sprite, {0.f, 0.f, 32.f, 32.f},
                                     m_position, {1.f, 1.f},
                                     0.f, WHITE);
            Metrics::DrawSpriteRotEx(Res::player_sprite, {0.f, 32.f, 32.f, 32.f},
                                     m_position, {1.f, 1.f},
                                     0.f, s_team_colors[m_team]);
        }
    }

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

void Player::SkipTurn() {
    // Actions are able to cancel passing the turn to the next player
    bool cancel_next_player_turn = false;
    for(Action *a : m_actions) {
        if(a->OnSkip(this)) cancel_next_player_turn = true;
    }

    m_energy += 10;         // Increase points
    m_current_action = -1;  // Cancel any action

    if(!cancel_next_player_turn) {
        m_gameplay_state->NextPlayerTurn(); //Give the turn to the next character (will clear action widgets)
    }
}

std::vector<Widget *> Player::GetActionWidgets() {
    std::vector<Widget *> r;

    for(int i = 0; i < m_actions.size(); ++i) {
        Action *a = m_actions[i];
        Button *b = new Button(0, 0, 70, 70, a->GetName(), [=]() {
            a->OnClick(this, i);
        });
        r.push_back(b);
    }

    Button *skip_button = new Button(0, 0, 70, 70, "Skip", [=]() {
        SkipTurn();
    });
    r.push_back(skip_button);
    return r;
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


void Player::SetThrowAngle(float a) {
    m_throw_angle = a;

    //TODO : do something with modulo instead of this ?
    while(m_throw_angle > PI) {
        m_throw_angle -= PI*2;
    }
    while(m_throw_angle < -PI) {
        m_throw_angle += PI*2;
    }
}

//////////////////////////////////
//// PRIVATE

void Player::UpdatePhysics(float dt) {
    Terrain *t = m_gameplay_state->GetTerrain();
    if(t == nullptr) return;

    bool collided = false;
    bool going_down = (sign<float, int>(m_velocity.x) == sign<float, int>(m_down_vector.x)
            || sign<float, int>(m_velocity.y) == sign<float, int>(m_down_vector.y));

    // Horizontal
    ProcessPhysicsX(dt);
    if(t->CheckCollisionRec(GetRectangle(), true) || CollideWithSolidObject()) {    // && m_velocity.x != 0.f ?
        m_use_small_hitbox = false;
        collided = true;
        int safe_count = 0;
        while(t->CheckCollisionRec(GetRectangle(), true) || CollideWithSolidObject()) {
            m_position.x -= 0.01f * sign<float,float>(m_velocity.x);
            if(++safe_count > 255) break;
        }
        m_velocity.x = 0;
    }

    // Vertical
    ProcessPhysicsY(dt);
    if(t->CheckCollisionRec(GetRectangle(), true) || CollideWithSolidObject()) {    // && m_velocity.y != 0.f ?
        m_use_small_hitbox = false;
        collided = true;
        int safe_count = 0;
        while(t->CheckCollisionRec(GetRectangle(), true) || CollideWithSolidObject()) {
            m_position.y -= 0.01f * sign<float,float>(m_velocity.y);
            if(++safe_count > 255) break;
        }
        m_velocity.y = 0;
    }

    if(collided && going_down) {
        m_velocity.x = 0;
        m_velocity.y = 0;
        m_enable_physics = false;
        if(IsPlaying()) {
            m_gameplay_state->ShowActionWidgets();
        }
    }
}
