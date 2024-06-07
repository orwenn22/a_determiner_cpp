#include "LegacyMapLoader.h"

#include <cstring>
#include <vector>

#include "engine/metrics/MetricsCamera.h"
#include "engine/util/Trace.h"
#include "GameplayState.h"
#include "Terrain.h"


LegacyMapLoader::LegacyMapLoader() = default;
LegacyMapLoader::~LegacyMapLoader() = default;

bool LegacyMapLoader::IsCorrectFormat(std::string path) {
    if(path.length() < 5) return false;
    if(path.substr(path.length()-4, 4) != ".leg") return false;
    FILE *in_file = fopen(path.c_str(), "r");
    if(in_file == nullptr) return false;

    char buf[7] = {0};
    fgets(buf, 7, in_file);
    fclose(in_file);

    return (strcmp(buf, "legacy") == 0);
}

GameplayState *LegacyMapLoader::LoadMap(std::string path) {
    FILE *in_file = fopen(path.c_str(), "r");
    if(in_file == nullptr) return nullptr;

    GameplayState *result = new GameplayState;
    bool terrain_initialised = false;

    char buf[512] = {0};
    while(!feof(in_file)) {
        //Get current line
        buf[0] = 0;
        fgets(buf, 512, in_file);
        size_t line_len = strlen(buf);
        if(buf[line_len-1] == '\n') {
            buf[line_len - 1] = 0;
            --line_len;
        }
        if(line_len == 0) continue;

        //Cut the line into tokens
        std::vector<std::string> tokens;
        std::string current_token = "";
        for(size_t i = 0; i < line_len; ++i) {
            if(buf[i] == ' ') {
                tokens.push_back(current_token);
                current_token = "";
                continue;
            }
            current_token.push_back(buf[i]);
        }
        if(!current_token.empty()) tokens.push_back(current_token);

        //Proceed to token identification
        if(tokens[0] == "camera_center") {
            TRACE("TODO : deprecate camera_center\n");
            if(tokens.size() != 3) {
                TRACE("camera_center needs 3 tokens\n");
                continue;
            }
            float pos_x = std::stof(tokens[1]);
            float pos_y = std::stof(tokens[2]);
            result->GetCamera()->SetCameraCenter({pos_x, pos_y});
        }
        else if(tokens[0] == "bitmap") {
            if(tokens.size() != 4) {
                TRACE("bitmap needs 4 tokens\n");
                continue;
            }
            float w = std::stof(tokens[2]);
            float h = std::stof(tokens[3]);
            Terrain *t = Terrain::construct(tokens[1].c_str(), {w, h});
            result->InitTerrain(t);
            if(t != nullptr) {
                TRACE("Loading terrain from bitmap %s failed\n", tokens[1].c_str());
                terrain_initialised = true;
            }
        }
        else if(tokens[0] == "team_start") {
            if(tokens.size() != 6) {
                TRACE("team_start needs 6 tokens\n");
                continue;
            }
            int team = std::stoi(tokens[1]);
            float x = std::stof(tokens[2]);
            float y = std::stof(tokens[3]);
            float w = std::stof(tokens[4]);
            float h = std::stof(tokens[5]);
            result->InitSpawnRegion(team, {x, y, w, h});
        }
        else {
            TRACE("Ignored token %s\n", tokens[0].c_str());
        }
    }

    fclose(in_file);
    if(terrain_initialised) return result;

    delete result;
    return nullptr;
}
