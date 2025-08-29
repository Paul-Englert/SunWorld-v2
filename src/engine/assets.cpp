#include "assets.h"

#include "../io/debug.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <cctype>

Animation::Animation(Texture2D spriteAtlas, std::vector<Rectangle> frames, std::vector<int> frameLayout, int fps, AnimationType type) 
: atlas(spriteAtlas), frames(frames), frameLayout(frameLayout), type(type), timer(TickTimer(fps)) 
{}

TextureInfo Animation::TextureInfo() {

    if (timer.ShouldTick()) {
        if (type == AnimationType::LOOPING) {

            ++framePtr;
            if (framePtr >= frameLayout.size())
                framePtr = 0;

        } else if (type == AnimationType::BACK_AND_FORTH) {

            if (dir == BackAndForthDirection::INC) {

                ++framePtr;
                if (framePtr >= frameLayout.size()) {
                    framePtr -= 2;
                    dir = BackAndForthDirection::DEC;
                }

            } else if (dir == BackAndForthDirection::DEC) {

                --framePtr;
                if (framePtr < 0) {
                    framePtr = 1;
                    dir = BackAndForthDirection::INC;
                }

            }

        }
    }

    const Rectangle frame = frames[frameLayout[framePtr]];

    return {
        .texture      = atlas,
        .textureFrame = frame
    };

}

void Animation::Free() {

    UnloadTexture(atlas);

}

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

    for (auto it = loadedAnimations.begin(); it != loadedSounds.end(); ++it) {
        it->second->Free();
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

std::optional<std::string> AssetManager::ReadResourceFile(std::string identifier) {

    if (parent != nullptr) {

        std::optional<std::string> parentRet = parent->ReadResourceFile(identifier);
        if (parentRet.has_value()) {
            return parentRet;
        }

    }

    std::vector<std::string> paths;
    paths.emplace_back(identifier);
    for (std::string searchDir : searchDirs) {

        std::string str = searchDir;
        str += identifier;
        paths.emplace_back(str);

    }

    for (std::string path : paths) {

        if (!std::filesystem::exists(path)) {
            Debug::Log(Debug::LogLevel::DEBUG, "path does not exist: %s", path.c_str());
            continue;
        }

        std::ifstream in(path);
        if (!in.is_open()) {
            Debug::Log(Debug::LogLevel::ERROR, "Could not open file %s.", path.c_str());
            return std::nullopt;
        }
        std::ostringstream out;

        out << in.rdbuf();

        return out.str();

    }

    return std::nullopt;

}

std::optional<Animation*> AssetManager::GetAnimation(std::string identifier) {

    if (parent != nullptr) {

        std::optional<Animation*> parentRet = parent->GetAnimation(identifier);
        if (parentRet.has_value()){
            return parentRet;
        }

    }

    if (loadedAnimations.contains(identifier)) {
        return loadedAnimations[identifier];
    }

    std::optional<std::string> opt = ReadResourceFile(identifier);

    if (!opt.has_value()) {
        Debug::Log(Debug::LogLevel::ERROR, "Could not load animation because file reading failed.");
        return std::nullopt;
    }

    Dictionary aniFileContent = ParseDictionary(opt.value(), "\n", "->");

    std::vector<std::string> base64Textures;
    base64Textures.reserve(ANIMATION_MAX_FRAMES);
    base64Textures.resize(ANIMATION_MAX_FRAMES);
    int frameCount;
    int fps;
    AnimationType type;
    std::vector<int> frameLayout;

    for (auto it = aniFileContent.begin(); it != aniFileContent.end(); ++it) {

        std::string key = it->first;
        std::string value = it->second;

        Debug::Debug("key is %s", key.c_str());

        if (IsPositiveInt(key)) {
            int index = std::stoi(key);
            base64Textures[index] = value;
        } else if (key.compare("typ") == 0 || key.compare("type") == 0) {

            if (value.compare("back/forth") == 0) {
                type = AnimationType::BACK_AND_FORTH;
            } else if (value.compare("looping") == 0) {
                type = AnimationType::LOOPING;
            } else {
                Debug::Log(Debug::LogLevel::ERROR, "Could not parse .ani file: Unknown animation type '%s'.", value.c_str());
                return std::nullopt;
            }

        } else if (key.compare("fps") == 0) {

            fps = std::stoi(value);

        } else if (key.compare("frames") == 0) {

            frameLayout = ParsePositiveIntList(value, ",");

        }

    }
    frameCount = frameLayout.size();

    std::vector<Texture2D> loadedTextures;
    for (int index = 0; index < base64Textures.size(); ++index) {
        std::string data = base64Textures[index];

        if (data.empty()) continue;

        const std::string prefix = "data:image/png;base64,";
        if (data.rfind(prefix, 0) == 0) {
            data = data.substr(prefix.size());
        }

        std::vector<unsigned char> decoded = Base64Decode(data);

        Image image = LoadImageFromMemory(".png", decoded.data(), decoded.size());

        Texture2D texture = LoadTextureFromImage(image);
        loadedTextures.push_back(texture);

        UnloadImage(image);
    }

    std::vector<Rectangle> frames;
    Texture2D spriteAtlas = BuildSpriteAtlas(loadedTextures, &frames);

    Animation *animation = animationAllocator.Alloc<Animation>();
    *animation = Animation(spriteAtlas, frames, frameLayout, fps, type);

    return animation;

}

FontRenderer::FontRenderer(std::string fontDir) {

    fontAssetManager.AddSearchDir(fontDir);

}

Texture2D FontRenderer::GetCharTexture(char c) {

    //Buchstabentexturen werden nochmal extra gecached (obwohl der AssetManager das auch schon tut), um die Ermittlung des korrekten Texturenpfades nicht bei jedem Aufruf durchlaufen zu müssen.
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

Dictionary ParseDictionary(std::string str, std::string entryDelimiter, std::string kvDelimiter) {

    Dictionary out;

    size_t newLinePos = -1;
    while ((newLinePos = str.find(entryDelimiter)) != std::string::npos) {
        std::string entry = str.substr(0, newLinePos);

        size_t delimiterPos = entry.find(kvDelimiter);
        std::string key = entry.substr(0, delimiterPos);
        std::string value = entry.substr(delimiterPos+kvDelimiter.size(), entry.size());

        out[key] = value;

        str.erase(0, newLinePos + entryDelimiter.size());
    }

    if (!str.empty()) {
        size_t delimiterPos = str.find(kvDelimiter);
        if (delimiterPos != std::string::npos) {
            std::string key = str.substr(0, delimiterPos);
            std::string value = str.substr(delimiterPos + kvDelimiter.size());
            out[key] = value;
        }
    }

    return out;
}

bool IsPositiveInt(const std::string& str) {

    for (size_t i = 0; i < str.length(); ++i) {

        if (!std::isdigit(str[i])) 
            return false;

    }

    return true;

}

std::vector<int> ParsePositiveIntList(std::string str, std::string delimiter) {

    std::vector<int> out;

    size_t delimiterPos = -1;
    while ((delimiterPos = str.find(delimiter)) != std::string::npos) {
        std::string num = str.substr(0, delimiterPos);

        if (!IsPositiveInt(num)) {
            Debug::Log(Debug::LogLevel::WARNING, "While trying to parse positive int list, entry '%s' was not a positive integer. Skipping entry.", str.c_str());
            continue;
        }

        out.emplace_back(std::stoi(num));

        str.erase(0, delimiterPos + delimiter.size());
    }

    return out;

}

//aus dem internet gekalut
//hoffentlich funktioniert das auch
std::vector<unsigned char> Base64Decode(const std::string& in) {
    static const std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    std::vector<int> T(256, -1);
    for (int i = 0; i < 64; i++) T[chars[i]] = i;

    std::vector<unsigned char> out;
    out.reserve(in.size() * 3 / 4);

    int val = 0, valb = -8;
    for (unsigned char c : in) {
        if (T[c] == -1) break;
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0) {
            out.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return out;
}

Texture2D BuildSpriteAtlas(const std::vector<Texture2D>& textures, std::vector<Rectangle> *frameInfoOutput) {

    const int frameCount = (int)textures.size();
    const int frameWidth = textures[0].width;
    const int frameHeight = textures[0].height;

    RenderTexture2D target = LoadRenderTexture(frameCount * frameWidth, frameHeight);
    BeginTextureMode(target);
    ClearBackground(BLANK);

    for (int i = 0; i < frameCount; i++) {
        DrawTexture(textures[i], i * frameWidth, 0, WHITE);
    }

    EndTextureMode();

    Texture2D atlas = target.texture;
    Image atlasImg = LoadImageFromTexture(target.texture);
    atlas = LoadTextureFromImage(atlasImg);
    UnloadImage(atlasImg);
    UnloadRenderTexture(target);

    frameInfoOutput->resize(frameCount);
    for (int i = 0; i < frameCount; i++) {
        (*frameInfoOutput)[i] = { (float)(i * frameWidth), 0.0f, (float)frameWidth, (float)frameHeight };
    }

    return atlas;
}