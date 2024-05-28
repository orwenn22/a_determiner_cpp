#include "GlobalResources.h"

namespace Res {

Texture player_sprite;
Texture player_in_jump_sprite;
Texture player_jump_sprite;
Texture player_shooting_sprite;
Texture player_wall_sprite;

Texture explosion_sprite;
Texture wall_sprite;

Texture trowel_sprite;


void LoadAll() {
    player_sprite = LoadTexture("res/player.png");
    player_in_jump_sprite = LoadTexture("res/player_injump.png");
    player_jump_sprite = LoadTexture("res/player_jump.png");
    player_shooting_sprite = LoadTexture("res/player_shooting.png");
    player_wall_sprite = LoadTexture("res/player_wall.png");

    explosion_sprite = LoadTexture("res/explosion-boom.png");
    wall_sprite = LoadTexture("res/wall.png");

    trowel_sprite = LoadTexture("res/truelle.png");
}

void UnloadAll() {
    UnloadTexture(trowel_sprite);

    UnloadTexture(wall_sprite);
    UnloadTexture(explosion_sprite);

    UnloadTexture(player_wall_sprite);
    UnloadTexture(player_shooting_sprite);
    UnloadTexture(player_jump_sprite);
    UnloadTexture(player_in_jump_sprite);
    UnloadTexture(player_sprite);
}

};  //namespace Res
