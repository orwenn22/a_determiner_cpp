#include "LevMapLoader.h"

#include "engine/util/Paths.h"
#include "engine/Tileset.h"
#include "editor/EditorLevel.h"
#include "editor/layers/LayerSpawnRegions.h"
#include "editor/layers/LayerTilemap.h"
#include "terrain/TilemapTerrain.h"
#include "GameplayState.h"


LevMapLoader::LevMapLoader() : MapLoader() { }

LevMapLoader::~LevMapLoader() = default;

bool LevMapLoader::IsCorrectFormat(std::string path) {
    if(GetExtensionFromPath(path) != "lev") return false;

    //TODO : Do some more checks by looking in the file

    return true;
}

ErrorOr<GameplayState *> LevMapLoader::LoadMap(std::string path) {
    EditorLevel *editor_level = EditorLevel::Load(path);
    if(editor_level == nullptr) return Error("Failed to path the level (check console)");

    ErrorOr<GameplayState *> r = LoadMap(editor_level);
    delete editor_level;
    return r;
}

ErrorOr<GameplayState *> LevMapLoader::LoadMap(EditorLevel *editor_level) {
    if(editor_level == nullptr) return Error("editor_level is nullptr");

    //Get all the layers  TODO : check types ?
    LayerTilemap *collisions_layer = (LayerTilemap *) editor_level->GetLayer("Collisions");
    if(collisions_layer == nullptr) return Error("Missing layer : Collisions");

    LayerTilemap *tilemap_layer = (LayerTilemap *) editor_level->GetLayer("Tilemap");
    if(tilemap_layer == nullptr) return Error("Missing layer : Tilemap");

    LayerSpawnRegions *spawn_regions_layer = (LayerSpawnRegions *) editor_level->GetLayer("Spawn regions");
    if(spawn_regions_layer == nullptr) return Error("Missing layer : Spawn regions");

    //Get the size of the grid
    int grid_width = editor_level->GridWidth();
    int grid_height = editor_level->GridHeight();

    //Create a terrain with the correct size
    TilemapTerrain *terrain = new TilemapTerrain(
            {editor_level->GetTerrainWidth(), editor_level->GetTerrainHeight()},
            grid_width, grid_height);

    //Get the tileset of the terrain
    Tileset *tileset = tilemap_layer->GetTileset();
    if(tileset == nullptr || !tileset->Usable()) {
        delete terrain;
        return Error("Failed to get tileset of tilemap layer\n");
    }

    Tileset *tileset_clone = tileset->Clone();
    terrain->SetTileset(tileset_clone);

    //Fill the tilemap of the terrain and collisions
    for(int y = 0; y < grid_height; ++y) {
        for(int x = 0; x < grid_width; ++x) {
            terrain->SetTile(x, y, tilemap_layer->GetTile(x, y), collisions_layer->GetTile(x, y));
        }
    }

    //Create the GameplayState with the terrain
    GameplayState *gs = new GameplayState();
    gs->InitTerrain(terrain);

    //Load spawn regions
    for(int i = 0; i < spawn_regions_layer->GetSpawnRegionCount(); ++i) {
        EditorSpawnRegion *region = spawn_regions_layer->GetSpawnRegion(i);
        gs->InitSpawnRegion(region->TeamIndex(), {region->X(), region->Y(), region->Width(), region->Height()});
    }

    return gs;
}
