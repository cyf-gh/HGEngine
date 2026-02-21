//
// Created by cyf-m on 2020/12/10.
//

#ifndef HONEYGAME_RENDERER2D_H
#define HONEYGAME_RENDERER2D_H

#include <unordered_map>
#include <vector>
#include "Renderer.h"

namespace HGEngine {
namespace V1SDL {

class DrawableGeo;
class GameObject;
class Texture;

/// \brief Batch render entry
struct BatchEntry {
    Texture* texture;
    SDL_Rect srcRect;
    SDL_Rect dstRect;
    double angle;
    SDL_Point center;
    SDL_RendererFlip flip;
};

// \brief Render batch for batching draw calls
class RenderBatch {
private:
    std::vector<BatchEntry> m_Entries;
    Texture* m_pCurrentTexture;
    bool m_bDirty;

public:
    RenderBatch() : m_pCurrentTexture(nullptr), m_bDirty(false) {}

    /// \brief Begin a new batch with texture
    void Begin(Texture* texture) {
        if (m_pCurrentTexture != texture) {
            Flush();
            m_pCurrentTexture = texture;
        }
    }

    /// \brief Add entry to batch
    void Add(Texture* texture, const SDL_Rect& src, const SDL_Rect& dst, 
             double angle, const SDL_Point& center, SDL_RendererFlip flip) {
        if (texture != m_pCurrentTexture) {
            Flush();
            m_pCurrentTexture = texture;
        }
        BatchEntry entry = {texture, src, dst, angle, center, flip};
        m_Entries.push_back(entry);
    }

    /// \brief Flush current batch (called by Renderer2D)
    void Flush() {
        m_Entries.clear();
        m_pCurrentTexture = nullptr;
        m_bDirty = false;
    }

    /// \brief Get batch size
    size_t GetSize() const { return m_Entries.size(); }
    
    /// \brief Get current texture
    Texture* GetCurrentTexture() const { return m_pCurrentTexture; }
    
    /// \brief Get entries for rendering
    const std::vector<BatchEntry>& GetEntries() const { return m_Entries; }

    /// \brief Check if batch has entries
    bool HasEntries() const { return !m_Entries.empty(); }
};

class Renderer2D : public HGEngine::V1SDL::Renderer {
private:
    /// \brief Render batching
    RenderBatch m_Batch;
    bool m_bBatchEnabled;
    int m_nBatchCount;

public:
    /// \brief create a texture via file name, after you have
    SDL_Texture* CreateTextureFromFile( const char* pStrFileName );
    
    /// \brief Copy with batching support
    void Copy( const GameObject* pGameObject, const SDL_Rect* pSrcRect,const SDL_Rect* pDstRect );
    void CopyEx( const GameObject* pGameObject, const SDL_Rect* pSrcRect,const SDL_Rect* pDstRect, const double f64Angle, const SDL_Point* pCenter, const SDL_RendererFlip &tFlip );
    
    /// \brief Flush batched render calls
    void FlushBatch();
    
    /// \brief Enable/disable batching
    void SetBatchEnabled(bool enabled) { 
        if (!enabled && m_bBatchEnabled) {
            FlushBatch();
        }
        m_bBatchEnabled = enabled; 
    }
    
    /// \brief Get batch statistics
    int GetBatchCount() const { return m_nBatchCount; }

    void SetDrawColor( Uint8 r, Uint8 g, Uint8 b, Uint8 a );
    void SetDrawColor( const DrawableGeo* pdg );
    void SetDrawColor( const HG::Math::HGColor& tc );
    void SetDrawBlendMode( const SDL_BlendMode eBm = SDL_BLENDMODE_BLEND );
    int DrawLine( int x, int y, int xx, int yy );
    explicit Renderer2D(
        SDL_Window* pWindow,
        int index = -1,
        Uint32 flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC,
        const SDL_BlendMode eBm = SDL_BLENDMODE_BLEND )
        : Renderer( pWindow, index, flags )
        , m_bBatchEnabled(true)
        , m_nBatchCount(0)
    { 
        SetDrawBlendMode( eBm );
    };
    
    virtual ~Renderer2D() {
        FlushBatch();
    }
};
}
}

#endif //HONEYGAME_RENDERER2D_H
