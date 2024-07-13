#ifndef A_DETERMINER_GLOBALRESOURCES_H
#define A_DETERMINER_GLOBALRESOURCES_H

#include <raylib.h>
#include "engine/TextureRef.h"

class Tileset;

namespace Res {
    extern Texture player_mini_sprite;
    extern Texture player_mini_ded_sprite;
    extern Texture player_sprite;
    extern Texture player_in_jump_sprite;
    extern Texture player_jump_sprite;
    extern Texture player_shooting_sprite;
    extern Texture player_wall_sprite;
    extern Texture player_portal_sprite;

    extern Texture green_marker_sprite;

    extern Texture explosion_sprite;
    extern Texture wall_sprite;
    extern Texture portal_sprite;

    extern Texture jump_icon_sprite;
    extern Texture shoot_icon_sprite;
    extern Texture trowel_sprite;
    extern Texture portalgun_sprite;
    extern Texture strength_upgrade_sprite;
    extern Texture strength_downgrade_sprite;
    extern Texture battery_sprite;
    extern Texture potato_sprite;

    extern Texture tiled_button_sprite;
    extern Texture tiled_button_left_sprite;
    extern Texture tiled_button_right_sprite;
    extern Texture empty_outline_sprite;

    extern Texture menubg_sprite;
    extern Texture menubg_options_sprite;
    extern Texture menubg_credits_sprite;
    extern Texture menubg_grayscale;

    extern TextureRef collisions_sprite;


    void LoadAll();
    void UnloadAll();

};  //namespace Res


#endif //A_DETERMINER_GLOBALRESOURCES_H
