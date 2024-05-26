#include "EntityObject.h"

#include <algorithm>

#include "BuiltinTypeIDs.h"


EntityObject::EntityObject(Vector2 position, float width, float height) {
    m_position.x = position.x;
    m_position.y = position.y;

    m_width = width;
    m_height = height;
    m_types.push_back(TypeID_EntityObject);
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
    return (Rectangle) {m_position.x - m_width/2, m_position.y - m_height/2, m_width, m_height};
}
