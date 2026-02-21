#pragma once

#include <vector>
#include <unordered_map>
#include <cstddef>

#include "GameObject.h"

namespace HGEngine {
namespace V1SDL {

/// \brief Grid-based spatial partition for collision detection
/// \note Simple and cache-friendly, suitable for tile-based games
class SpatialGrid {
private:
    /// \brief Cell size for grid
    int m_nCellSize;
    
    /// \brief Grid dimensions
    int m_nGridWidth;
    int m_nGridHeight;
    
    /// \brief Grid cells: maps cell coordinate to list of GameObjects
    std::unordered_map<int, std::vector<GameObject*>> m_Cells;
    
    /// \brief Get cell index from world position
    inline int GetCellIndex(int x, int y) const {
        int cellX = x / m_nCellSize;
        int cellY = y / m_nCellSize;
        return cellY * m_nGridWidth + cellX;
    }

public:
    /// \brief Constructor
    /// \param cellSize Size of each cell in pixels
    /// \param worldWidth World width in pixels
    /// \param worldHeight World height in pixels
    SpatialGrid(int cellSize, int worldWidth, int worldHeight)
        : m_nCellSize(cellSize)
        , m_nGridWidth((worldWidth + cellSize - 1) / cellSize)
        , m_nGridHeight((worldHeight + cellSize - 1) / cellSize)
    {
    }

    /// \brief Destructor
    ~SpatialGrid() {
    }

    /// \brief Clear all objects from grid
    void Clear() {
        m_Cells.clear();
    }

    /// \brief Insert object into grid
    void Insert(GameObject* pObj) {
        if (pObj == nullptr) return;
        
        auto pTransform = pObj->GetComponent<Transform>();
        if (pTransform == nullptr) return;
        
        int x = (int)pTransform->tPosition.X;
        int y = (int)pTransform->tPosition.Y;
        
        // Add to all cells that the object overlaps
        int startCellX = x / m_nCellSize;
        int startCellY = y / m_nCellSize;
        int endCellX = (x + (int)pTransform->tRect.W) / m_nCellSize;
        int endCellY = (y + (int)pTransform->tRect.H) / m_nCellSize;
        
        for (int cy = startCellY; cy <= endCellY; cy++) {
            for (int cx = startCellX; cx <= endCellX; cx++) {
                int cellIdx = cy * m_nGridWidth + cx;
                m_Cells[cellIdx].push_back(pObj);
            }
        }
    }

    /// \brief Remove object from grid
    void Remove(GameObject* pObj) {
        if (pObj == nullptr) return;
        
        // Remove from all cells (inefficient but simple)
        for (auto& pair : m_Cells) {
            auto& vec = pair.second;
            for (auto it = vec.begin(); it != vec.end(); ) {
                if (*it == pObj) {
                    it = vec.erase(it);
                } else {
                    ++it;
                }
            }
        }
    }

    /// \brief Get potential collision candidates for an object
    /// \param pObj GameObject to find candidates for
    /// \param output Output vector to store candidates
    void GetCandidates(GameObject* pObj, std::vector<GameObject*>& output) const {
        if (pObj == nullptr) return;
        
        auto pTransform = pObj->GetComponent<Transform>();
        if (pTransform == nullptr) return;
        
        int x = (int)pTransform->tPosition.X;
        int y = (int)pTransform->tPosition.Y;
        
        int startCellX = x / m_nCellSize;
        int startCellY = y / m_nCellSize;
        int endCellX = (x + (int)pTransform->tRect.W) / m_nCellSize;
        int endCellY = (y + (int)pTransform->tRect.H) / m_nCellSize;
        
        // Use a simple visited check to avoid duplicates
        // In production, you'd use a better data structure
        for (int cy = startCellY; cy <= endCellY; cy++) {
            for (int cx = startCellX; cx <= endCellX; cx++) {
                int cellIdx = cy * m_nGridWidth + cx;
                auto it = m_Cells.find(cellIdx);
                if (it != m_Cells.end()) {
                    for (GameObject* candidate : it->second) {
                        if (candidate != pObj) {
                            // Check if already added
                            bool found = false;
                            for (size_t i = 0; i < output.size(); i++) {
                                if (output[i] == candidate) {
                                    found = true;
                                    break;
                                }
                            }
                            if (!found) {
                                output.push_back(candidate);
                            }
                        }
                    }
                }
            }
        }
    }

    /// \brief Rebuild entire grid from scene objects
    void Rebuild(const std::vector<HG::HGComponent*>& components) {
        Clear();
        for (HG::HGComponent* pComp : components) {
            GameObject* pObj = pComp->GetGameObject();
            if (pObj != nullptr && pObj->IsEnable()) {
                Insert(pObj);
            }
        }
    }

    /// \brief Get grid statistics
    int GetCellCount() const { return (int)m_Cells.size(); }
    int GetCellSize() const { return m_nCellSize; }
};

} // namespace V1SDL
} // namespace HGEngine
