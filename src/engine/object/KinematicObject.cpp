#include "KinematicObject.h"

#include "../metrics/Graphics.h"
#include "../util/VectorOps.h"
#include "BuiltinTypeIDs.h"

KinematicObject::KinematicObject(Vector2 position, float width, float height, float mass) : EntityObject(position, width, height) {
    RegisterType(TypeID_KinematicObject);

    m_velocity = {0.f, 0.f};
    m_acceleration = {0.f, 0.f};

    m_enable_physics = true;
    m_enable_gravity = true;

    m_gravity = 9.81f;
    m_down_vector = {0.f, 1.f};
}

KinematicObject::~KinematicObject() {
    //Nothing for now
}

void KinematicObject::ApplyForce(Vector2 force) {
    Vector2 new_acceleration = force / m_mass;
    m_acceleration = m_acceleration + new_acceleration;
}

void KinematicObject::DrawHitbox() {
    EntityObject::DrawHitbox();

    //Velocity vector
    Metrics::DrawLine(m_position, m_position + m_velocity, YELLOW);
}

////////////////////////////////
//// PROTECTED

void KinematicObject::ProcessPhysicsX(float dt) {
    if(!m_enable_physics) return;

    if(m_enable_physics) {
        ApplyForce({m_down_vector.x*m_mass, 0.f});
    }

    m_velocity.x += m_acceleration.x * dt;
    m_position.x += m_velocity.x * dt;
    m_acceleration.x = 0.f;
}

void KinematicObject::ProcessPhysicsY(float dt) {
    if(!m_enable_physics) return;

    if(m_enable_physics) {
        ApplyForce({0.f, m_down_vector.y*m_mass});
    }

    m_velocity.y += m_acceleration.y * dt;
    m_position.y += m_velocity.y * dt;
    m_acceleration.y = 0.f;
}

void KinematicObject::ProcessPhysics(float dt) {
    ProcessPhysicsX(dt);
    ProcessPhysicsY(dt);
}


