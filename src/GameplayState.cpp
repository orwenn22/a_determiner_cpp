#include "GameplayState.h"

#include <raylib.h>
#include <stdio.h>

#include "engine/Globals.h"
#include "engine/metrics/Graphics.h"
#include "engine/metrics/MetricsCamera.h"
#include "engine/object/ObjectManager.h"
#include "engine/widgets/WidgetManager.h"
#include "engine/widgets/Widget.h"
#include "objects/actors/Player.h"
#include "Terrain.h"


//GameplayState *GameplayState::singleton = nullptr;


GameplayState::GameplayState() {
    m_camera = new MetricsCamera(0, 0, 20);
    m_cam_follow_mouse = false;
    m_cam_mouse_offset = {0.f, 0.f};

    m_terrain = Terrain::construct("res/wowmap.png", {32.f, 22.f});
    m_object_manager = new ObjectManager;

    m_current_player = -1;
    m_players_per_team = 3;

    m_action_widgets = new WidgetManager;
    m_show_action_widgets = false;
}

GameplayState::~GameplayState() {
    delete m_action_widgets;
    delete m_object_manager;
    delete m_terrain;
    delete m_camera;
}

void GameplayState::Update(float dt) {
    if(IsKeyDown(KEY_UP)) m_camera->origin_y += (int)100*dt;

    int mouse_x = GetMouseX();
    int mouse_y = GetMouseY();

    //TODO : update widgets here
    if(m_show_action_widgets) m_action_widgets->Update();

    HandleDragCamera((float)mouse_x, (float)mouse_y);

    Vector2 mouse_meters = m_camera->ConvertAbsoluteToMeters(mouse_x, mouse_y);
    //m_terrain->DestroyCircle(mouse_meters, 1.f);

    m_object_manager->Update(dt);
    if(PlacingPlayers()) PlacePlayer(mouse_meters, m_players.size()%2);

    //TODO : update hotbar text
}

void GameplayState::Draw() {
    Metrics::SetGraphicsCam(m_camera);
    Metrics::DrawGrid();
    m_terrain->Draw();
    m_object_manager->Draw();

    if(m_show_action_widgets) m_action_widgets->Draw();
}


void GameplayState::PlacePlayer(Vector2 pos, int team, bool check_start_pos) {
    //TODO : move this condition outside ?
    if(!IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseUsed()) return;

    Player *p = new Player(pos, team, this, 10);

    if(m_terrain->CheckCollisionRec(p->GetRectangle(), true)) {
        delete p;
        return;
    }

    //TODO : check collision with corresponding start location

    m_players.push_back(p);
    m_object_manager->AddObject(p);
    printf("GameplayState::PlacePlayer : Player spawned\n");    //TODO : display player pos

    if(m_players.size() >= m_players_per_team*2) {
        NextPlayerTurn();
    }
}

void GameplayState::NextPlayerTurn() {
    ++m_current_player;
    m_current_player %= (int) m_players.size();

    // We don't want to give the turn to a dead player
    while(m_players[m_current_player] == nullptr) {         //TODO : use "do {} while();" and remove previous lines ?
        ++m_current_player;
        m_current_player %= (int) m_players.size();
    }

    printf("GameplayState::NextPlayerTurn : player %i's turn\n", m_current_player);
    ShowActionWidgets();

    //TODO : spawn random items here
}

/**
 * Kill a player.
 * Player should be removed from the game only using this method, and nothing else.
 * @param p the player we want to kill
 */
void GameplayState::KillPlayer(Player *p) {
    int p_index = -1;
    for(int i = 0; i < m_players.size(); ++i) {
        if(m_players[i] == p) {
            p_index = i;
            break;
        }
    }

    if(p_index == -1) return;

    printf("GameplayState::KillPlayer : killing player %i\n", p_index);
    m_players[p_index] = nullptr;
    m_object_manager->DestroyObject(p);

    //TODO : check victory here

    if(p_index == m_current_player) NextPlayerTurn();   // This is in the case the current player died :(
}

void GameplayState::ShowActionWidgets() {
    m_action_widgets->Clear();
    m_show_action_widgets = true;
    int marge = 10;

    Player *current_player = GetCurrentPlayer();
    if(current_player == nullptr) return;

    auto widgets = current_player->GetActionWidgets();
    if(widgets.empty()) return;

    int widgets_width = 0;      // Get width of all widget combined

    for(Widget *w : widgets) widgets_width += w->Width();
    widgets_width += marge * (int)(widgets.size()-1);        // Add marge

    // Display all the widgets
    int x_pos = -widgets_width/2 + widgets[0]->Width()/2;
    for(int i = 0; i < widgets.size(); ++i) {
        widgets[i]->SetAlignment(WidgetAlignment_MiddleBottom);
        widgets[i]->SetPosition(x_pos, marge);
        m_action_widgets->AddWidget(widgets[i]);
        if(i != widgets.size()-1) x_pos += (widgets[i]->Width() + widgets[i+1]->Width())/2 + marge;
    }
}

void GameplayState::HideActionWidgets() {
    m_show_action_widgets = false;
    m_action_widgets->Clear();
}


Player *GameplayState::GetCurrentPlayer() {
    if(m_current_player < 0 || m_current_player >= m_players.size()) return nullptr;
    return m_players[m_current_player];
}



////////////////////////////////
//// PRIVATE

void GameplayState::HandleDragCamera(float mouse_x, float mouse_y) {
    if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && !IsMouseUsed()) {
        m_cam_follow_mouse = true;
        m_cam_mouse_offset = {m_camera->origin_x - mouse_x, m_camera->origin_y - mouse_y};
    }
    if(IsMouseButtonUp(MOUSE_BUTTON_RIGHT)) {
        m_cam_follow_mouse = false;
    }

    if(m_cam_follow_mouse) {
        m_camera->origin_x = m_cam_mouse_offset.x + mouse_x;
        m_camera->origin_y = m_cam_mouse_offset.y + mouse_y;
    }
    else {
        if(IsMouseUsed()) return;

        // Handle zoom
        float mouse_wheel = GetMouseWheelMove();
        if(mouse_wheel > 0) m_camera->SetPixelsPerMeter(m_camera->PixelsPerMeter() * 2);
        else if(mouse_wheel < 0) m_camera->SetPixelsPerMeter(m_camera->PixelsPerMeter() / 2);
    }
}
