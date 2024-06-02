#ifndef UI_FRAMEWORK_POSTGAMEMENU_H
#define UI_FRAMEWORK_POSTGAMEMENU_H

#include "engine/state/State.h"

class TiledBackground;
class WidgetManager;

class PostGameMenu : public State {
public:
    PostGameMenu(int winning_team);     //TODO : pass gameplay stats here
    ~PostGameMenu() override;

    void Update(float dt) override;
    void Draw() override;

    inline const char *GetIdentifier() override { return "PostGameMenu"; }

private:
    WidgetManager *m_widgets;
    TiledBackground *m_bg;
};


#endif //UI_FRAMEWORK_POSTGAMEMENU_H
