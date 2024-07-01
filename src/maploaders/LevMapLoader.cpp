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

    //Get all the layers  TODO : check types ?
    LayerTilemap *collisions_layer = (LayerTilemap *) editor_level->GetLayer("Collisions");
    if(collisions_layer == nullptr) return Error("Missing layer : Collisions");

    LayerTilemap *tilemap_layer = (LayerTilemap *) editor_level->GetLayer("Tilemap");
    if(tilemap_layer == nullptr) return Error("Missing layer : Tilemap");

    LayerSpawnRegions *spawn_regions_layer = (LayerSpawnRegions *) editor_level->GetLayer("Spawn regions");
    if(spawn_regions_layer == nullptr) return Error("Missing layer : Spawn regions");

    int grid_width = editor_level->GridWidth();
    int grid_height = editor_level->GridHeight();

    TilemapTerrain *terrain = new TilemapTerrain(
            {editor_level->GetTerrainWidth(), editor_level->GetTerrainHeight()},
            grid_width, grid_height);

    Tileset *tileset = tilemap_layer->GetTileset()->StrongClone();      //Ownership transfer
    if(tileset == nullptr) {
        delete editor_level;
        delete terrain;
        return Error("Failed to get tileset");
    }
    terrain->SetTileset(tileset);

    for(int y = 0; y < grid_height; ++y) {
        for(int x = 0; x < grid_width; ++x) {
            terrain->SetTile(x, y, tilemap_layer->GetTile(x, y), collisions_layer->GetTile(x, y));
        }
    }

    GameplayState *gs = new GameplayState();
    gs->InitTerrain(terrain);

    for(int i = 0; i < spawn_regions_layer->GetSpawnRegionCount(); ++i) {
        EditorSpawnRegion *region = spawn_regions_layer->GetSpawnRegion(i);
        gs->InitSpawnRegion(region->TeamIndex(), {region->X(), region->Y(), region->Width(), region->Height()});
    }

    delete editor_level;
    return gs;
}
