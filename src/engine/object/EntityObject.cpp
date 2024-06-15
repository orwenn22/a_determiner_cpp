#include "EntityObject.h"

#include <algorithm>

#include "../metrics/Graphics.h"
#include "BuiltinTypeIDs.h"


EntityObject::EntityObject(Vector2 position, float width, float height) {
    m_position.x = position.x;
    m_position.y = position.y;

    m_width = width;
    m_height = height;
    RegisterType(TypeID_EntityObject);
    m_manager = nullptr;
}

EntityObject::~EntityObject() {
    m_types.pop_back();
    m_manager = nullptr;
}

bool EntityObject::IsOfType(int type_id) {
    auto result = std::find(m_types.begin(), m_types.end(), type_id);
    return (result != m_types.end());
}

int EntityObject::GetMainType() {
    return m_types.back();
}

Rectangle EntityObject::GetRectangle() {
    return {m_position.x - m_width/2, m_position.y - m_height/2, m_width, m_height};
}

void EntityObject::DrawHitbox() {
    Metrics::DrawRectangle(GetRectangle(), RED, false);

    //Cross at the center of the object
    Metrics::DrawLine({m_position.x - m_width/4, m_position.y},
                      {m_position.x + m_width/4, m_position.y},
                      RED);
    Metrics::DrawLine({m_position.x, m_position.y - m_height/4},
                      {m_position.x, m_position.y + m_height/4},
                      RED);
}


////////////////////////////////
//// PROTECTED

void EntityObject::RegisterType(int type_id) {
    m_types.push_back(type_id);
}
