#include "EditorImportTilesetWindow.h"

#include "editor/layers/LayerTilemap.h"
#include "engine/widgets/Button.h"
#include "engine/widgets/IntField.h"
#include "engine/widgets/Label.h"
#include "engine/widgets/WidgetManager.h"
#include "engine/windows/WindowManager.h"
#include "engine/Tileset.h"

EditorImportTilesetWindow::EditorImportTilesetWindow(LayerTilemap *layer, std::string file_path) : Window(50, 50, 255, 200) {
    m_layer = layer;
    m_file_path = file_path;        //don't std::move it

    m_tile_width_px = 8;
    m_tile_height_px = 8;

    Label *tile_size_label = new Label(5, 5, 10, "Tile size (px) :");
    IntField *tile_width_filed = new IntField(150, 5, 100, 10, &m_tile_width_px);
    IntField *tile_height_filed = new IntField(150, 28, 100, 10, &m_tile_height_px);

    Button *confirm_button = new Button(5, 50, 150, 20, "Confirm", [this]() {
        //TODO : check if the layer still exists ?
        if(m_tile_width_px <= 0 || m_tile_height_px <= 0) return;       //TODO : display error / close ?

        Texture texture = LoadTexture(m_file_path.c_str());

        //TODO : display error / close ?
        if(texture.id == 0) return;
        if(m_tile_width_px > texture.width || m_tile_height_px > texture.height) return;

        m_layer->SetTileset(new Tileset(&texture, m_tile_width_px, m_tile_height_px, true));
        GetWindowManager()->CloseWindowByPtr(this);
    });
    confirm_button->CenterLabel();

    GetWidgetManager()->AddWidget(tile_size_label);
    GetWidgetManager()->AddWidget(tile_width_filed);
    GetWidgetManager()->AddWidget(tile_height_filed);
    GetWidgetManager()->AddWidget(confirm_button);
}

EditorImportTilesetWindow::~EditorImportTilesetWindow() = default;
