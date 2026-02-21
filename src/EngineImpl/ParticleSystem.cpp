#include "ParticleSystem.h"
#include "EngineImpl.h"
#include "Transform.hpp"
#include <Math.hpp>
#include <cmath>

using namespace HG::Math;

namespace HGEngine {
namespace V1SDL {

ParticleSystem::ParticleSystem(const char* strName)
    : HG::HGComponent(strName)
    , m_fEmitTimer(0.0f)
    , m_fEmitAccumulator(0.0f)
    , m_nActiveCount(0)
    , m_pTexture(nullptr)
    , m_bIsPlaying(false)
    , m_bIsLooping(true)
    , m_bIsLocal(false)
{
    for (int i = 0; i < MAX_PARTICLES; i++) {
        m_Particles[i].active = false;
    }
}

ParticleSystem::~ParticleSystem() {
}

Texture* ParticleSystem::GetRenderTarget(Renderer2D* pRd) {
    (void)pRd;
    return nullptr;
}

void ParticleSystem::Update(float deltaTime) {
    if (!m_bIsPlaying) return;

    if (m_Config.emissionRate > 0) {
        float emitInterval = 1.0f / m_Config.emissionRate;
        m_fEmitTimer += deltaTime;
        
        while (m_fEmitTimer >= emitInterval) {
            Emit();
            m_fEmitTimer -= emitInterval;
        }
    }

    m_nActiveCount = 0;
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (m_Particles[i].active) {
            UpdateParticle(m_Particles[i], deltaTime);
            if (m_Particles[i].active) {
                m_nActiveCount++;
            }
        }
    }

    if (!m_bIsLooping && m_nActiveCount == 0 && m_fEmitAccumulator == 0.0f) {
        m_bIsPlaying = false;
    }
}

void ParticleSystem::Clear() {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        m_Particles[i].active = false;
    }
    m_nActiveCount = 0;
    m_fEmitTimer = 0.0f;
    m_fEmitAccumulator = 0.0f;
}

void ParticleSystem::Emit() {
    int idx = GetFreeParticle();
    if (idx < 0) return;

    Particle& p = m_Particles[idx];
    
    float angle = m_Config.emissionAngle + (rand() % (int)m_Config.emissionSpread - m_Config.emissionSpread / 2.0f);
    float angleRad = angle * 3.14159265359f / 180.0f;
    float speed = m_Config.minSpeed + (rand() % 100) / 100.0f * (m_Config.maxSpeed - m_Config.minSpeed);
    
    p.position = GetGameObject()->GetComponent<Transform>()->tPosition;
    p.velocity.X = cosf(angleRad) * speed;
    p.velocity.Y = sinf(angleRad) * speed;
    p.acceleration = m_Config.gravity;
    
    p.maxLifeTime = m_Config.minLifeTime + (rand() % 100) / 100.0f * (m_Config.maxLifeTime - m_Config.minLifeTime);
    p.lifeTime = p.maxLifeTime;
    
    p.size = m_Config.minSize + (rand() % 100) / 100.0f * (m_Config.maxSize - m_Config.minSize);
    p.rotation = 0.0f;
    p.rotationSpeed = m_Config.minRotationSpeed + (rand() % 100) / 100.0f * (m_Config.maxRotationSpeed - m_Config.minRotationSpeed);
    
    p.color = m_Config.startColor;
    p.alpha = 1.0f;
    p.active = true;
}

int ParticleSystem::GetFreeParticle() {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (!m_Particles[i].active) {
            return i;
        }
    }
    return -1;
}

void ParticleSystem::UpdateParticle(Particle& p, float deltaTime) {
    p.velocity.X += p.acceleration.X * deltaTime;
    p.velocity.Y += p.acceleration.Y * deltaTime;
    
    p.position.X += p.velocity.X * deltaTime;
    p.position.Y += p.velocity.Y * deltaTime;
    
    p.rotation += p.rotationSpeed * deltaTime;
    
    p.lifeTime -= deltaTime;
    if (p.lifeTime <= 0.0f) {
        p.active = false;
        return;
    }
    
    float lifeRatio = p.lifeTime / p.maxLifeTime;
    p.size = m_Config.minSize + (1.0f - lifeRatio) * (m_Config.maxSize - m_Config.minSize) * m_Config.sizeOverLifetime;
    
    float r = m_Config.startColor.R + (m_Config.endColor.R - m_Config.startColor.R) * (1.0f - lifeRatio);
    float g = m_Config.startColor.G + (m_Config.endColor.G - m_Config.startColor.G) * (1.0f - lifeRatio);
    float b = m_Config.startColor.B + (m_Config.endColor.B - m_Config.startColor.B) * (1.0f - lifeRatio);
    float a = m_Config.startColor.A + (m_Config.endColor.A - m_Config.startColor.A) * (1.0f - lifeRatio);
    
    p.color.R = (unsigned char)r;
    p.color.G = (unsigned char)g;
    p.color.B = (unsigned char)b;
    p.color.A = (unsigned char)a;
    p.alpha = a / 255.0f;
}

} // namespace V1SDL
} // namespace HGEngine
