#ifndef A_DETERMINER_LEAVEEDITORWINDOW_H
#define A_DETERMINER_LEAVEEDITORWINDOW_H

#include "engine/windows/Window.h"


class EditorState;

class LeaveEditorWindow : public Window {
public:
    LeaveEditorWindow(EditorState *editor, int x, int y);
    ~LeaveEditorWindow();

private:
    EditorState *m_editor;
};



#endif //A_DETERMINER_LEAVEEDITORWINDOW_H
