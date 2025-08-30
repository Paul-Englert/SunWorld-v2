#pragma once

#include "../../include/raylib.h"

#include "timer.h"
#include "allocator.h"

#include <functional>
#include <unordered_map>
#include <string>
#include <optional>
#include <queue>

enum class AnimationType {

    LOOPING, BACK_AND_FORTH

};

enum class BackAndForthDirection {
    INC, DEC
};

inline constexpr size_t ANIMATION_MAX_FRAMES = 32;

//Eine Animation braucht mindestens 2 Frames, ansonsten passieren komische Sachen (array out of bounds).
class Animation final {
    public:
        Animation(Texture2D spriteAtlas, std::vector<Rectangle> frames, std::vector<int> frameLayout, int fps, AnimationType type);
        void Free();
    private:
        Texture2D atlas;
        std::vector<Rectangle> frames;
        std::vector<int> frameLayout;
        int framePtr = 0;

        AnimationType type;
        //wird nur benutzt wenn type == BACK_AND_FORTH
        BackAndForthDirection dir = BackAndForthDirection::INC;
        TickTimer timer;
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
        /**
         * Liest eine Datei ein und gibt das Ergebnis als String zurück.
         */
        std::optional<std::string> ReadResourceFile(std::string identifier);
        /**
         * Lädt eine Animation und gibt einen Zeiger zu dieser zurück.
         * Alle Animationen werden innerhalb des AssetManagers gespeichert; Interaktion erfolgt nur über Zeiger damit der State der Animationen konsistent bleibt.
         */
        std::optional<Animation*> GetAnimation(std::string identifier);
    private:
        std::optional<Texture2D> _GetTexture(std::string identifier);
        std::optional<Sound> _GetSound(std::string identifier);
        AssetManager *parent = nullptr;
        std::vector<std::string> searchDirs;
        std::unordered_map<std::string, Texture2D> loadedTextures;
        std::unordered_map<std::string, Sound> loadedSounds;
        std::unordered_map<std::string, Animation*> loadedAnimations;
        ArenaAllocator<sizeof(Animation)*64> animationAllocator;
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

struct SoundQueueEntry {
    bool isSilence;
    int silenceMillis;
    Sound sound;
    bool looping;
    bool fadeIn;
    int fadeInMillis;
};

enum class SoundQueueState {

    EMPTY,
    SILENCE,
    PLAYING,
    FADING_IN,
    FADING_OUT

};

class SoundQueue {
    public:
        SoundQueue();
        ~SoundQueue() = default;
        void QueueSilence(int silenceMillis);
        void Queue(Sound sound);
        void QueueFadeIn(Sound sound, int fadeInMillis);
        void QueueLooping(Sound sound);
        void QueueLoopingFadeIn(Sound sound, int fadeInMillis);
        void SkipToNext();
        void FadeOutAndSkipToNext(int fadeOutMillis);
        void Update();
        bool IsEmpty();
        void Clear();
    private:
        void BeginPlaying();
        std::queue<SoundQueueEntry> queuedSounds;
        SoundQueueState state = SoundQueueState::EMPTY;
        int fadeMillis = -1;
        TickTimer silenceTimer;
        TickTimer fadeTimer;
};

using Dictionary = std::unordered_map<std::string, std::string>;

Dictionary ParseDictionary(std::string stringToParse, std::string entryDelimiter, std::string keyValueDelimiter);

bool IsPositiveInt(const std::string& str);

std::vector<int> ParsePositiveIntList(std::string stringToParse, std::string delimiter);

std::vector<unsigned char> Base64Decode(const std::string& in);

Texture2D BuildSpriteAtlas(const std::vector<Texture2D>& textures, std::vector<Rectangle> *frameInfoOutput);