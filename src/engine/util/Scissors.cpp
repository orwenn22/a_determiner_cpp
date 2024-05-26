#include "Scissors.h"

#include <vector>
#include "raylib.h"

static std::vector<Rectangle> s_scissors;

void PushScissors(Rectangle rec) {
    if(!s_scissors.empty()) {
        EndScissorMode();
    }
    s_scissors.push_back(rec);
    BeginScissorMode((int)rec.x, (int)rec.y, (int)rec.width, (int)rec.height);
}

void PopScissors() {
    if(s_scissors.empty()) return;

    EndScissorMode();
    s_scissors.pop_back();
    if(s_scissors.empty()) return;

    Rectangle& rec = s_scissors.back();
    BeginScissorMode((int)rec.x, (int)rec.y, (int)rec.width, (int)rec.height);
}