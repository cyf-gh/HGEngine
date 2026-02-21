#include "Audio.h"

namespace HGEngine {
namespace V1SDL {

AudioSystem* AudioSystem::s_Instance = nullptr;

Sound::Sound(const char* name, Mix_Chunk* chunk)
    : m_pChunk(chunk)
    , m_strName(name)
    , m_nVolume(128)
{
}

Sound::~Sound() {
    if (m_pChunk != nullptr) {
        Mix_FreeChunk(m_pChunk);
        m_pChunk = nullptr;
    }
}

void Sound::SetVolume(int volume) {
    m_nVolume = volume;
    if (m_pChunk != nullptr) {
        Mix_VolumeChunk(m_pChunk, volume);
    }
}

Music::Music(const char* name, Mix_Music* music)
    : m_pMusic(music)
    , m_strName(name)
    , m_nVolume(128)
{
}

Music::~Music() {
    if (m_pMusic != nullptr) {
        Mix_FreeMusic(m_pMusic);
        m_pMusic = nullptr;
    }
}

void Music::SetVolume(int volume) {
    m_nVolume = volume;
    Mix_VolumeMusic(volume);
}

AudioSystem::AudioSystem()
    : m_nMasterVolume(128)
    , m_nSoundVolume(128)
    , m_nMusicVolume(128)
    , m_bMuted(false)
{
}

AudioSystem::~AudioSystem() {
    Shutdown();
}

bool AudioSystem::Initialize(int frequency, int chunksize) {
    if (Mix_OpenAudio(frequency, AUDIO_S16SYS, 2, 4096) < 0) {
        return false;
    }
    
    Mix_AllocateChannels(16);
    Mix_Volume(-1, m_nSoundVolume);
    
    return true;
}

void AudioSystem::Shutdown() {
    FreeAllSounds();
    FreeAllMusic();
    Mix_CloseAudio();
}

Sound* AudioSystem::LoadSound(const char* name, const char* filePath) {
    if (m_Sounds.find(name) != m_Sounds.end()) {
        return m_Sounds[name];
    }
    
    Mix_Chunk* chunk = Mix_LoadWAV(filePath);
    if (chunk == nullptr) {
        return nullptr;
    }
    
    Sound* sound = new Sound(name, chunk);
    sound->SetVolume(m_nSoundVolume);
    m_Sounds[name] = sound;
    
    return sound;
}

Music* AudioSystem::LoadMusic(const char* name, const char* filePath) {
    if (m_Music.find(name) != m_Music.end()) {
        return m_Music[name];
    }
    
    Mix_Music* music = Mix_LoadMUS(filePath);
    if (music == nullptr) {
        return nullptr;
    }
    
    Music* mus = new Music(name, music);
    mus->SetVolume(m_nMusicVolume);
    m_Music[name] = mus;
    
    return mus;
}

SoundChannel AudioSystem::PlaySound(const char* soundName, int loops, int volume) {
    Sound* sound = FindSound(soundName);
    if (sound == nullptr || !sound->IsValid()) {
        return -1;
    }
    
    if (volume >= 0) {
        Mix_Volume(-1, volume);
    }
    
    return Mix_PlayChannel(-1, sound->GetChunk(), loops);
}

SoundChannel AudioSystem::PlaySoundChannel(const char* soundName, int channel, int loops, int volume) {
    Sound* sound = FindSound(soundName);
    if (sound == nullptr || !sound->IsValid()) {
        return -1;
    }
    
    if (volume >= 0) {
        Mix_Volume(channel, volume);
    }
    
    return Mix_PlayChannel(channel, sound->GetChunk(), loops);
}

void AudioSystem::PlayMusic(const char* musicName, int loops) {
    Music* music = FindMusic(musicName);
    if (music == nullptr || !music->IsValid()) {
        return;
    }
    
    Mix_PlayMusic(music->GetMusic(), loops);
}

void AudioSystem::PauseMusic() {
    Mix_PauseMusic();
}

void AudioSystem::ResumeMusic() {
    Mix_ResumeMusic();
}

void AudioSystem::StopMusic() {
    Mix_HaltMusic();
}

void AudioSystem::PauseAllSounds() {
    Mix_Pause(-1);
}

void AudioSystem::ResumeAllSounds() {
    Mix_Resume(-1);
}

void AudioSystem::StopAllSounds() {
    Mix_HaltChannel(-1);
}

void AudioSystem::SetMasterVolume(int volume) {
    m_nMasterVolume = volume;
    Mix_Volume(-1, m_nSoundVolume);
    Mix_VolumeMusic(m_nMusicVolume);
}

void AudioSystem::SetSoundVolume(int volume) {
    m_nSoundVolume = volume;
    Mix_Volume(-1, volume);
}

void AudioSystem::SetMusicVolume(int volume) {
    m_nMusicVolume = volume;
    Mix_VolumeMusic(volume);
}

void AudioSystem::SetMuted(bool muted) {
    m_bMuted = muted;
    if (muted) {
        Mix_Volume(-1, 0);
        Mix_VolumeMusic(0);
    } else {
        Mix_Volume(-1, m_nSoundVolume);
        Mix_VolumeMusic(m_nMusicVolume);
    }
}

Sound* AudioSystem::FindSound(const char* name) const {
    auto it = m_Sounds.find(name);
    if (it != m_Sounds.end()) {
        return it->second;
    }
    return nullptr;
}

Music* AudioSystem::FindMusic(const char* name) const {
    auto it = m_Music.find(name);
    if (it != m_Music.end()) {
        return it->second;
    }
    return nullptr;
}

void AudioSystem::FreeSound(const char* name) {
    auto it = m_Sounds.find(name);
    if (it != m_Sounds.end()) {
        delete it->second;
        m_Sounds.erase(it);
    }
}

void AudioSystem::FreeMusic(const char* name) {
    auto it = m_Music.find(name);
    if (it != m_Music.end()) {
        delete it->second;
        m_Music.erase(it);
    }
}

void AudioSystem::FreeAllSounds() {
    for (auto& pair : m_Sounds) {
        delete pair.second;
    }
    m_Sounds.clear();
}

void AudioSystem::FreeAllMusic() {
    for (auto& pair : m_Music) {
        delete pair.second;
    }
    m_Music.clear();
}

AudioSource::AudioSource(const char* name)
    : HG::HGComponent(name)
    , m_pSound(nullptr)
    , m_pMusic(nullptr)
    , m_bAutoPlay(false)
    , m_bLoop(false)
    , m_nVolume(128)
{
}

AudioSource::~AudioSource() {
}

void AudioSource::SetSound(const char* soundName) {
    m_pSound = AudioSystem::GetInstance()->FindSound(soundName);
    m_pMusic = nullptr;
}

void AudioSource::SetMusic(const char* musicName) {
    m_pMusic = AudioSystem::GetInstance()->FindMusic(musicName);
    m_pSound = nullptr;
}

void AudioSource::Play() {
    if (m_pSound != nullptr && m_pSound->IsValid()) {
        int loops = m_bLoop ? 0 : 0;
        AudioSystem::GetInstance()->PlaySound(m_pSound->GetName(), loops, m_nVolume);
    } else if (m_pMusic != nullptr && m_pMusic->IsValid()) {
        int loops = m_bLoop ? -1 : 0;
        AudioSystem::GetInstance()->PlayMusic(m_pMusic->GetName(), loops);
    }
}

void AudioSource::Stop() {
    if (m_pMusic != nullptr) {
        AudioSystem::GetInstance()->StopMusic();
    }
}

} // namespace V1SDL
} // namespace HGEngine
