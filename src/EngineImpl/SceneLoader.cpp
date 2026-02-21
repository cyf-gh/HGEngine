#include "SceneLoader.h"
#include "Scene.h"
#include "EngineImpl.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace HGEngine {
namespace V1SDL {

static std::string ReadFileToString(const char* filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        HG_LOG->Error("SceneLoader: Failed to open file: %s", filePath);
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

static void ParseJsonObject(const std::string& json, SceneObjectConfig& config, int depth = 0);

static std::string GetJsonValue(const std::string& json, const char* key) {
    std::string keyStr = "\"" + std::string(key) + "\"";
    size_t pos = json.find(keyStr);
    if (pos == std::string::npos) return "";
    
    pos = json.find(":", pos);
    if (pos == std::string::npos) return "";
    
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n')) pos++;
    
    if (pos >= json.size()) return "";
    
    if (json[pos] == '"') {
        pos++;
        size_t end = json.find('"', pos);
        if (end == std::string::npos) return "";
        return json.substr(pos, end - pos);
    }
    
    size_t end = pos;
    while (end < json.size() && json[end] != ',' && json[end] != '}' && json[end] != '\n') end++;
    return json.substr(pos, end - pos);
}

static int GetJsonValueInt(const std::string& json, const char* key, int defaultVal) {
    std::string val = GetJsonValue(json, key);
    if (val.empty()) return defaultVal;
    try {
        return std::stoi(val);
    } catch (...) {
        return defaultVal;
    }
}

static float GetJsonValueFloat(const std::string& json, const char* key, float defaultVal) {
    std::string val = GetJsonValue(json, key);
    if (val.empty()) return defaultVal;
    try {
        return std::stof(val);
    } catch (...) {
        return defaultVal;
    }
}

static bool GetJsonValueBool(const std::string& json, const char* key, bool defaultVal) {
    std::string val = GetJsonValue(json, key);
    if (val.empty()) return defaultVal;
    return val == "true" || val == "1";
}

SceneConfig SceneLoader::LoadFromFile(const char* filePath) {
    SceneConfig config;
    config.name = "";
    config.cameraX = 0;
    config.cameraY = 0;
    config.cameraWidth = 800;
    config.cameraHeight = 600;
    
    std::string json = ReadFileToString(filePath);
    if (json.empty()) {
        return config;
    }
    
    config.name = GetJsonValue(json, "name");
    config.background = GetJsonValue(json, "background");
    config.cameraX = GetJsonValueInt(json, "cameraX", 0);
    config.cameraY = GetJsonValueInt(json, "cameraY", 0);
    config.cameraWidth = GetJsonValueInt(json, "cameraWidth", 800);
    config.cameraHeight = GetJsonValueInt(json, "cameraHeight", 600);
    
    size_t objectsStart = json.find("\"objects\"");
    if (objectsStart != std::string::npos) {
        size_t braceStart = json.find("[", objectsStart);
        size_t braceEnd = json.find("]", braceStart);
        if (braceStart != std::string::npos && braceEnd != std::string::npos) {
            std::string objectsJson = json.substr(braceStart + 1, braceEnd - braceStart - 1);
            
            size_t objStart = 0;
            while ((objStart = objectsJson.find("{", objStart)) != std::string::npos) {
                int depth = 1;
                size_t objEnd = objStart + 1;
                while (objEnd < objectsJson.size() && depth > 0) {
                    if (objectsJson[objEnd] == '{') depth++;
                    else if (objectsJson[objEnd] == '}') depth--;
                    objEnd++;
                }
                
                std::string objJson = objectsJson.substr(objStart, objEnd - objStart);
                SceneObjectConfig objConfig;
                ParseJsonObject(objJson, objConfig, 0);
                config.objects.push_back(objConfig);
                
                objStart = objEnd;
            }
        }
    }
    
    size_t uiStart = json.find("\"ui\"");
    if (uiStart != std::string::npos) {
        size_t braceStart = json.find("[", uiStart);
        size_t braceEnd = json.find("]", braceStart);
        if (braceStart != std::string::npos && braceEnd != std::string::npos) {
            std::string uiJson = json.substr(braceStart + 1, braceEnd - braceStart - 1);
            
            size_t objStart = 0;
            while ((objStart = uiJson.find("{", objStart)) != std::string::npos) {
                int depth = 1;
                size_t objEnd = objStart + 1;
                while (objEnd < uiJson.size() && depth > 0) {
                    if (uiJson[objEnd] == '{') depth++;
                    else if (uiJson[objEnd] == '}') depth--;
                    objEnd++;
                }
                
                std::string objJson = uiJson.substr(objStart, objEnd - objStart);
                SceneObjectConfig objConfig;
                ParseJsonObject(objJson, objConfig, 0);
                config.uiObjects.push_back(objConfig);
                
                objStart = objEnd;
            }
        }
    }
    
    return config;
}

static void ParseJsonObject(const std::string& json, SceneObjectConfig& config, int depth) {
    if (depth > 5) return;
    
    config.name = GetJsonValue(json, "name");
    config.prefab = GetJsonValue(json, "prefab");
    config.positionX = GetJsonValueFloat(json, "x", 0.0f);
    config.positionY = GetJsonValueFloat(json, "y", 0.0f);
    config.width = GetJsonValueFloat(json, "width", 50.0f);
    config.height = GetJsonValueFloat(json, "height", 50.0f);
    config.sprite = GetJsonValue(json, "sprite");
    config.physics = GetJsonValueBool(json, "physics", false);
    config.isStatic = GetJsonValueBool(json, "static", false);
    
    size_t propsStart = json.find("\"properties\"");
    if (propsStart != std::string::npos) {
        size_t braceStart = json.find("{", propsStart);
        size_t braceEnd = json.find("}", braceStart);
        if (braceStart != std::string::npos && braceEnd != std::string::npos) {
            std::string propsJson = json.substr(braceStart + 1, braceEnd - braceStart - 1);
            
            size_t pos = 0;
            while ((pos = propsJson.find("\"", pos)) != std::string::npos) {
                size_t keyStart = pos + 1;
                size_t keyEnd = propsJson.find("\"", keyStart);
                if (keyEnd == std::string::npos) break;
                
                std::string key = propsJson.substr(keyStart, keyEnd - keyStart);
                
                size_t valStart = propsJson.find(":", keyEnd);
                if (valStart == std::string::npos) break;
                valStart++;
                
                while (valStart < propsJson.size() && (propsJson[valStart] == ' ' || propsJson[valStart] == '\t')) valStart++;
                
                std::string value;
                if (valStart < propsJson.size() && propsJson[valStart] == '"') {
                    valStart++;
                    size_t valEnd = propsJson.find('"', valStart);
                    if (valEnd == std::string::npos) break;
                    value = propsJson.substr(valStart, valEnd - valStart);
                    pos = valEnd + 1;
                } else {
                    size_t valEnd = valStart;
                    while (valEnd < propsJson.size() && propsJson[valEnd] != ',' && propsJson[valEnd] != '\n') valEnd++;
                    value = propsJson.substr(valStart, valEnd - valStart);
                    pos = valEnd;
                }
                
                config.components[key] = value;
            }
        }
    }
}

void SceneLoader::CreateSceneObjects(Scene* scene, const std::vector<SceneObjectConfig>& objects) {
    for (size_t i = 0; i < objects.size(); i++) {
        CreateObjectFromConfig(scene, objects[i]);
    }
}

GameObject* SceneLoader::CreateObjectFromConfig(Scene* scene, const SceneObjectConfig& config) {
    GameObject* go = new GameObject(config.name.c_str());
    go->Enable();
    
    Transform* tf = go->GetComponent<Transform>();
    tf->tPosition.X = config.positionX;
    tf->tPosition.Y = config.positionY;
    tf->tRect.W = (int)config.width;
    tf->tRect.H = (int)config.height;
    
    if (!config.sprite.empty()) {
        Spirte* sprite = new Spirte(config.name.c_str(), config.sprite.c_str());
        go->AddComponent(sprite);
    }
    
    if (config.physics) {
        RigidBodyB2* rb = new RigidBodyB2(
            HG_ENGINE()->tPhyiscs.FirstWorld(),
            tf,
            !config.isStatic,
            (config.name + "_RB").c_str()
        );
        go->AddComponent(rb);
    }
    
    for (auto it = config.components.begin(); it != config.components.end(); ++it) {
        HG::HGComponent* comp = CreateComponentFromConfig(it->first.c_str());
        if (comp != nullptr) {
            std::map<std::string, std::string> props;
            props[it->first] = it->second;
            ApplyComponentConfig(comp, props);
            go->AddComponent(comp);
        }
    }
    
    for (size_t i = 0; i < config.children.size(); i++) {
        CreateObjectFromConfig(scene, config.children[i]);
    }
    
    return go;
}

HG::HGComponent* SceneLoader::CreateComponentFromConfig(const char* componentType) {
    if (strcmp(componentType, "Button") == 0) {
        return new Button("Button");
    }
    if (strcmp(componentType, "Slider") == 0) {
        return new Slider("Slider");
    }
    if (strcmp(componentType, "Checkbox") == 0) {
        return new Checkbox("Checkbox");
    }
    if (strcmp(componentType, "Listbox") == 0) {
        return new Listbox("Listbox");
    }
    if (strcmp(componentType, "InputField") == 0) {
        return new InputField("InputField");
    }
    if (strcmp(componentType, "VerticalLayout") == 0) {
        return new VerticalLayout("Layout");
    }
    if (strcmp(componentType, "HorizontalLayout") == 0) {
        return new HorizontalLayout("Layout");
    }
    if (strcmp(componentType, "ParticleSystem") == 0) {
        return new ParticleSystem("Particles");
    }
    return nullptr;
}

void SceneLoader::ApplyComponentConfig(HG::HGComponent* comp, const std::map<std::string, std::string>& props) {
    for (auto it = props.begin(); it != props.end(); ++it) {
        const std::string& key = it->first;
        const std::string& val = it->second;
        
        if (strcmp(comp->GetTypeName(), "Button") == 0) {
            Button* btn = static_cast<Button*>(comp);
            if (key == "text") btn->SetText(val.c_str());
        }
        else if (strcmp(comp->GetTypeName(), "Slider") == 0) {
            Slider* slider = static_cast<Slider*>(comp);
            if (key == "min") slider->SetRange((float)atof(val.c_str()), slider->GetMaxValue());
            if (key == "max") slider->SetRange(slider->GetMinValue(), (float)atof(val.c_str()));
            if (key == "value") slider->SetValue((float)atof(val.c_str()));
        }
        else if (strcmp(comp->GetTypeName(), "Checkbox") == 0) {
            Checkbox* cb = static_cast<Checkbox*>(comp);
            if (key == "text") cb->SetText(val.c_str());
            if (key == "checked") cb->SetChecked(val == "true");
        }
        else if (strcmp(comp->GetTypeName(), "Listbox") == 0) {
            Listbox* lb = static_cast<Listbox*>(comp);
            if (key == "items") {
                size_t start = 0;
                std::string items = val;
                while ((start = items.find(";", start)) != std::string::npos) {
                    lb->AddItem(items.substr(0, start).c_str());
                    items = items.substr(start + 1);
                    start = 0;
                }
                if (!items.empty()) lb->AddItem(items.c_str());
            }
        }
    }
}

std::string SceneLoader::GetStringProp(const std::map<std::string, std::string>& props, const char* key, const char* defaultValue) {
    auto it = props.find(key);
    if (it != props.end()) return it->second;
    return defaultValue;
}

int SceneLoader::GetIntProp(const std::map<std::string, std::string>& props, const char* key, int defaultValue) {
    auto it = props.find(key);
    if (it != props.end()) return atoi(it->second.c_str());
    return defaultValue;
}

float SceneLoader::GetFloatProp(const std::map<std::string, std::string>& props, const char* key, float defaultValue) {
    auto it = props.find(key);
    if (it != props.end()) return (float)atof(it->second.c_str());
    return defaultValue;
}

bool SceneLoader::GetBoolProp(const std::map<std::string, std::string>& props, const char* key, bool defaultValue) {
    auto it = props.find(key);
    if (it != props.end()) return it->second == "true" || it->second == "1";
    return defaultValue;
}

} // namespace V1SDL
} // namespace HGEngine
