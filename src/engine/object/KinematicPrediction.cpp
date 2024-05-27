#include "KinematicPrediction.h"

#include "../metrics/Graphics.h"

KinematicPrediction::KinematicPrediction(Vector2 pos, float w, float h, float mass, Vector2 velocity, Vector2 acceleration)
: KinematicObject(pos, w, h, mass) {
    m_velocity = velocity;
    m_acceleration = acceleration;
    m_enable_physics = true;
}

KinematicPrediction::KinematicPrediction(KinematicObject *other)
: KinematicPrediction(other->GetPosition(), other->Width(), other->Height(), other->Mass(), other->GetVelocity(), other->GetAcceleration()) {
    m_enable_gravity = other->HaveGravityEnabled();
}

void KinematicPrediction::DrawSimulation(int step, int simulation_amount, float vdt, Color c) {
    // Backup all current physics state
    Vector2 a = m_acceleration;
    Vector2 v = m_velocity;
    Vector2 p = m_position;

    //Simulate
    for(int i = 0; i < simulation_amount; ++i) {
        ProcessPhysics(vdt);
        if(i%step == 0) Metrics::DrawCircle(m_position, .1f, c);
    }

    // Restore
    m_acceleration = a;
    m_velocity = v;
    m_position = p;
}
