#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <mutex>

#include "Asset.h"

namespace HGEngine {
namespace V1SDL {

/// \brief Resource type enumeration
enum class ResourceType {
    Unknown = 0,
    Texture = 1,
    Font = 2,
    Audio = 3,
    Music = 4,
    Script = 5,
    Scene = 6,
    Data = 7
};

/// \brief Resource handle with reference counting
struct ResourceHandle {
    void* pResource;
    ResourceType eType;
    std::string strFilePath;
    int nRefCount;
    int nMemorySize;
    bool bIsValid;

    ResourceHandle()
        : pResource(nullptr)
        , eType(ResourceType::Unknown)
        , strFilePath("")
        , nRefCount(0)
        , nMemorySize(0)
        , bIsValid(false)
    {}

    ResourceHandle(void* res, ResourceType type, const char* path, int memSize = 0)
        : pResource(res)
        , eType(type)
        , strFilePath(path)
        , nRefCount(1)
        , nMemorySize(memSize)
        , bIsValid(res != nullptr)
    {}

    void AddRef() { nRefCount++; }
    
    bool ReleaseRef() { 
        nRefCount--; 
        return nRefCount <= 0; 
    }
    
    int GetRefCount() const { return nRefCount; }
};

/// \brief Asset manager for resource pooling and caching
class AssetManager {
private:
    static AssetManager* s_Instance;

    std::unordered_map<std::string, ResourceHandle> m_Cache;
    std::vector<std::string> m_LoadOrder;
    
    std::mutex m_Mutex;
    
    int m_nMaxCacheSize;
    int m_nCurrentMemoryUsage;
    int m_nTotalMemoryUsed;

public:
    static AssetManager* GetInstance() {
        if (s_Instance == nullptr) {
            s_Instance = new AssetManager();
        }
        return s_Instance;
    }

    static void Destroy() {
        if (s_Instance != nullptr) {
            delete s_Instance;
            s_Instance = nullptr;
        }
    }

    AssetManager();
    ~AssetManager();

    void SetMaxCacheSize(int maxMemoryMB);

    template<typename T>
    T* LoadResource(const char* name, const char* filePath);

    template<typename T>
    T* GetResource(const char* name);

    template<typename T>
    void ReleaseResource(const char* name);

    void UnloadResource(const char* name);

    void UnloadAll();

    void UnloadUnused();

    void UnloadOldest(int count);

    bool IsCached(const char* name) const;

    int GetCacheCount() const { return (int)m_Cache.size(); }

    int GetMemoryUsage() const { return m_nCurrentMemoryUsage; }

    int GetResourceCount() const { return (int)m_Cache.size(); }

    const char* GetResourcePath(const char* name) const;

    int GetResourceRefCount(const char* name) const;

    void SetResourceMemorySize(const char* name, int sizeBytes);

    void PreloadResources(const char** names, const char** paths, int count);

    void LogCacheStats() const;

private:
    std::string GetCacheKey(const char* name, ResourceType type) const;

    int EstimateMemorySize(const char* filePath, ResourceType type) const;

    void UpdateMemoryUsage(int delta);
};

/// \brief Get global asset manager
inline AssetManager* GetAssetManager() {
    return AssetManager::GetInstance();
}

} // namespace V1SDL
} // namespace HGEngine
