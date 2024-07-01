#include "ErrorWindow.h"

#include "engine/widgets/Label.h"
#include "engine/widgets/WidgetManager.h"


ErrorWindow::ErrorWindow(int x, int y, std::string message) : Window(x, y, 10, 10) {
    m_titlebar_color = WHITE;
    m_title_color = RED;
    SetTitle("Error");

    std::string line = "";
    int max_length_px = 10;
    int height = 24;
    for(char &c : message) {
        if(c != '\n') {
            line.push_back(c);
        }
        else {
            Label *label = new Label(3, height-12, 10, line);
            GetWidgetManager()->AddWidget(label);
            height += 12;
            int length_px = MeasureText(line.c_str(), 10) + 6;
            if(length_px > max_length_px) max_length_px = length_px;
            line = "";
        }
    }

    if(!line.empty()) {
        Label *label = new Label(3, height-12, 10, line);
        GetWidgetManager()->AddWidget(label);
        height += 12;
        int length_px = MeasureText(line.c_str(), 10) + 6;
        if(length_px > max_length_px) max_length_px = length_px;
        line = "";
    }

    SetSize(max_length_px, height+12);
}

ErrorWindow::~ErrorWindow() = default;
