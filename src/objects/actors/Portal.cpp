#include "Portal.h"

#include <algorithm>
#include <cmath>

#include "engine/Globals.h"
#include "engine/metrics/Graphics.h"
#include "engine/metrics/MetricsCamera.h"
#include "engine/object/ObjectManager.h"
#include "engine/util/VectorOps.h"
#include "GameplayState.h"
#include "GlobalResources.h"
#include "../ObjectIDs.h"

Portal::Portal(Vector2 pos, GameplayState *gameplay_state) : EntityObject(pos, 1.f, 1.f) {
    m_gameplay_state = gameplay_state;
    m_cooldown = 1.f;
    m_mouse_hovering = false;
    m_destination = nullptr;
    m_whitelisted_types.push_back(TypeID_Player);
    m_whitelisted_types.push_back(TypeID_Bullet);
    m_whitelisted_types.push_back(TypeID_Wall);

    m_animation_duration = 1.f;
    m_animation_time = 0.f;
    m_animation_frame_count = 24;
}

Portal::~Portal() {
    //TODO : this naively assume that the two portals are pointing to each other, however in more complex cases
    //       it might not be the case
    if(m_destination != nullptr) {
        if(m_destination->m_destination == this) m_destination->m_destination = nullptr;
    }
}

void Portal::Update(float dt) {
    m_animation_time += dt;
    m_animation_time = (float)fmod((double)m_animation_time, (double)m_animation_duration);

    m_mouse_hovering = false;
    Vector2 mouse_meter = m_gameplay_state->GetCamera()->ConvertAbsoluteToMeters(GetMouseX(), GetMouseY());

    if(!IsMouseUsed() && CheckCollisionPointRec(mouse_meter, GetRectangle())) {
        UseMouse();
        m_mouse_hovering = true;
    }

    if(m_destination == nullptr) return;

    if(m_cooldown > 0.f) {
        m_cooldown -= dt;
        return;
    }

    auto cols = Manager()->GetCollisions(this, TypeID_EntityObject);
    for(EntityObject *e : cols) {
        if(std::find(m_whitelisted_types.begin(), m_whitelisted_types.end(), e->GetMainType()) == m_whitelisted_types.end())
            continue;

        m_cooldown = 1.f;
        m_destination->m_cooldown = 1.f;
        e->m_position = m_destination->m_position;
    }
}

void Portal::Draw() {
    //TODO : replace by sprite
    int current_frame = (int) (m_animation_time/m_animation_duration * (float)m_animation_frame_count);
    Metrics::DrawSpriteRotEx(Res::portal_sprite, {40.f*(float)current_frame, 0.f, 40.f, 40.f},
                             m_position, {m_width, m_height},
                             0.f, {255, 255, 255, (m_cooldown > 0.f) ? (unsigned char)127 : (unsigned char)255});

    // If there are no destination draw a red dot and stop drawing here
    if(m_destination == nullptr) {
        Metrics::DrawCircle(m_position, .05f, RED);
        return;
    }

    // Draw a line between this portal and its destinations if overed
    if(m_mouse_hovering || m_destination->m_mouse_hovering) {
        Vector2 vec_step = m_destination->m_position - m_position;
        vec_step = vec_step * (1.f/15.f);

        Vector2 painter_pos = m_position;
        for(int i = 0; i < 15; ++i) {
            Metrics::DrawCircle(painter_pos, .05f, {150, 92, 191, 255});
            painter_pos = painter_pos + vec_step;
        }
        Metrics::DrawLine(m_position, m_destination->m_position, {150, 92, 191, 255});
    }
}

void Portal::SetDestination(Portal *other) {
    m_destination = other;
}

void Portal::SpawnPair(ObjectManager *manager, Vector2 pos1, Vector2 pos2, GameplayState *gameplay_state) {
    if(manager == nullptr) return;
    Portal *p1 = new Portal(pos1, gameplay_state);
    Portal *p2 = new Portal(pos2, gameplay_state);
    p1->SetDestination(p2);
    p2->SetDestination(p1);
    manager->AddObject(p1);
    manager->AddObject(p2);
}
