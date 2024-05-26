#ifndef UI_FRAMEWORK_KINEMATICOBJECT_H
#define UI_FRAMEWORK_KINEMATICOBJECT_H

#include "EntityObject.h"

class KinematicObject : public EntityObject {
public:
    KinematicObject(Vector2 position, float width, float height, float mass);
    ~KinematicObject() override;

    void ApplyForce(Vector2 force);

    void DrawHitbox() override;

protected:
    void ProcessPhysicsX(float dt);
    void ProcessPhysicsY(float dt);
    void ProcessPhysics(float dt);

    Vector2 m_velocity;
    Vector2 m_acceleration;
    float m_mass;
    bool m_enable_physics;
    bool m_enable_gravity;

    Vector2 m_down_vector;      //IMPORTANT : this should be normalised to a length of 1.
    float m_gravity;
};


#endif //UI_FRAMEWORK_KINEMATICOBJECT_H
