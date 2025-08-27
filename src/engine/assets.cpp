#include "assets.h"

#include "../io/debug.h"

#include <filesystem>

AssetManager::AssetManager(AssetManager *parent) {
    this->parent = parent;
}

AssetManager::~AssetManager() {

    for (auto it = loadedTextures.begin(); it != loadedTextures.end(); ++it) {
        UnloadTexture(it->second);
    }

    for (auto it = loadedSounds.begin(); it != loadedSounds.end(); ++it) {
        UnloadSound(it->second);
    }

}

void AssetManager::AddSearchDir(std::string dir) {
    searchDirs.emplace_back(dir);
}

std::optional<Texture2D> AssetManager::GetTexture(std::string identifier) {

    if (parent != nullptr) {

        std::optional<Texture2D> parentRet = parent->GetTexture(identifier);
        if (parentRet.has_value()) {
            return parentRet;
        }

    }

    if (loadedTextures.contains(identifier)) {
        return loadedTextures[identifier];
    }

    std::vector<std::string> paths;
    paths.emplace_back(identifier);
    for (std::string searchDir : searchDirs) {

        std::string str = searchDir;
        str += identifier;
        paths.emplace_back(str);

    }

    Image image{};
    for (std::string path : paths) {

        if (!std::filesystem::exists(path)) {
            Debug::Log(Debug::LogLevel::DEBUG, "path does not exist: %s", path.c_str());
            continue;
        }

        image = LoadImage(path.c_str());

        if (image.data != nullptr) {

            Texture2D texture = LoadTextureFromImage(image);
            loadedTextures[identifier] = texture;
            UnloadImage(image);
            return texture;

        }

    }

    return std::nullopt;

}

std::optional<Sound> AssetManager::GetSound(std::string identifier) {

    if (parent != nullptr) {

        std::optional<Sound> parentRet = parent->GetSound(identifier);
        if (parentRet.has_value()) {
            return parentRet;
        }

    }

    if (loadedSounds.contains(identifier)) {
        return loadedSounds[identifier];
    }

    std::vector<std::string> paths;
    paths.emplace_back(identifier);
    for (std::string searchDir : searchDirs) {

        std::string str = searchDir;
        str += identifier;
        paths.emplace_back(str);

    }

    Sound sound{};
    for (std::string path : paths) {

        if (!std::filesystem::exists(path)) {
            Debug::Log(Debug::LogLevel::DEBUG, "path does not exist: %s", path.c_str());
            continue;
        }

        sound = LoadSound(path.c_str());
        if (sound.stream.buffer != nullptr) {

            loadedSounds[identifier] = sound;
            return sound;

        }

    }

    return std::nullopt;

}