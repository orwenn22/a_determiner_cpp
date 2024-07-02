#ifndef A_DETERMINER_PORTAL_H
#define A_DETERMINER_PORTAL_H

#include "engine/object/EntityObject.h"

#include <vector>

class GameplayState;

class Portal : public EntityObject {
public:
    explicit Portal(Vector2 pos, GameplayState *gameplay_state);
    ~Portal() override;

    void Update(float dt) override;
    void Draw() override;

    void SetDestination(Portal *other);

    static void SpawnPair(ObjectManager *manager, Vector2 pos1, Vector2 pos2, GameplayState *gameplay_state);

private:
    GameplayState *m_gameplay_state;
    Portal *m_destination;
    float m_cooldown;
    bool m_mouse_hovering;
    std::vector<int> m_whitelisted_types;

    float m_animation_duration;
    float m_animation_time;
    int m_animation_frame_count;
};


#endif //A_DETERMINER_PORTAL_H
