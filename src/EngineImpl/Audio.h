#pragma once

#ifdef FUCKMSVC

#include <string>
#include <vector>
#include <map>

#ifdef _WIN32
#include <SDL_mixer.h>
#else
#include <SDL_mixer/SDL_mixer.h>
#endif

#include "../Engine/HGComponent.h"

namespace HGEngine {
namespace V1SDL {

/// \brief Sound channel handle
typedef int SoundChannel;

/// \brief Sound effect
class Sound {
private:
    Mix_Chunk* m_pChunk;
    std::string m_strName;
    int m_nVolume;

public:
    Sound(const char* name, Mix_Chunk* chunk);
    ~Sound();

    /// \brief Get sound name
    const char* GetName() const { return m_strName.c_str(); }

    /// \brief Get chunk
    Mix_Chunk* GetChunk() const { return m_pChunk; }

    /// \brief Set volume (0-128)
    void SetVolume(int volume);

    /// \brief Get volume
    int GetVolume() const { return m_nVolume; }

    /// \brief Check if valid
    bool IsValid() const { return m_pChunk != nullptr; }
};

/// \brief Music track
class Music {
private:
    Mix_Music* m_pMusic;
    std::string m_strName;
    int m_nVolume;

public:
    Music(const char* name, Mix_Music* music);
    ~Music();

    /// \brief Get music name
    const char* GetName() const { return m_strName.c_str(); }

    /// \brief Get music
    Mix_Music* GetMusic() const { return m_pMusic; }

    /// \brief Set volume (0-128)
    void SetVolume(int volume);

    /// \brief Get volume
    int GetVolume() const { return m_nVolume; }

    /// \brief Check if valid
    bool IsValid() const { return m_pMusic != nullptr; }
};

/// \brief Audio system
class AudioSystem {
private:
    static AudioSystem* s_Instance;
    
    std::map<std::string, Sound*> m_Sounds;
    std::map<std::string, Music*> m_Music;
    
    int m_nMasterVolume;
    int m_nSoundVolume;
    int m_nMusicVolume;
    
    bool m_bMuted;

public:
    static AudioSystem* GetInstance() {
        if (s_Instance == nullptr) {
            s_Instance = new AudioSystem();
        }
        return s_Instance;
    }
    
    static void Destroy() {
        if (s_Instance != nullptr) {
            delete s_Instance;
            s_Instance = nullptr;
        }
    }

    AudioSystem();
    ~AudioSystem();

    /// \brief Initialize audio system
    bool Initialize(int frequency = 44100, int chunksize = 2048);

    /// \brief Shutdown audio system
    void Shutdown();

    /// \brief Load sound effect from file
    Sound* LoadSound(const char* name, const char* filePath);

    /// \brief Load music from file
    Music* LoadMusic(const char* name, const char* filePath);

    /// \brief Play sound effect
    SoundChannel PlaySound(const char* soundName, int loops = 0, int volume = -1);

    /// \brief Play sound effect with channel
    SoundChannel PlaySoundChannel(const char* soundName, int channel, int loops = 0, int volume = -1);

    /// \brief Play music
    void PlayMusic(const char* musicName, int loops = -1);

    /// \brief Pause music
    void PauseMusic();

    /// \brief Resume music
    void ResumeMusic();

    /// \brief Stop music
    void StopMusic();

    /// \brief Pause all sounds
    void PauseAllSounds();

    /// \brief Resume all sounds
    void ResumeAllSounds();

    /// \brief Stop all sounds
    void StopAllSounds();

    /// \brief Set master volume
    void SetMasterVolume(int volume);

    /// \brief Set sound volume
    void SetSoundVolume(int volume);

    /// \brief Set music volume
    void SetMusicVolume(int volume);

    /// \brief Get master volume
    int GetMasterVolume() const { return m_nMasterVolume; }

    /// \brief Get sound volume
    int GetSoundVolume() const { return m_nSoundVolume; }

    /// \brief Get music volume
    int GetMusicVolume() const { return m_nMusicVolume; }

    /// \brief Mute/unmute
    void SetMuted(bool muted);
    bool IsMuted() const { return m_bMuted; }

    /// \brief Find sound by name
    Sound* FindSound(const char* name) const;

    /// \brief Find music by name
    Music* FindMusic(const char* name) const;

    /// \brief Free sound
    void FreeSound(const char* name);

    /// \brief Free music
    void FreeMusic(const char* name);

    /// \brief Free all sounds
    void FreeAllSounds();

    /// \brief Free all music
    void FreeAllMusic();
};

/// \brief Audio component for GameObjects
class AudioSource : public HG::HGComponent {
private:
    Sound* m_pSound;
    Music* m_pMusic;
    bool m_bAutoPlay;
    bool m_bLoop;
    int m_nVolume;

public:
    AudioSource(const char* name);
    virtual ~AudioSource();

    /// \brief Set sound to play
    void SetSound(const char* soundName);

    /// \brief Set music to play
    void SetMusic(const char* musicName);

    /// \brief Set auto play
    void SetAutoPlay(bool autoPlay) { m_bAutoPlay = autoPlay; }

    /// \brief Set loop
    void SetLoop(bool loop) { m_bLoop = loop; }

    /// \brief Set volume
    void SetVolume(int volume) { m_nVolume = volume; }

    /// \brief Get volume
    int GetVolume() const { return m_nVolume; }

    /// \brief Play
    void Play();

    /// \brief Stop
    void Stop();

    /// \brief Get component type name
    const char* GetTypeName() const override { return "AudioSource"; }

    /// \brief Get component type id
    size_t GetTypeId() const override { return typeid(AudioSource).hash_code(); }
};

} // namespace V1SDL
} // namespace HGEngine

#endif