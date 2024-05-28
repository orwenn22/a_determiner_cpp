#ifndef UI_FRAMEWORK_WALLACTION_H
#define UI_FRAMEWORK_WALLACTION_H

#include "Action.h"

class WallAction : public Action {
public:
    WallAction();

    void OnClick(Player *player, int action_index) override;
    void OnUpdate(Player *player, float dt) override;
    void OnDraw(Player *player) override;


private:
    void PlaceWall(Player *player);

    // For the character animation
    float m_animation_time;             // Time since the beginning of the animation cycle (in seconds)
    int m_animation_current_frame;      // Current frame being displayed
    int m_animation_frame_count;          // Number of frames in the animation
    float m_animation_duration;           // The time it take in second to loop through all the frames

};


#endif //UI_FRAMEWORK_WALLACTION_H
