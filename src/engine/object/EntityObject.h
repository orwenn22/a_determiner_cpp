#ifndef UI_FRAMEWORK_ENTITYOBJECT_H
#define UI_FRAMEWORK_ENTITYOBJECT_H

#include "raylib.h"
#include <vector>

class ObjectManager;

class EntityObject {
public:
    EntityObject(Vector2 position, float width, float height);
    virtual ~EntityObject();

    virtual void Update(float dt) = 0;
    virtual void Draw() = 0;

    bool IsOfType(int type_id);
    int GetMainType();

    Rectangle GetRectangle();

protected:
    Vector2 m_position;
    float m_width, m_height;

private:
    friend ObjectManager;

    std::vector<int> m_types;
    ObjectManager *m_manager;
};


#endif //UI_FRAMEWORK_ENTITYOBJECT_H
