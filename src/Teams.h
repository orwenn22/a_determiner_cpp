#ifndef UI_FRAMEWORK_TEAMS_H
#define UI_FRAMEWORK_TEAMS_H

#include <raylib.h>

static Color s_team_colors[] = {
    BLUE,
    RED,
    GREEN,
    YELLOW,
    PURPLE,
    ORANGE,
    //Add more color if we have more teams in the future
};

static const char *s_team_names[] = {
    "blue",
    "red",
    "green",
    "yellow",
    "purple",
    "orange",
    //Add more names if we have more teams in the future
};

static constexpr int s_team_count = sizeof(s_team_colors) / sizeof(Color);

#endif //UI_FRAMEWORK_TEAMS_H
