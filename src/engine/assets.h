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
        /**
         * Fügt einen Suchordner hinzu. Asset-Suche ist nicht rekursiv, Unterordner müssen explizit hinzugefügt werden.
         */
        void AddSearchDir(std::string dir);
        /**
         * Findet eine Textur. Wenn diese nicht geladen ist wird sie anhand des Parameters und der angegebenen Suchordner geladen.
         */
        std::optional<Texture2D> GetTexture(std::string identifier);
        /**
         * Findet einen Sound. Wenn dieser nicht geladen ist wird er anhand des Parameters und der angegebenen Suchordner geladen.
         */
        std::optional<Sound> GetSound(std::string identifier);
        /**
         * Entlädt einen spezifischen Sound. Muss nicht zwingend verwendet werden, da bei der Zerstörung des AssetManagers alle Sounds entladen werden.
         */
        void FreeTexture(std::string identifier);
        /**
         * Entlädt eine spezifische Textur. Muss nicht zwingend verwendet werden, da bei der Zerstörung des AssetManagers alle Texturen entladen werden.
         */
        void FreeSound(std::string identifier);
        /**
         * Lädt ein Bild aus den Suchordnern. Das Image muss vom Caller entladen werden.
         */
        Image LoadRawImage(std::string identifier);
        /**
         * Erstellt eine Textur aus einem Bild. Kann verwendet werden, um über LoadRawImage() ein Bild zu laden, es zu verändern, und dann als Textur hochzuladen.
         */
        Texture2D UploadCustomTexture(std::string identifier, Image image);
    private:
        std::optional<Texture2D> _GetTexture(std::string identifier);
        std::optional<Sound> _GetSound(std::string identifier);
        AssetManager *parent = nullptr;
        std::vector<std::string> searchDirs;
        std::unordered_map<std::string, Texture2D> loadedTextures;
        std::unordered_map<std::string, Sound> loadedSounds;
};

class FontRenderer {
    public:
        FontRenderer(std::string fontDir);
        ~FontRenderer() = default;
        /**
         * Zeichnet einen String, eventuell mit Scale Faktor.
         */
        void DrawString(std::string str, Vector2 position, float scaleFactor = 1.0f);
        /**
         * Gibt Höhe und Breite dieses Strings zurück, eventuell unter Berücksichtigung eines Scale Faktors.
         */
        Vector2 MeasureString(std::string str, float scaleFactor = 1.0f);
        /**
         * Zeichnet einen String und gibt dann Höhe und Breite, eventuell unter Berücksichtigung eines Scale Faktors, zurück.
         */
        Vector2 DrawStringAndMeasure(std::string str, Vector2 position, float scaleFactor = 1.0f);
    private:
        Texture2D GetCharTexture(char c);
        AssetManager fontAssetManager;
        std::unordered_map<char, Texture2D> cachedTextures;
        static constexpr inline float spaceWidth = 16;
};