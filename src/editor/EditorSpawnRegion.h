#ifndef UI_FRAMEWORK_EDITORSPAWNREGION_H
#define UI_FRAMEWORK_EDITORSPAWNREGION_H

#include <cstdio>

class EditorState;

class EditorSpawnRegion {
public:
    EditorSpawnRegion(EditorState *editor, float x, float y, float w, float h, int team_index);

    void Update();
    void Draw();

    void Save(FILE *out_file);

private:
    float m_x, m_y, m_w, m_h;
    int m_team_index;

    bool m_follow_mouse;
    bool m_resized;
    float m_mouse_offset_x, m_mouse_offset_y;

    EditorState *m_editor;
};


#endif //UI_FRAMEWORK_EDITORSPAWNREGION_H
