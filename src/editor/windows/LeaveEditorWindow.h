#ifndef LEAVEEDITORWINDOW_H
#define LEAVEEDITORWINDOW_H

#include "engine/windows/Window.h"


class EditorState;

class LeaveEditorWindow : public Window {
public:
    LeaveEditorWindow(EditorState *editor, int x, int y);
    ~LeaveEditorWindow();

private:
    EditorState *m_editor;
};



#endif //LEAVEEDITORWINDOW_H
