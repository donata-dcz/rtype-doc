/*
** EPITECH PROJECT, 2026
** RType
** File description:
** Audio.cpp
*/

#include <Client/Audio.hpp>
#include <algorithm>

namespace Audio
{
    AudioSystem::~AudioSystem()
    {
        if (!ready)
            return;
        UnloadMusic();
        for (Voices &voices : effects) {
            for (Sound &alias : voices.aliases)
                UnloadSoundAlias(alias);
            if (IsSoundValid(voices.source))
                UnloadSound(voices.source);
        }
        CloseAudioDevice();
    }

    void AudioSystem::LoadEffect(std::size_t index)
    {
        Voices &voices = effects[index];
        const std::string path = dir + "/" + EffectFiles[index];

        voices.source = LoadSound(path.c_str());
        if (!IsSoundValid(voices.source)) {
            TraceLog(LOG_WARNING, "AUDIO: cannot load the effect %s", path.c_str());
            return;
        }
        for (std::size_t voice = 1; voice < VoicesPerEffect; ++voice)
            voices.aliases.push_back(LoadSoundAlias(voices.source));
    }

    bool AudioSystem::Init(const std::string &soundsDirectory)
    {
        if (ready)
            return true;
        InitAudioDevice();
        if (!IsAudioDeviceReady()) {
            TraceLog(LOG_WARNING, "AUDIO: no audio device, the game will run without sound");
            return false;
        }
        ready = true;
        dir = soundsDirectory;
        for (std::size_t i = 0; i < EffectCount; ++i)
            LoadEffect(i);
        return true;
    }

    void AudioSystem::UnloadMusic()
    {
        if (file.empty())
            return;
        StopMusicStream(music);
        UnloadMusicStream(music);
        music = Music{};
        file.clear();
    }

    void AudioSystem::Update()
    {
        if (!file.empty())
            UpdateMusicStream(music);
    }

    void AudioSystem::SetMasterVolume(float volume)
    {
        if (ready)
            ::SetMasterVolume(std::clamp(volume, 0.0F, 1.0F));
    }

    void AudioSystem::SetEffectsVolume(float volume)
    {
        effectsVolume = std::clamp(volume, 0.0F, 1.0F);
    }

    void AudioSystem::PlayMusic(const std::string &fileName, bool loop)
    {
        const std::string path = dir + "/" + fileName;
    
        if (!ready || (file == fileName && IsMusicStreamPlaying(music)))
            return;
        UnloadMusic();
        music = LoadMusicStream(path.c_str());
        if (!IsMusicValid(music)) {
            TraceLog(LOG_WARNING, "AUDIO: cannot load the music %s", path.c_str());
            music = Music{};
            return;
        }
        music.looping = loop;
        ::SetMusicVolume(music, MusicVolume);
        PlayMusicStream(music);
        file = fileName;
    }

    void AudioSystem::PlayTitle()
    {
        PlayMusic(TitleMusic, true);
    }

    void AudioSystem::PlayStage(std::size_t stage)
    {
        PlayMusic(StageMusics[std::min(stage, StageMusics.size() - 1)], true);
    }

    void AudioSystem::PlayGameOver()
    {
        PlayMusic(GameOverMusic, false);
    }

    void AudioSystem::Play(Effect effect)
    {
        Voices &voices = effects[static_cast<std::size_t>(effect)];
        Sound &voice = (voices.next == 0) ? voices.source : voices.aliases[voices.next - 1];

        if (!ready || !IsSoundValid(voices.source))
            return;
        voices.next = (voices.next + 1) % VoicesPerEffect;
        SetSoundVolume(voice, effectsVolume);
        PlaySound(voice);
    }
}
