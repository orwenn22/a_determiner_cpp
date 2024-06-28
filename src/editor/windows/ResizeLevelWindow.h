#ifndef UI_FRAMEWORK_RESIZELEVELWINDOW_H
#define UI_FRAMEWORK_RESIZELEVELWINDOW_H

#include "engine/windows/Window.h"

class EditorState;

class ResizeLevelWindow : public Window {
public:
    ResizeLevelWindow(EditorState *editor);
    ~ResizeLevelWindow() override;


private:
    EditorState *m_editor;
    int m_grid_w, m_grid_h, m_level_w_m, m_level_h_m;
};


#endif //UI_FRAMEWORK_RESIZELEVELWINDOW_H
