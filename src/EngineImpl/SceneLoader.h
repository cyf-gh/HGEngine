#pragma once

#include <string>
#include <vector>
#include <map>

#include "../Engine/HGComponent.h"
#include "GameObject.h"
#include "Transform.hpp"
#include "Spirte.hpp"
#include "RigidBody.h"
#include "Physics.hpp"
#include "GUI/UIComponents.h"
#include "GUI/Layout.h"
#include "ParticleSystem.h"

namespace HGEngine {
namespace V1SDL {

struct SceneObjectConfig {
    std::string name;
    std::string prefab;
    float positionX;
    float positionY;
    float width;
    float height;
    std::string sprite;
    bool physics;
    bool isStatic;
    std::map<std::string, std::string> components;
    std::vector<SceneObjectConfig> children;
};

struct SceneConfig {
    std::string name;
    std::string background;
    int cameraX;
    int cameraY;
    int cameraWidth;
    int cameraHeight;
    std::vector<SceneObjectConfig> objects;
    std::vector<SceneObjectConfig> uiObjects;
};

class SceneLoader {
public:
    static SceneConfig LoadFromFile(const char* filePath);
    
    static void CreateSceneObjects(Scene* scene, const std::vector<SceneObjectConfig>& objects);
    
    static GameObject* CreateObjectFromConfig(Scene* scene, const SceneObjectConfig& config);
    
    static HG::HGComponent* CreateComponentFromConfig(const char* componentType);
    
    static void ApplyComponentConfig(HG::HGComponent* comp, const std::map<std::string, std::string>& props);
    
private:
    static std::string GetStringProp(const std::map<std::string, std::string>& props, const char* key, const char* defaultValue = "");
    static int GetIntProp(const std::map<std::string, std::string>& props, const char* key, int defaultValue = 0);
    static float GetFloatProp(const std::map<std::string, std::string>& props, const char* key, float defaultValue = 0.0f);
    static bool GetBoolProp(const std::map<std::string, std::string>& props, const char* key, bool defaultValue = false);
};

} // namespace V1SDL
} // namespace HGEngine
