#include <random>
#include <raylib.h>
#include <time.h>

#include "engine/Globals.h"
#include "engine/state/StateManager.h"
#include "engine/util/DebugOverlay.h"
#include "engine/util/IDBFS.h"
#include "engine/KeyBinds.h"
#include "menus/MainMenu.h"
#include "Config.h"
#include "GameplayState.h"
#include "GlobalResources.h"


//Try to load keybindings from config file, and initialise them if they are not set.
void InitKeybinds() {
    KeyBinds::Load(Config::GetKeybindConfigPath());
    if(KeyBinds::The() == nullptr) KeyBinds::Init();

    //If the keys are not in the config file then put the default
    if(!KeyBinds::The()->IsKeyRegistered(KeyBind_Action)) KeyBinds::The()->RegisterKey(KeyBind_Action, KEY_SPACE);
    if(!KeyBinds::The()->IsKeyRegistered(KeyBind_Left))   KeyBinds::The()->RegisterKey(KeyBind_Left, KEY_A);
    if(!KeyBinds::The()->IsKeyRegistered(KeyBind_Right))  KeyBinds::The()->RegisterKey(KeyBind_Right, KEY_D);

    //Save them just in case
    KeyBinds::The()->Save(Config::GetKeybindConfigPath());
}


int main() {
#ifdef __EMSCRIPTEN__
    MountIDBFS();
#endif

    //TODO : set cwd to the path of the executable when not using emscripten ?
    srand(time(nullptr));
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(960, 540, "À déterminer");
    SetExitKey(KEY_NULL);
    SetTargetFPS(240);
    InitKeybinds();
    Res::LoadAll();

    StateManager *state_manager = new StateManager(new MainMenu);

    bool display_debug_overlay = true;
    while(!WindowShouldClose() && !QuitRequested()) {
        // UPDATE
        float dt = GetFrameTime();
        display_debug_overlay ^= IsKeyPressed(KEY_F3);
        ResetMouse();
        state_manager->Update(dt);

        //DRAW
        BeginDrawing();
        ClearBackground(BLACK);
        state_manager->Draw();
        if(display_debug_overlay) DrawDebugOverlay(dt, state_manager);
        EndDrawing();
    }

    delete state_manager;
    Res::UnloadAll();
    KeyBinds::The()->Save(Config::GetKeybindConfigPath());
    KeyBinds::Finit();
    CloseWindow();
    return 0;
}
