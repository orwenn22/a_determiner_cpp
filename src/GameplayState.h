#ifndef UI_FRAMEWORK_GAMEPLAYSTATE_H
#define UI_FRAMEWORK_GAMEPLAYSTATE_H

#include "engine/state/State.h"

#include <raylib.h>


class MetricsCamera;

class GameplayState : public State {
public:
    GameplayState();
    ~GameplayState() override;

    void Update(float dt) override;
    void Draw() override;

    inline const char *GetIdentifier() override { return "GameplayState"; }


private:
    void HandleDragCamera(float mouse_x, float mouse_y);        //Take mouse pos in pixel

    MetricsCamera *m_camera;
    bool m_cam_follow_mouse;
    Vector2 m_cam_mouse_offset;
};


#endif //UI_FRAMEWORK_GAMEPLAYSTATE_H
