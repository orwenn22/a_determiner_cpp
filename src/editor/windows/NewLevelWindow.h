#ifndef A_DETERMINER_NEWLEVELWINDOW_H
#define A_DETERMINER_NEWLEVELWINDOW_H

#include "engine/windows/Window.h"

class EditorState;

class NewLevelWindow : public Window {
public:
    NewLevelWindow(EditorState *editor);
    ~NewLevelWindow() override;


private:
    EditorState *m_editor;
    int m_grid_w, m_grid_h, m_level_w_m, m_level_h_m;
};


#endif //A_DETERMINER_NEWLEVELWINDOW_H
