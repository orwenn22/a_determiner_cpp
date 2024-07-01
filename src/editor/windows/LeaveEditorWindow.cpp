#include "LeaveEditorWindow.h"

#include <EditorState.h>

#include "engine/state/StateManager.h"
#include "engine/widgets/Button.h"
#include "engine/widgets/Label.h"
#include "engine/widgets/WidgetManager.h"
#include "menus/MainMenu.h"


LeaveEditorWindow::LeaveEditorWindow(EditorState *editor, int x, int y) : Window(15, 15, 150, 50) {
    SetTitle("Leave editor ?");
    m_editor = editor;

    Label *message_label = new Label(3, 3, 10, "Exit the editor ? :(");

    Button *confirm_button = new Button(3, 3, 75, 20, "Let me out!!", [this]() {
        m_editor->Manager()->SetState(new MainMenu);
    });
    confirm_button->CenterLabel();
    confirm_button->SetAlignment(WidgetAlignment_BottomRight);

    GetWidgetManager()->AddWidget(message_label);
    GetWidgetManager()->AddWidget(confirm_button);
}

LeaveEditorWindow::~LeaveEditorWindow() = default;
