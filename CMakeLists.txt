cmake_minimum_required(VERSION 3.28)
project(UI_framework)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(raylib)

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
)
target_link_libraries(UI_framework PRIVATE raylib)
