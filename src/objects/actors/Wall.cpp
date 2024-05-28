#include "Wall.h"

#include "engine/metrics/Graphics.h"
#include "engine/object/ObjectManager.h"
#include "engine/util/Math.h"
#include "objects/actors/Player.h"
#include "GameplayState.h"
#include "GlobalResources.h"
#include "Terrain.h"
#include "../ObjectIDs.h"


Wall::Wall(Vector2 pos, float w, float h, GameplayState *gameplay_state) : KinematicObject(pos, w, h, 50.f) {
    RegisterType(TypeID_Wall);
    m_gameplay_state = gameplay_state;
    m_solid_types.push_back(TypeID_Wall);
}

void Wall::Update(float dt) {
    if(!Grounded()) m_enable_physics = true;
    if(m_enable_physics) UpdatePhysics(dt);     //If we have problems with the wall we can remove this condition
}

void Wall::Draw() {
    Metrics::DrawSpriteScale(Res::wall_sprite, GetRectangle(), WHITE);
}

bool Wall::Grounded() {
    float old_x = m_position.x;
    float old_y = m_position.y;

    //m_position = m_position + m_down_vector * 0.02f;
    m_position.x += m_down_vector.x * 0.02f;
    m_position.y += m_down_vector.y * 0.02f;

    bool r = m_gameplay_state->GetTerrain()->CheckCollisionRec(GetRectangle(), true) || CollideWithSolidObject();

    m_position = {old_x, old_y};
    return r;
}

bool Wall::CollideWithSolidObject() {
    for(int solid_type : m_solid_types) {
        auto col = Manager()->GetCollisions(this, solid_type);
        if(!col.empty()) return true;
    }
    return false;
}


///////////////////////////////
//// PRIVATE

void Wall::UpdatePhysics(float dt) {
    Terrain *t = m_gameplay_state->GetTerrain();
    if(t == nullptr) return;

    bool collided = false;
    bool going_down = (sign<float, int>(m_velocity.x) == sign<float, int>(m_down_vector.x)
                       || sign<float, int>(m_velocity.y) == sign<float, int>(m_down_vector.y));

    //Horizontal
    ProcessPhysicsX(dt);

    auto cols = Manager()->GetCollisions(this, TypeID_Player);
    for(EntityObject *e : cols) {
        Player *player = (Player *) e;

        // Make ture the player fall if it is pushed in the void by the wall
        player->EnablePhysics();

        // Shift the player in the opposite direction of the wall
        Rectangle rec = GetRectangle();
        int it = 0;     // (it = iterations)
        while(CheckCollisionRecs(rec, player->GetRectangle()) && it < 250) {
            player->m_position.x += sign<float, float>(m_velocity.x) * .02f;
            ++it;
        }

        // After pushing the player, if we are on a slope, it is possible that it is clipping in the floor,
        // so move it up if it is the case
        it = 0;
        while(m_gameplay_state->GetTerrain()->CheckCollisionRec(player->GetRectangle()) && it <= 150) {
            player->m_position.y -= .01f;       //TODO : take into account down vector ?
            ++it;
        }

        // Crushed by wall and terrain
        if(m_gameplay_state->GetTerrain()->CheckCollisionRec(player->GetRectangle()) || player->CollideWithSolidObject()) {
            m_gameplay_state->KillPlayer(player);
            continue;       //In case we add more stuff after that in the future
        }
    }

    // If the wall itself if colliding with the terrain or another solid object then we shift it to the opposite direction
    if(m_gameplay_state->GetTerrain()->CheckCollisionRec(GetRectangle(), true) || CollideWithSolidObject()) {
        collided = true;
        int safe_count = 0;
        while(m_gameplay_state->GetTerrain()->CheckCollisionRec(GetRectangle(), true) || CollideWithSolidObject()) {
            m_position.x -= sign<float, float>(m_velocity.x) * .01f;
            if(++safe_count >= 250) break;      //suffocation ?
        }
    }


    // Vertical
    ProcessPhysicsY(dt);

    // If the wall crush players then we need to kill them.
    // If the player is on the wall however, we need to move it with the wall. | TODO : stress test this
    cols = Manager()->GetCollisions(this, TypeID_Player);
    for(EntityObject *e : cols) {
        Player *player = (Player *) e;
        if(going_down) {      //going down
            m_gameplay_state->KillPlayer(player);
        }
        else {                      //going up or static
            player->m_velocity.y = m_velocity.y;
            player->m_velocity.x = m_velocity.x;        // maybe ???
            player->EnablePhysics();
        }
    }

    // If the wall is clipping with the terrain then we make it go in the opposite direction of its velocity
    if(m_gameplay_state->GetTerrain()->CheckCollisionRec(GetRectangle(), true) || CollideWithSolidObject()) {
        collided = true;
        int safe_count = 0;
        while(m_gameplay_state->GetTerrain()->CheckCollisionRec(GetRectangle(), true) || CollideWithSolidObject()) {
            m_position.y -= sign<float, float>(m_velocity.y) * .01f;
            if(++safe_count >= 250) break;      //suffocation ?
        }
        m_velocity.y = 0.f;
    }

    if(collided && going_down) {
        m_velocity = {0.f, 0.f};
        m_enable_physics = false;
    }
}
