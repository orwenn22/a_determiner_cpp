#include "CollectibleSpawnWindow.h"

#include "engine/widgets/Button.h"
#include "engine/widgets/WidgetManager.h"
#include "objects/collectibles/Constructors.h"
#include "GameplayState.h"


CollectibleSpawnWindow::CollectibleSpawnWindow(int x, int y, GameplayState *gameplay_state) : Window(x, y, 300, 300) {
    m_gameplay_state = gameplay_state;

    SetTitle("Spawn collectible");
    m_titlebar_color = PURPLE;
    m_background_color = {10, 10, 10, 150};

    int painter_x = 10;
    int painter_y = 10;
    for(int i = 0; i < s_item_count; ++i) {
        Button *b = new Button(painter_x, painter_y, 25, 25, std::to_string(i), [=]() {
            m_gameplay_state->SetSpawnedObject(s_item_constructors[i]({0.f, 0.f}));
        });
        GetWidgetManager()->AddWidget(b);
        painter_x += 30;

        if(painter_x+30 >= Width()-10) {
            painter_x = 10;
            painter_y += 30;
        }
    }
}

CollectibleSpawnWindow::~CollectibleSpawnWindow() {
    m_gameplay_state->SetSpawnedObject(nullptr);
}
