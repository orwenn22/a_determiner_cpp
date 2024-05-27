cmake_minimum_required(VERSION 3.28)
project(UI_framework)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(raylib)
file(COPY ./res DESTINATION .)

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
)
target_link_libraries(UI_framework PRIVATE raylib)
target_include_directories(UI_framework PRIVATE ./src)
