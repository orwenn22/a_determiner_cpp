#ifndef UI_FRAMEWORK_OBJECTMANAGER_H
#define UI_FRAMEWORK_OBJECTMANAGER_H

#include "raylib.h"
#include <vector>


class EntityObject;

class ObjectManager {
public:
    ObjectManager();
    ~ObjectManager();

    void Update(float dt);
    void Draw(float dt);

    void AddObject(EntityObject *object);
    //TODO : method to remove without de-allocation ?
    void DestroyObject(EntityObject *object);

    /**
     * Check the collisions between an objects and all the others
     * @param source_object the object we want to check collision with the others
     * @param type_id type of the other objects we want to get
     * @return the objects colliding with source_object
     */
    std::vector<EntityObject *> GetCollisions(EntityObject *source_object, int type_id);

private:
    void ClearTrashcan();

    std::vector<EntityObject *> m_objects;
    std::vector<EntityObject *> m_trashcan;
    int m_update_i;
};


#endif //UI_FRAMEWORK_OBJECTMANAGER_H
