#pragma once

#include "../../include/raylib.h"

#include <functional>
#include <unordered_map>
#include <string>
#include <optional>

struct TextureInfo {
    std::string identifier;
    unsigned int u, v;
};

class AssetManager {
    public:
        AssetManager() = default;
        AssetManager(AssetManager *parent);
        ~AssetManager();
        void AddSearchDir(std::string dir);
        std::optional<Texture2D> GetTexture(std::string identifier);
        std::optional<Sound> GetSound(std::string identifier);
    private:
        AssetManager *parent = nullptr;
        std::vector<std::string> searchDirs;
        std::unordered_map<std::string, Texture2D> loadedTextures;
        std::unordered_map<std::string, Sound> loadedSounds;
};