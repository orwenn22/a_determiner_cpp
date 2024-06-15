#include <filesystem>
#include <random>
#include <raylib.h>
#include <time.h>

#include "engine/Globals.h"
#include "engine/state/StateManager.h"
#include "engine/util/DebugOverlay.h"
#include "engine/util/IDBFS.h"
#include "engine/util/Paths.h"
#include "engine/util/Trace.h"
#include "engine/KeyBinds.h"
#include "menus/MainMenu.h"
#include "Config.h"
#include "Ext.h"
#include "GameplayState.h"
#include "GlobalResources.h"

#if !defined(__EMSCRIPTEN__)

// Relative paths are used in a lot of places, like for map loading, resources, etc...
// Therefore, we need to make sure we are in the correct directory before launching the game.
// (this is only required for the desktop build, because in emscripten every embedded files is at the root directory)

bool IsCWDCorrect(std::string exec_name) {
    bool r = false;
    for(auto f : std::filesystem::directory_iterator(std::filesystem::current_path())) {
        //TRACE("name : %s\n", f.path().filename().string().c_str());
        if(is_regular_file(f) && f.path().filename().string() == exec_name) {
            r = true;
        }
    }
    return r;
}

bool CorrectCWD(std::string argv0) {
    TRACE("Current CWD : %s\n", GetCWD().c_str());
    std::string executable_name = GetFileFromPath(argv0);
    if(IsCWDCorrect(executable_name)) return true;

    std::string executable_path = RemoveFileFromPath(argv0);
    TRACE("CWD incorrect, attempting to CHDIR into '%s'\n", executable_path.c_str());
    if(executable_path.empty()) return false;

    std::filesystem::current_path(executable_path);
    if(IsCWDCorrect(executable_name)) return true;

    TRACE("Invalid launch directory, exiting\n");
    return false;
}

#endif


//Try to load keybindings from config file, and initialise them if they are not set.
void InitKeybinds() {
    KeyBinds::Init();
    KeyBinds::The()->Load(Config::GetKeybindConfigPath());

    //If the keys are not in the config file then put the default
    if(!KeyBinds::The()->IsKeyRegistered(KeyBind_Action)) KeyBinds::The()->RegisterKey(KeyBind_Action, KEY_SPACE);
    if(!KeyBinds::The()->IsKeyRegistered(KeyBind_Left))   KeyBinds::The()->RegisterKey(KeyBind_Left, KEY_A);
    if(!KeyBinds::The()->IsKeyRegistered(KeyBind_Right))  KeyBinds::The()->RegisterKey(KeyBind_Right, KEY_D);

    //Save them just in case
    KeyBinds::The()->Save(Config::GetKeybindConfigPath());
}


int main(int argc, const char *argv[]) {
#ifdef __EMSCRIPTEN__
    MountIDBFS();
#else
    if(!CorrectCWD(argv[0])) return 1;
#endif

    Ext::Init();
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
