#include <algorithm>
#include "ObjectManager.h"

#include "EntityObject.h"


ObjectManager::ObjectManager() {
    m_update_i = -1;
}

ObjectManager::~ObjectManager() {
    ClearTrashcan();
    for(EntityObject *o : m_objects) {
        delete o;
    }
    m_objects.clear();
}

void ObjectManager::Update(float dt) {
    for(m_update_i = 0; m_update_i < m_objects.size(); ++m_update_i) {
        m_objects[m_update_i]->Update(dt);
    }
    ClearTrashcan();
}

void ObjectManager::Draw() {
    for(EntityObject *o : m_objects) {
        o->Draw();
    }
}

void ObjectManager::AddObject(EntityObject *object) {
    if(object == nullptr) return;

    //TODO : check for duplicates ?
    m_objects.push_back(object);
    object->m_manager = this;
}

void ObjectManager::DestroyObject(EntityObject *object) {
    if(object == nullptr) return;

    //Check if object is in manager
    auto result = std::find(m_objects.begin(), m_objects.end(), object);
    if(result == m_objects.end()) return;

    //Compute index from search result
    size_t index = result - m_objects.begin();

    //Decrease update iterator if necessary
    if(index <= m_update_i) --m_update_i;

    //Put object in trashcan for future de-allocation
    m_objects.erase(result);
    m_trashcan.push_back(object);
}

bool ObjectManager::ObjectExist(EntityObject *object) {
    return std::find(m_objects.begin(), m_objects.end(), object) != m_objects.end();
}

std::vector<EntityObject *> ObjectManager::GetCollisions(EntityObject *source_object, int type_id) {
    std::vector<EntityObject *> result;
    if(source_object == nullptr) return result;

    Rectangle rec = source_object->GetRectangle();

    for(EntityObject *o : m_objects) {
        if(o == source_object) continue;

        if(o->IsOfType(type_id) && CheckCollisionRecs(rec, o->GetRectangle())) {
            result.push_back(o);
        }
    }

    return result;
}


/////////////////////////////////////////
// PRIVATE

void ObjectManager::ClearTrashcan() {
    if(m_trashcan.empty()) return;

    for(EntityObject *o : m_trashcan) {
        delete o;
    }

    m_trashcan.clear();
}
