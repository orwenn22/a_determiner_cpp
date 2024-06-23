#ifndef UI_FRAMEWORK_EDITORSTATE_H
#define UI_FRAMEWORK_EDITORSTATE_H

#include "engine/state/State.h"

class TilemapTerrain;
class TiledBackground;

class EditorState : public State {
public:
    EditorState();
    ~EditorState() override;

    void Update(float dt) override;
    void Draw() override;


private:
    TiledBackground *m_bg;

    TilemapTerrain *m_terrain;
};


#endif //UI_FRAMEWORK_EDITORSTATE_H
