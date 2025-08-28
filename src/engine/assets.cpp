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

std::optional<Texture2D> AssetManager::_GetTexture(std::string identifier) {

    if (parent != nullptr) {

        std::optional<Texture2D> parentRet = parent->_GetTexture(identifier);
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

std::optional<Texture2D> AssetManager::GetTexture(std::string identifier) {

    const std::optional<Texture2D> ret = _GetTexture(identifier);

    if (Debug::Config::LOG_MISSING_ASSETS && !ret.has_value()) {

        Debug::Log(Debug::LogLevel::WARNING, "Missing texture: %s", identifier.c_str());

    }

    return ret;

}

std::optional<Sound> AssetManager::_GetSound(std::string identifier) {

    if (parent != nullptr) {

        std::optional<Sound> parentRet = parent->_GetSound(identifier);
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

std::optional<Sound> AssetManager::GetSound(std::string identifier) {

    const std::optional<Sound> ret = _GetSound(identifier);

    if (Debug::Config::LOG_MISSING_ASSETS && !ret.has_value()) {

        Debug::Log(Debug::LogLevel::WARNING, "Missing sound: %s", identifier.c_str());

    }

    return ret;

}

void AssetManager::FreeTexture(std::string identifier) {

    if (loadedTextures.contains(identifier)) {

        UnloadTexture(loadedTextures[identifier]);
        loadedTextures.erase(identifier);

    }

}

void AssetManager::FreeSound(std::string identifier) {

    if (loadedSounds.contains(identifier)) {

        UnloadSound(loadedSounds[identifier]);
        loadedSounds.erase(identifier);

    }

}

Image AssetManager::LoadRawImage(std::string identifier) {

    const Image image = LoadImage(identifier.c_str());

    if (image.data == nullptr) {

        if (Debug::Config::LOG_MISSING_ASSETS) {

            Debug::Log(Debug::LogLevel::WARNING, "Could not load image from path: %s", identifier.c_str());

        }

    }

    return image;

}

Texture2D AssetManager::UploadCustomTexture(std::string identifier, Image image) {

    if (loadedTextures.contains(identifier)) {

        Debug::Log(Debug::LogLevel::WARNING, "A texture is already loaded for identifier %s. Overriding existing texture.", identifier.c_str());
        UnloadTexture(loadedTextures[identifier]);

    }

    const Texture2D texture = LoadTextureFromImage(image);
    loadedTextures[identifier] = texture;

    return texture;

}

FontRenderer::FontRenderer(std::string fontDir) {

    fontAssetManager.AddSearchDir(fontDir);

}

Texture2D FontRenderer::GetCharTexture(char c) {

    if (cachedTextures.contains(c)) {

        return cachedTextures[c];

    }

    std::optional<Texture2D> texture = std::nullopt;

    //Für manche Buchstaben brauchen wir besonders Handling. Standardverhalten: Großbuchstabe + ".png" -> Dateiname innerhalb des Font-Ordners.
    switch (c) {
        case ':': {
            texture = fontAssetManager.GetTexture("colon.png");
        } break;
        case '.': {
            texture = fontAssetManager.GetTexture("period.png");
        } break;
        case '?': {
            texture = fontAssetManager.GetTexture("questionmark.png");
        } break;
        case '/': {
            texture = fontAssetManager.GetTexture("slash.png");
        } break;
        default: {
            c = std::toupper(c);
            std::string str = "";
            str += c;
            str += ".png";

            texture = fontAssetManager.GetTexture(str);
        } break;
        
    }

    if (texture.has_value()) {

        cachedTextures[c] = texture.value();
        return texture.value();

    } else {

        Debug::Log(Debug::LogLevel::ERROR, "Font Renderer: Could not find texture for character '%c'", c);
        return Texture2D{};

    }

}

void FontRenderer::DrawString(std::string str, Vector2 position, float scaleFactor) {

    const char *text = str.c_str();
    size_t index = 0;

    const float y = position.y;
    float x = position.x;

    while (text[index] != '\0') {

        if (text[index] == ' ') {

            x += (spaceWidth * scaleFactor);
            ++index;
            continue;

        }

        const Texture2D texture = GetCharTexture(text[index]);

        //fehlerhaft geladene texturen überspringen
        if (texture.id == 0) {
            ++index;
            continue;
        }

        const Rectangle sourceRec{0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height)};
        const Rectangle destRec{x, y, static_cast<float>(texture.width * scaleFactor), static_cast<float>(texture.height * scaleFactor)};
        constexpr Vector2 origin{0, 0};
        constexpr float rotation{0};
        constexpr Color NO_TINT = WHITE;

        DrawTexturePro(texture, sourceRec, destRec, origin, rotation, NO_TINT);

        x += (texture.width * scaleFactor);
        ++index;

    }

}

Vector2 FontRenderer::MeasureString(std::string str, float scaleFactor) {

    const char *text = str.c_str();
    size_t index = 0;

    float width = 0, height = 0;

    while (text[index] != '\0') {

        if (text[index] == ' ') {

            width += (spaceWidth * scaleFactor);
            ++index;
            continue;

        }

        const Texture2D texture = GetCharTexture(text[index]);

        //fehlerhaft geladene texturen überspringen
        if (texture.id == 0) {
            ++index;
            continue;
        }

        const float textureHeight = texture.height * scaleFactor;
        if (textureHeight > height) {
            height = textureHeight;
        }

        width += (texture.width * scaleFactor);
        ++index;

    }

    return {width, height};

}

Vector2 FontRenderer::DrawStringAndMeasure(std::string str, Vector2 position, float scaleFactor) {

    const char *text = str.c_str();
    size_t index = 0;

    const float y = position.y;
    float x = position.x;

    float height = 0;

    while (text[index] != '\0') {

        if (text[index] == ' ') {

            x += (spaceWidth * scaleFactor);
            ++index;
            continue;

        }

        const Texture2D texture = GetCharTexture(text[index]);

        //fehlerhaft geladene texturen überspringen
        if (texture.id == 0) {
            ++index;
            continue;
        }

        const float textureHeight = texture.height * scaleFactor;
        if (textureHeight > height) {
            height = textureHeight;
        }

        const Rectangle sourceRec{0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height)};
        const Rectangle destRec{x, y, static_cast<float>(texture.width * scaleFactor), static_cast<float>(texture.height * scaleFactor)};
        constexpr Vector2 origin{0, 0};
        constexpr float rotation{0};
        constexpr Color NO_TINT = WHITE;

        DrawTexturePro(texture, sourceRec, destRec, origin, rotation, NO_TINT);

        x += (texture.width * scaleFactor);
        ++index;

    }

    return {x, height};

}