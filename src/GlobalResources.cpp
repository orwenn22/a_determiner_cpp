#include "GlobalResources.h"

namespace Res {

Texture player_sprite;
Texture player_in_jump_sprite;
Texture player_jump_sprite;
Texture player_shooting_sprite;

Texture explosion_sprite;


void LoadAll() {
    player_sprite = LoadTexture("res/player.png");
    player_in_jump_sprite = LoadTexture("res/player_injump.png");
    player_jump_sprite = LoadTexture("res/player_jump.png");
    player_shooting_sprite = LoadTexture("res/player_shooting.png");

    explosion_sprite = LoadTexture("res/explosion-boom.png");
}

void UnloadAll() {
    UnloadTexture(explosion_sprite);

    UnloadTexture(player_shooting_sprite);
    UnloadTexture(player_jump_sprite);
    UnloadTexture(player_in_jump_sprite);
    UnloadTexture(player_sprite);
}

};  //namespace Res
