#ifndef UI_FRAMEWORK_GLOBALRESOURCES_H
#define UI_FRAMEWORK_GLOBALRESOURCES_H

#include <raylib.h>

namespace Res {
    extern Texture player_sprite;
    extern Texture player_in_jump_sprite;
    extern Texture player_jump_sprite;
    extern Texture player_shooting_sprite;
    extern Texture player_wall_sprite;
    extern Texture player_portal_sprite;

    extern Texture explosion_sprite;
    extern Texture wall_sprite;
    extern Texture portal_sprite;

    extern Texture trowel_sprite;
    extern Texture portalgun_sprite;


    void LoadAll();
    void UnloadAll();

};  //namespace Res


#endif //UI_FRAMEWORK_GLOBALRESOURCES_H
