#ifndef UI_FRAMEWORK_PORTALACTION_H
#define UI_FRAMEWORK_PORTALACTION_H

#include "Action.h"

class Portal;

class PortalAction : public Action {
public:
    PortalAction();

    void OnUpdate(Player *player, float dt);
    void OnDraw(Player *player);

private:
    Portal *m_first_portal;
    bool m_draw_preview;

    float m_animation_duration;
    float m_animation_time;
    int m_animation_frame_count;
};


#endif //UI_FRAMEWORK_PORTALACTION_H
