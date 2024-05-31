#include "OptionsMenu.h"

#include <cstddef>

#include "engine/state/StateManager.h"
#include "engine/widgets/Label.h"
#include "engine/widgets/TiledButton.h"
#include "engine/widgets/WidgetManager.h"
#include "utils/TiledBackground.h"
#include "GlobalResources.h"
#include "KeyBinds.h"
#include "KeyLookup.h"
#include "MainMenu.h"


OptionsMenu::OptionsMenu() : State() {
    m_currently_rebinding = -1;
    m_bg = new TiledBackground(&Res::menubg_options_sprite);
    m_widgets = new WidgetManager;

    Label *title = new Label(0, -200, 40, "Options");
    title->SetAlignment(WidgetAlignment_Center);
    title->SetOutline(true);
    m_widgets->AddWidget(title);

    KeyBindID keybind_ids[] = {KeyBind_Left, KeyBind_Right, KeyBind_Action};        //These are defined in KeyBinds.h
    const char *keybind_names[] = {"Left key :", "Right key :", "Action key :"};
    size_t keybinds_count = sizeof(keybind_ids) / sizeof(KeyBindID);

    int y = -50;
    for(size_t i = 0; i < keybinds_count; ++i) {
        KeyBindID key = keybind_ids[i];     //do this so the callback can catch it by value

        TiledButton *left_part = new TiledButton(-85, y, 140, 40, &Res::tiled_button_left_sprite, 8, 2, keybind_names[i]);
        left_part->SetAlignment(WidgetAlignment_Center);
        left_part->SetFontSize(20);
        left_part->CenterLabel();

        TiledButton *right_part = new TiledButton(70, y, 170, 40, &Res::tiled_button_right_sprite, 8, 2, g_key_lookup[KeyBinds::The()->GetKeyboardKey(keybind_ids[i])]);
        right_part->SetAlignment(WidgetAlignment_Center);
        right_part->SetFontSize(20);
        right_part->CenterLabel();

        std::function<void(void)> callback = [=]() { RequestRebinding(key); };

        left_part->SetCallback(callback);
        right_part->SetCallback(callback);

        m_widgets->AddWidget(left_part);
        m_widgets->AddWidget(right_part);
        m_key_buttons[key] = right_part;
        y += 50;
    }

    TiledButton *main_menu = new TiledButton(0, 200, 250, 40, &Res::tiled_button_sprite, 8, 2, "Return to main menu");
    main_menu->SetCallback([=]() { Manager()->SetState(new MainMenu, true); });
    main_menu->SetAlignment(WidgetAlignment_Center);
    main_menu->SetFontSize(20);
    main_menu->CenterLabel();
    m_widgets->AddWidget(main_menu);
}

OptionsMenu::~OptionsMenu() {
    delete m_widgets;
    delete m_bg;
}

void OptionsMenu::Update(float dt) {
    m_widgets->Update();
    m_bg->Update(dt);

    if(m_currently_rebinding != -1) {       //Check if we are rebinding
        for(int i = KEY_SPACE; i < KEY_PAUSE+1; ++i) {      //If so, check all the keys
            if(!IsKeyPressed(i)) continue;
            KeyBinds::The()->RegisterKey((KeyBindID)m_currently_rebinding, (KeyboardKey)i);
            m_currently_rebinding = -1;
            UpdateKeyButtonLabels();
            break;
        }
    }
}

void OptionsMenu::Draw() {
    m_bg->Draw();
    m_widgets->Draw();
}

void OptionsMenu::RequestRebinding(int keybind_id) {
    m_currently_rebinding = keybind_id;
    UpdateKeyButtonLabels();
}

void OptionsMenu::UpdateKeyButtonLabels() {
    for(auto [key, val] : m_key_buttons) {
        if(m_currently_rebinding == key) {
            val->SetLabel(">PRESS KEY<");
            val->SetLabelColor(YELLOW);
        }
        else {
            val->SetLabel(g_key_lookup[KeyBinds::The()->GetKeyboardKey((KeyBindID) key)]);
            val->SetLabelColor({255, 255, 255, 255});
        }
        val->CenterLabel();
    }
}
