cmake_minimum_required(VERSION 3.28)
project(UI_framework)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(raylib)
file(COPY ./res DESTINATION .)

# Get current branch
execute_process(
        COMMAND git rev-parse --abbrev-ref HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE)

# Get current commit
execute_process(
        COMMAND git rev-parse HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE)


add_executable(UI_framework src/main.cpp
        src/engine/widgets/WidgetManager.cpp
        src/engine/widgets/Widget.cpp
        src/engine/Globals.cpp
        src/engine/widgets/Button.cpp
        src/engine/widgets/Label.cpp
        src/engine/widgets/Clickable.cpp
        src/engine/windows/WindowManager.cpp
        src/engine/windows/Window.cpp
        src/engine/widgets/HSplit.cpp
        src/engine/widgets/VSplit.cpp
        src/engine/widgets/IntField.cpp
        src/engine/widgets/IntField.h
        src/engine/util/Scissors.cpp
        src/engine/object/ObjectManager.cpp
        src/engine/object/EntityObject.cpp
        src/engine/object/KinematicObject.cpp
        src/engine/object/KinematicObject.h
        src/engine/util/VectorOps.h
        src/engine/metrics/MetricsCamera.cpp
        src/engine/metrics/MetricsCamera.h
        src/engine/metrics/Graphics.cpp
        src/engine/metrics/Graphics.h
        src/engine/state/StateManager.cpp
        src/engine/state/StateManager.h
        src/engine/state/State.cpp
        src/engine/state/State.h
        src/engine/util/DebugOverlay.cpp
        src/engine/util/DebugOverlay.h
        src/GameplayState.cpp
        src/GameplayState.h
        src/Terrain.cpp
        src/Terrain.h
        src/objects/actors/Player.cpp
        src/objects/actors/Player.h
        src/engine/util/Math.h
        src/objects/ObjectIDs.h
        src/actions/Action.cpp
        src/actions/Action.h
        src/engine/object/KinematicPrediction.cpp
        src/engine/object/KinematicPrediction.h
        src/actions/JumpAction.cpp
        src/actions/JumpAction.h
        src/GlobalResources.cpp
        src/GlobalResources.h
        src/Teams.h
        src/objects/actors/Explosion.cpp
        src/objects/actors/Explosion.h
        src/objects/actors/Bullet.cpp
        src/objects/actors/Bullet.h
        src/actions/ShootAction.cpp
        src/actions/ShootAction.h
        src/objects/actors/Wall.cpp
        src/objects/actors/Wall.h
        src/actions/WallAction.cpp
        src/actions/WallAction.h
        src/objects/collectibles/Collectible.cpp
        src/objects/collectibles/Collectible.h
        src/objects/collectibles/Trowel.cpp
        src/objects/collectibles/Trowel.h
        src/objects/actors/Portal.cpp
        src/objects/actors/Portal.h
        src/actions/PortalAction.cpp
        src/actions/PortalAction.h
        src/objects/collectibles/Portalgun.cpp
        src/objects/collectibles/Portalgun.h
        src/engine/util/Trace.h
        src/engine/widgets/TiledButton.cpp
        src/engine/widgets/TiledButton.h
        src/widgets/FakeActionWidget.cpp
        src/widgets/FakeActionWidget.h
        src/widgets/ActionWidget.cpp
        src/widgets/ActionWidget.h
        src/utils/TiledBackground.cpp
        src/utils/TiledBackground.h
        src/menus/MainMenu.cpp
        src/menus/MainMenu.h
        src/menus/OptionsMenu.cpp
        src/menus/OptionsMenu.h
        src/KeyBinds.cpp
        src/KeyBinds.h
        src/KeyLookup.h
        src/KeyLookup.c
        src/menus/CreditsMenu.cpp
        src/menus/CreditsMenu.h
        src/menus/PauseMenu.cpp
        src/menus/PostGameMenu.cpp
)
target_link_libraries(UI_framework PRIVATE raylib)
target_include_directories(UI_framework PRIVATE ./src)

# Expose branch and commit as #DEFINED preprocessor things
target_compile_definitions(UI_framework PRIVATE
        "-DGIT_COMMIT_HASH=\"${GIT_COMMIT_HASH}\"")
target_compile_definitions(UI_framework PRIVATE
        "-DGIT_BRANCH=\"${GIT_BRANCH}\"")

