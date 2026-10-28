/*
** EPITECH PROJECT, 2026
** RType
** File description:
** Audio.hpp
*/

#ifndef AUDIO_HPP
    #define AUDIO_HPP
    #include <array>
    #include <cstddef>
    #include <string>
    #include <vector>
    #include <raylib.h>

namespace Audio
{
    enum class Effect : std::size_t {
        Shoot,
        Count
    };

    constexpr std::size_t EffectCount = static_cast<std::size_t>(Effect::Count);
    constexpr std::size_t VoicesPerEffect = 4;
    constexpr std::array<const char *, EffectCount> EffectFiles = {"Shoot.wav"};
    constexpr const char *DefaultSoundsDirectory = "Assets/Sounds";
    constexpr const char *TitleMusic = "MenuScreen.mp3";
    constexpr const char *GameOverMusic = "GameOver.mp3";
    constexpr std::array<const char *, 1> StageMusics = {"AsWetAsAFish.mp3"};
    constexpr float MusicVolume = 0.5F;

    class AudioSystem {
        public:
            AudioSystem() = default;
            ~AudioSystem();
            AudioSystem(const AudioSystem &) = delete;
            AudioSystem &operator=(const AudioSystem &) = delete;
            bool Init(const std::string &soundsDirectory = DefaultSoundsDirectory);
            void Update();
            void SetMasterVolume(float volume);
            void SetEffectsVolume(float volume);
            void PlayTitle();
            void PlayStage(std::size_t stage);
            void PlayGameOver();
            void Play(Effect effect);
        private:
            struct Voices {
                Sound source{};
                std::vector<Sound> aliases;
                std::size_t next = 0;
            };
            void LoadEffect(std::size_t index);
            void UnloadMusic();
            void PlayMusic(const std::string &fileName, bool loop);
            bool ready = false;
            std::string dir;
            std::array<Voices, EffectCount> effects{};
            Music music{};
            std::string file;
            float effectsVolume = 1.0F;
    };
}

#endif
