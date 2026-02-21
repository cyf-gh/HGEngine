#include "AssetManager.h"
#include "Font.hpp"
#include "Audio.h"
#include "Log.hpp"
#include "GameObject.h"

namespace HGEngine {
namespace V1SDL {

AssetManager::AssetManager()
    : m_nMaxCacheSize(256)
    , m_nCurrentMemoryUsage(0)
    , m_nTotalMemoryUsed(0)
{
}

AssetManager::~AssetManager() {
    UnloadAll();
}

void AssetManager::SetMaxCacheSize(int maxMemoryMB) {
    m_nMaxCacheSize = maxMemoryMB * 1024 * 1024;
}

Texture* AssetManager::LoadTexture(const char* name, const char* filePath) {
    std::lock_guard<std::mutex> lock(m_Mutex);
    
    std::string key = GetCacheKey(name, ResourceType::Texture);
    auto it = m_Cache.find(key);
    
    if (it != m_Cache.end() && it->second.bIsValid) {
        it->second.AddRef();
        m_nCurrentMemoryUsage += it->second.nMemorySize;
        HG_LOG_INFO("AssetManager: Using cached texture: %s", name);
        return static_cast<Texture*>(it->second.pResource);
    }
    
    Texture* pTexture = new Texture(name, filePath);
    
    if (pTexture->GetHandle() != nullptr) {
        int memSize = EstimateMemorySize(filePath, ResourceType::Texture);
        
        ResourceHandle handle(pTexture, ResourceType::Texture, filePath, memSize);
        m_Cache[key] = handle;
        m_LoadOrder.push_back(key);
        
        m_nCurrentMemoryUsage += memSize;
        m_nTotalMemoryUsed += memSize;
        
        HG_LOG_INFO("AssetManager: Loaded texture: %s (%.2f KB)", name, memSize / 1024.0f);
        
        if (m_nCurrentMemoryUsage > m_nMaxCacheSize) {
            UnloadUnused();
        }
        
        return pTexture;
    }
    
    delete pTexture;
    HG_LOG->Error("AssetManager: Failed to load texture: %s", filePath);
    return nullptr;
}

void* AssetManager::GetResource(const char* name) {
    std::lock_guard<std::mutex> lock(m_Mutex);
    
    auto it = m_Cache.find(name);
    if (it != m_Cache.end() && it->second.bIsValid) {
        it->second.AddRef();
        return it->second.pResource;
    }
    return nullptr;
}

void AssetManager::ReleaseResource(const char* name) {
    std::lock_guard<std::mutex> lock(m_Mutex);
    
    auto it = m_Cache.find(name);
    if (it != m_Cache.end()) {
        if (it->second.ReleaseRef()) {
            m_nCurrentMemoryUsage -= it->second.nMemorySize;
            HG_LOG_INFO("AssetManager: Released resource (no more refs): %s", name);
        }
    }
}

void AssetManager::UnloadResource(const char* name) {
    std::lock_guard<std::mutex> lock(m_Mutex);
    
    auto it = m_Cache.find(name);
    if (it != m_Cache.end()) {
        m_nCurrentMemoryUsage -= it->second.nMemorySize;
        
        if (it->second.pResource != nullptr) {
            delete it->second.pResource;
        }
        
        m_Cache.erase(it);
        
        for (auto it2 = m_LoadOrder.begin(); it2 != m_LoadOrder.end(); ++it2) {
            if (*it2 == name) {
                m_LoadOrder.erase(it2);
                break;
            }
        }
        
        HG_LOG_INFO("AssetManager: Unloaded resource: %s", name);
    }
}

void AssetManager::UnloadAll() {
    std::lock_guard<std::mutex> lock(m_Mutex);
    
    for (auto& pair : m_Cache) {
        if (pair.second.pResource != nullptr) {
            delete pair.second.pResource;
        }
    }
    
    m_Cache.clear();
    m_LoadOrder.clear();
    m_nCurrentMemoryUsage = 0;
    
    HG_LOG_INFO("AssetManager: Unloaded all resources");
}

void AssetManager::UnloadUnused() {
    std::lock_guard<std::mutex> lock(m_Mutex);
    
    std::vector<std::string> toUnload;
    
    for (auto& pair : m_Cache) {
        if (pair.second.nRefCount <= 0) {
            toUnload.push_back(pair.first);
        }
    }
    
    for (const std::string& name : toUnload) {
        auto it = m_Cache.find(name);
        if (it != m_Cache.end()) {
            m_nCurrentMemoryUsage -= it->second.nMemorySize;
            
            if (it->second.pResource != nullptr) {
                delete it->second.pResource;
            }
            
            m_Cache.erase(it);
            
            for (auto it2 = m_LoadOrder.begin(); it2 != m_LoadOrder.end(); ++it2) {
                if (*it2 == name) {
                    m_LoadOrder.erase(it2);
                    break;
                }
            }
        }
    }
    
    if (!toUnload.empty()) {
        HG_LOG_INFO("AssetManager: Unloaded %d unused resources", (int)toUnload.size());
    }
}

void AssetManager::UnloadOldest(int count) {
    std::lock_guard<std::mutex> lock(m_Mutex);
    
    int unloaded = 0;
    
    for (auto it = m_LoadOrder.begin(); it != m_LoadOrder.end() && unloaded < count; ++it) {
        auto cacheIt = m_Cache.find(*it);
        if (cacheIt != m_Cache.end() && cacheIt->second.nRefCount <= 0) {
            m_nCurrentMemoryUsage -= cacheIt->second.nMemorySize;
            
            if (cacheIt->second.pResource != nullptr) {
                delete cacheIt->second.pResource;
            }
            
            m_Cache.erase(cacheIt);
            unloaded++;
        }
    }
    
    for (int i = 0; i < unloaded && !m_LoadOrder.empty(); ) {
        std::string& name = m_LoadOrder.front();
        if (m_Cache.find(name) == m_Cache.end()) {
            m_LoadOrder.erase(m_LoadOrder.begin());
        } else {
            ++i;
        }
    }
    
    if (unloaded > 0) {
        HG_LOG_INFO("AssetManager: Unloaded %d oldest resources", unloaded);
    }
}

bool AssetManager::IsCached(const char* name) {
    std::lock_guard<std::mutex> lock(m_Mutex);
    return m_Cache.find(name) != m_Cache.end();
}

const char* AssetManager::GetResourcePath(const char* name) {
    std::lock_guard<std::mutex> lock(m_Mutex);
    
    auto it = m_Cache.find(name);
    if (it != m_Cache.end()) {
        return it->second.strFilePath.c_str();
    }
    return "";
}

int AssetManager::GetResourceRefCount(const char* name) {
    std::lock_guard<std::mutex> lock(m_Mutex);
    
    auto it = m_Cache.find(name);
    if (it != m_Cache.end()) {
        return it->second.nRefCount;
    }
    return 0;
}

void AssetManager::SetResourceMemorySize(const char* name, int sizeBytes) {
    std::lock_guard<std::mutex> lock(m_Mutex);
    
    auto it = m_Cache.find(name);
    if (it != m_Cache.end()) {
        int delta = sizeBytes - it->second.nMemorySize;
        it->second.nMemorySize = sizeBytes;
        m_nCurrentMemoryUsage += delta;
    }
}

void AssetManager::PreloadResources(const char** names, const char** paths, int count) {
    for (int i = 0; i < count; ++i) {
        if (!IsCached(names[i])) {
            LoadTexture(names[i], paths[i]);
        }
    }
    
    HG_LOG_INFO("AssetManager: Preloaded %d resources", count);
}

void AssetManager::LogCacheStats() const {
    HG_LOG_INFO("=== AssetManager Stats ===");
    HG_LOG_INFO("Cached Resources: %d", (int)m_Cache.size());
    HG_LOG_INFO("Memory Usage: %.2f MB", m_nCurrentMemoryUsage / 1024.0 / 1024.0f);
    HG_LOG_INFO("Total Memory Used: %.2f MB", m_nTotalMemoryUsage / 1024.0 / 1024.0f);
    HG_LOG_INFO("Max Cache Size: %.2f MB", m_nMaxCacheSize / 1024.0 / 1024.0f);
    
    for (const auto& pair : m_Cache) {
        HG_LOG_INFO("  - %s: %d refs, %.2f KB", 
            pair.first.c_str(), 
            pair.second.nRefCount, 
            pair.second.nMemorySize / 1024.0f);
    }
}

std::string AssetManager::GetCacheKey(const char* name, ResourceType type) const {
    std::string key = name;
    switch (type) {
        case ResourceType::Texture:
            key += "_tex";
            break;
        case ResourceType::Font:
            key += "_font";
            break;
        case ResourceType::Audio:
            key += "_audio";
            break;
        case ResourceType::Music:
            key += "_music";
            break;
        default:
            break;
    }
    return key;
}

int AssetManager::EstimateMemorySize(const char* filePath, ResourceType type) const {
    (void)filePath;
    (void)type;
    
    return 1024 * 1024;
}

void AssetManager::UpdateMemoryUsage(int delta) {
    m_nCurrentMemoryUsage += delta;
}

} // namespace V1SDL
} // namespace HGEngine
