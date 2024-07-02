#ifndef A_DETERMINER_EDITORSPAWNREGION_H
#define A_DETERMINER_EDITORSPAWNREGION_H

#include <cstdio>

class EditorLevel;
class EditorState;

class EditorSpawnRegion {
public:
    EditorSpawnRegion(EditorLevel *level, float x, float y, float w, float h, int team_index);

    //For the editor
    void Update(EditorState *editor);
    void Draw(EditorState *editor);

    void Save(FILE *out_file);

    //Getters
    inline float X() { return m_x; }
    inline float Y() { return m_y; }
    inline float Width() { return m_w; }
    inline float Height() { return m_h; }
    inline int TeamIndex() { return m_team_index; }

private:
    float m_x, m_y, m_w, m_h;
    int m_team_index;

    bool m_follow_mouse;
    bool m_resized;
    float m_mouse_offset_x, m_mouse_offset_y;

    EditorLevel *m_level;
};


#endif //A_DETERMINER_EDITORSPAWNREGION_H
