#ifndef A_DETERMINER_OPTIONSMENU_H
#define A_DETERMINER_OPTIONSMENU_H

#include "engine/state/State.h"

#include <map>

class TiledBackground;
class TiledButton;
class WidgetManager;

class OptionsMenu : public State {
public:
    OptionsMenu();
    ~OptionsMenu() override;

    void Update(float dt) override;
    void Draw() override;

    inline const char *GetIdentifier() override { return "OptionsMenu"; }


private:
    void RequestRebinding(int keybind_id);
    void UpdateKeyButtonLabels();

    TiledBackground *m_bg;
    WidgetManager *m_widgets;

    int m_currently_rebinding;                      //KeyBindID currently being rebinded
    std::map<int, TiledButton *> m_key_buttons;     //Keep track of all the key buttons to we can update all of them (m_currently_rebinding[KeyBindID] -> TiledButton *)
};


#endif //A_DETERMINER_OPTIONSMENU_H
