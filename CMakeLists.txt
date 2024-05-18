cmake_minimum_required(VERSION 3.28)
project(UI_framework)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(raylib)

add_executable(UI_framework src/main.cpp
        src/widgets/WidgetManager.cpp
        src/widgets/Widget.cpp
        src/Globals.cpp
        src/widgets/Button.cpp
        src/widgets/Label.cpp
        src/widgets/Clickable.cpp
        src/windows/WindowManager.cpp
        src/windows/Window.cpp
        src/widgets/HSplit.cpp
        src/widgets/VSplit.cpp
        src/widgets/IntField.cpp
        src/widgets/IntField.h
        src/util/Scissors.cpp
)
target_link_libraries(UI_framework PRIVATE raylib)
