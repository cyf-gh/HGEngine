#pragma once

#include <vector>
#include <cstddef>

#include "../Engine/HGComponent.h"
#include "Renderer2D.h"
#include "Transform.hpp"

namespace HGEngine {
namespace V1SDL {

/// \brief Particle data structure
struct Particle {
    HG::Math::HGVec2<float> position;
    HG::Math::HGVec2<float> velocity;
    HG::Math::HGVec2<float> acceleration;
    float lifeTime;
    float maxLifeTime;
    float size;
    float rotation;
    float rotationSpeed;
    HG::Math::HGColor color;
    float alpha;
    bool active;
    
    Particle() 
        : position(), velocity(), acceleration()
        , lifeTime(0.0f), maxLifeTime(1.0f)
        , size(1.0f), rotation(0.0f), rotationSpeed(0.0f)
        , color(255, 255, 255, 255), alpha(1.0f), active(false) {}
};

/// \brief Particle emitter configuration
struct EmitterConfig {
    // Emission
    int emissionRate;          // Particles per second
    float emissionAngle;       // Angle in degrees
    float emissionSpread;       // Spread angle in degrees
    float minSpeed;
    float maxSpeed;
    
    // Lifetime
    float minLifeTime;
    float maxLifeTime;
    
    // Appearance
    float minSize;
    float maxSize;
    float sizeOverLifetime;    // Size multiplier at end of life
    
    // Color
    HG::Math::HGColor startColor;
    HG::Math::HGColor endColor;
    
    // Physics
    HG::Math::HGVec2<float> gravity;
    float minRotationSpeed;
    float maxRotationSpeed;
    
    EmitterConfig()
        : emissionRate(10)
        , emissionAngle(0.0f)
        , emissionSpread(360.0f)
        , minSpeed(10.0f)
        , maxSpeed(50.0f)
        , minLifeTime(1.0f)
        , maxLifeTime(2.0f)
        , minSize(1.0f)
        , maxSize(4.0f)
        , sizeOverLifetime(0.5f)
        , startColor(255, 255, 255, 255)
        , endColor(255, 255, 255, 0)
        , gravity(0.0f, 0.0f)
        , minRotationSpeed(-180.0f)
        , maxRotationSpeed(180.0f) {}
};

/// \brief Particle system component
/// \note Renders particles efficiently using a single texture
class ParticleSystem : public HG::HGComponent {
private:
    static const int MAX_PARTICLES = 1000;
    
    Particle m_Particles[MAX_PARTICLES];
    EmitterConfig m_Config;
    
    float m_fEmitTimer;
    float m_fEmitAccumulator;
    int m_nActiveCount;
    
    SDL_Texture* m_pTexture;
    bool m_bIsPlaying;
    bool m_bIsLooping;
    bool m_bIsLocal;

public:
    HG_COMPONENT_RENDERABLE
    
    ParticleSystem(const char* strName);
    virtual ~ParticleSystem();
    
    /// \brief Get render target for rendering
    Texture* GetRenderTarget(Renderer2D* pRd) override;
    
    /// \brief Set particle texture
    void SetTexture(SDL_Texture* texture) { m_pTexture = texture; }
    
    /// \brief Set emitter configuration
    void SetConfig(const EmitterConfig& config) { m_Config = config; }
    
    /// \brief Get emitter configuration
    const EmitterConfig& GetConfig() const { return m_Config; }
    
    /// \brief Play particle system
    void Play() { m_bIsPlaying = true; m_fEmitAccumulator = 0.0f; }
    
    /// \brief Stop particle system
    void Stop() { m_bIsPlaying = false; }
    
    /// \brief Pause particle system
    void Pause() { m_bIsPlaying = false; }
    
    /// \brief Set looping
    void SetLooping(bool looping) { m_bIsLooping = looping; }
    
    /// \brief Check if playing
    bool IsPlaying() const { return m_bIsPlaying; }
    
    /// \brief Check if looping
    bool IsLooping() const { return m_bIsLooping; }
    
    /// \brief Get active particle count
    int GetActiveCount() const { return m_nActiveCount; }
    
    /// \brief Get max particle count
    int GetMaxCount() const { return MAX_PARTICLES; }
    
    /// \brief Set local emission (particles move with emitter) or world
    void SetLocal(bool local) { m_bIsLocal = local; }
    
    /// \brief Check if local
    bool IsLocal() const { return m_bIsLocal; }
    
    /// \brief Update particles
    void Update(float deltaTime);
    
    /// \brief Clear all particles
    void Clear();
    
    /// \brief Get component type name
    const char* GetTypeName() const override { return "ParticleSystem"; }
    
    /// \brief Get component type id
    size_t GetTypeId() const override { return typeid(ParticleSystem).hash_code(); }

private:
    /// \brief Emit a new particle
    void Emit();
    
    /// \brief Get free particle index
    int GetFreeParticle();
    
    /// \brief Update single particle
    void UpdateParticle(Particle& p, float deltaTime);
};

} // namespace V1SDL
} // namespace HGEngine
