#include "GlobalResources.h"

#include "engine/Tileset.h"

namespace Res {

Texture player_mini_sprite;
Texture player_mini_ded_sprite;
Texture player_sprite;
Texture player_in_jump_sprite;
Texture player_jump_sprite;
Texture player_shooting_sprite;
Texture player_wall_sprite;
Texture player_portal_sprite;

Texture green_marker_sprite;;

Texture explosion_sprite;
Texture wall_sprite;
Texture portal_sprite;

Texture jump_icon_sprite;
Texture shoot_icon_sprite;
Texture trowel_sprite;
Texture portalgun_sprite;
Texture strength_upgrade_sprite;
Texture strength_downgrade_sprite;
Texture battery_sprite;
Texture potato_sprite;

Texture tiled_button_sprite;
Texture tiled_button_left_sprite;
Texture tiled_button_right_sprite;
Texture empty_outline_sprite;

Texture menubg_sprite;
Texture menubg_options_sprite;
Texture menubg_credits_sprite;
Texture menubg_grayscale;

Texture collisions_sprite;
Tileset *collisions_tileset = nullptr;


void LoadAll() {
    player_mini_sprite = LoadTexture("res/mini.png");
    player_mini_ded_sprite = LoadTexture("res/mini_ded.png");
    player_sprite = LoadTexture("res/player.png");
    player_in_jump_sprite = LoadTexture("res/player_injump.png");
    player_jump_sprite = LoadTexture("res/player_jump.png");
    player_shooting_sprite = LoadTexture("res/player_shooting.png");
    player_wall_sprite = LoadTexture("res/player_wall.png");
    player_portal_sprite = LoadTexture("res/player_portal.png");

    green_marker_sprite = LoadTexture("res/green_marker.png");

    jump_icon_sprite = LoadTexture("res/jumping_sprite.png");
    shoot_icon_sprite = LoadTexture("res/cannon_sprite.png");
    explosion_sprite = LoadTexture("res/explosion-boom.png");
    wall_sprite = LoadTexture("res/wall.png");
    portal_sprite = LoadTexture("res/portal_anim.png");
    strength_upgrade_sprite = LoadTexture("res/strength_upgrade.png");
    strength_downgrade_sprite = LoadTexture("res/strength_downgrade.png");
    battery_sprite = LoadTexture("res/battery.png");
    potato_sprite = LoadTexture("res/potato.png");

    trowel_sprite = LoadTexture("res/truelle.png");
    portalgun_sprite = LoadTexture("res/portal_gun.png");

    tiled_button_sprite = LoadTexture("res/tiled_button.png");
    tiled_button_left_sprite = LoadTexture("res/tiled_button_left.png");
    tiled_button_right_sprite = LoadTexture("res/tiled_button_right.png");
    empty_outline_sprite = LoadTexture("res/empty_outline.png");

    menubg_sprite = LoadTexture("res/menubg.png");
    menubg_options_sprite = LoadTexture("res/menubg_option.png");
    menubg_credits_sprite = LoadTexture("res/menubg_credits_alt.png");
    menubg_grayscale = LoadTexture("res/menubg_grayscale.png");

    collisions_sprite = LoadTexture("res/collisions.png");
    collisions_tileset = new Tileset(&collisions_sprite, 16, 16, false);
}

void UnloadAll() {
    delete collisions_tileset;
    UnloadTexture(collisions_sprite);

    UnloadTexture(menubg_grayscale);
    UnloadTexture(menubg_credits_sprite);
    UnloadTexture(menubg_options_sprite);
    UnloadTexture(menubg_sprite);

    UnloadTexture(empty_outline_sprite);
    UnloadTexture(tiled_button_right_sprite);
    UnloadTexture(tiled_button_left_sprite);
    UnloadTexture(tiled_button_sprite);

    UnloadTexture(portalgun_sprite);
    UnloadTexture(trowel_sprite);

    UnloadTexture(potato_sprite);
    UnloadTexture(battery_sprite);
    UnloadTexture(strength_downgrade_sprite);
    UnloadTexture(strength_upgrade_sprite);
    UnloadTexture(portal_sprite);
    UnloadTexture(wall_sprite);
    UnloadTexture(explosion_sprite);
    UnloadTexture(shoot_icon_sprite);
    UnloadTexture(jump_icon_sprite);

    UnloadTexture(green_marker_sprite);

    UnloadTexture(player_portal_sprite);
    UnloadTexture(player_wall_sprite);
    UnloadTexture(player_shooting_sprite);
    UnloadTexture(player_jump_sprite);
    UnloadTexture(player_in_jump_sprite);
    UnloadTexture(player_sprite);
    UnloadTexture(player_mini_ded_sprite);
    UnloadTexture(player_mini_sprite);
}

};  //namespace Res
