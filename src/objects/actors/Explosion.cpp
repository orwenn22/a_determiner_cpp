#include "Explosion.h"

#include <cmath>
#include <cstdio>
#include <raymath.h>

#include "engine/metrics/Graphics.h"
#include "engine/object/BuiltinTypeIDs.h"
#include "engine/object/KinematicObject.h"
#include "engine/object/ObjectManager.h"
#include "engine/util/VectorOps.h"
#include "GameplayState.h"
#include "GlobalResources.h"
#include "Terrain.h"

Explosion::Explosion(Vector2 pos, float radius, GameplayState *gameplay_state) : EntityObject(pos, radius*2, radius*2) {
    m_radius = radius;
    m_explosion_power = 500.f;
    m_gameplay_state = gameplay_state;
    m_anim_timer = 0.f;
    m_anim_duraton = 1.f;
    m_exploded_terrain = false;
    m_exploded_enemies = false;
}

void Explosion::Update(float dt) {
    m_anim_timer += dt;

    ExplodeNearbyEnemies(dt);

    int animation_frame = GetCurrentAnimationFrame();
    if(animation_frame >= 6) {
        ExplodeTerrain();
    }

    if(m_anim_timer >= m_anim_duraton) {
        Manager()->DestroyObject(this);
    }
}

void Explosion::Draw() {
    int animation_frame = GetCurrentAnimationFrame();

    Metrics::DrawSpriteRotEx(Res::explosion_sprite, {71.f*(float)animation_frame, 0.f, 71.f, 98.f},
                             m_position,
                             {m_width*1.5f, m_height*2.25f},
                             0.f,
                             WHITE);
}


/////////////////////////////////
//// PRIVATE

void Explosion::ExplodeNearbyEnemies(float dt) {
    if(m_exploded_enemies) return;
    m_exploded_enemies = true;

    for(EntityObject *obj : *(Manager()->RawVector())) {
        if(obj->IsOfType(TypeID_KinematicObject)) {
            ApplyForceToOther((KinematicObject *) obj, dt);
        }
    }
}

void Explosion::ExplodeTerrain() {
    if(m_exploded_terrain) return;
    m_exploded_terrain = true;
    m_gameplay_state->GetTerrain()->DestroyCircle(m_position, m_radius);
}

int Explosion::GetCurrentAnimationFrame() {
    return (int)((m_anim_timer/m_anim_duraton)*16.f);       //16 frames in explosion animation
}

void Explosion::ApplyForceToOther(KinematicObject *obj, float dt) {
    Vector2 obj_pos = obj->GetPosition();
    Vector2 relative_position = obj_pos - m_position;       //Relative position of the other object from the explosion
    float distance = sqrt(relative_position.x*relative_position.x + relative_position.y*relative_position.y);

    // The object is too far, we don't need to apply a force on it.
    if(distance > m_radius) return;

    // Calculate the coefficient we neet to apply to the force
    float force_coefficient = 1.f - distance/m_radius;
    float total_coefficient = force_coefficient * m_explosion_power;

    // Calculate the vector of the force
    Vector2 normalised_vector = Vector2Normalize(relative_position);
    Vector2 total_force = normalised_vector * (total_coefficient/dt);

    printf("Explosion::ApplyForceToOther : Applying force %f %f", total_force.x, total_force.y);
    obj->ApplyForce(total_force);

    // If the player is grounded, then its physics is disabled, which mean that if we don't enable it again,
    // the force will be processed when the ground is destroyed, which would cause the player to make a huuuge jump.
    obj->EnablePhysics();   // TODO ? : only do this for the player ??? idk
}


