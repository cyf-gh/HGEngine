#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <Error.h>
#include <Serialization.h>

namespace HG {

/// \brief Configuration value union
struct ConfigValue {
    enum class Type {
        None,
        Int,
        Float,
        String,
        Bool
    };

    Type type;
    union {
        int intVal;
        float floatVal;
        bool boolVal;
    };
    std::string strVal;

    ConfigValue() : type(Type::None), intVal(0) {}
    ~ConfigValue() {
        if (type == Type::String) {
            strVal.~basic_string();
        }
    }

    ConfigValue(int val) : type(Type::Int), intVal(val) {}
    ConfigValue(float val) : type(Type::Float), floatVal(val) {}
    ConfigValue(bool val) : type(Type::Bool), boolVal(val) {}
    ConfigValue(const std::string& val) : type(Type::String), strVal(val) {}
    ConfigValue(const char* val) : type(Type::String), strVal(val) {}
};

/// \brief Configuration manager class
class Config {
private:
    static Config* s_Instance;
    std::unordered_map<std::string, ConfigValue> m_Values;
    std::string m_FilePath;
    bool m_IsDirty;

public:
    /// \brief Get singleton instance
    static Config* GetInstance() {
        if (s_Instance == nullptr) {
            s_Instance = new Config();
        }
        return s_Instance;
    }

    /// \brief Initialize config from file
    /// \param filePath Path to config file (JSON format)
    static HG::Result Initialize(const char* filePath) {
        if (s_Instance != nullptr) {
            delete s_Instance;
        }
        s_Instance = new Config();
        return s_Instance->Load(filePath);
    }

    /// \brief Shutdown config
    static void Shutdown() {
        if (s_Instance != nullptr) {
            s_Instance->Save();
            delete s_Instance;
            s_Instance = nullptr;
        }
    }

    /// \brief Constructor
    Config() : m_FilePath("./config.json"), m_IsDirty(false) {}

    /// \brief Destructor
    ~Config() {
        if (m_IsDirty) {
            Save();
        }
    }

    /// \brief Load config from file
    /// \param filePath Path to config file
    /// \return Result code
    Result Load(const char* filePath) {
        m_FilePath = filePath;

        FILE* fp = fopen(filePath, "r");
        if (fp == nullptr) {
            // Create default config if file doesn't exist
            return Save();
        }

        fseek(fp, 0, SEEK_END);
        long size = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        std::string json;
        json.resize(size);
        fread(&json[0], 1, size, fp);
        fclose(fp);

        return ParseJson(json.c_str());
    }

    /// \brief Save config to file
    /// \return Result code
    Result Save() const {
        std::string json = ToJson();

        FILE* fp = fopen(m_FilePath.c_str(), "w");
        if (fp == nullptr) {
            return Result(HG_ERR_UNKNOWN, "Failed to open config file for writing");
        }

        fwrite(json.c_str(), 1, json.length(), fp);
        fclose(fp);

        return Result(HG_ERR_OK);
    }

    /// \brief Convert config to JSON string
    std::string ToJson() const {
        std::string json = "{\n";
        bool first = true;
        for (const auto& pair : m_Values) {
            if (!first) json += ",\n";
            first = false;
            json += "  \"" + pair.first + "\": ";
            
            switch (pair.second.type) {
                case ConfigValue::Type::Int:
                    json += std::to_string(pair.second.intVal);
                    break;
                case ConfigValue::Type::Float:
                    json += std::to_string(pair.second.floatVal);
                    break;
                case ConfigValue::Type::Bool:
                    json += pair.second.boolVal ? "true" : "false";
                    break;
                case ConfigValue::Type::String:
                    json += "\"" + pair.second.strVal + "\"";
                    break;
                default:
                    json += "null";
                    break;
            }
        }
        json += "\n}";
        return json;
    }

    /// \brief Parse JSON string
    Result ParseJson(const char* json) {
        if (json == nullptr || strlen(json) == 0) {
            return Result(HG_ERR_PARSE_FAILED, "Empty JSON");
        }

        // Simple JSON parsing (key: value pairs)
        m_Values.clear();
        
        // For now, just create empty config if parsing fails
        // A full JSON parser would be complex
        return Result(HG_ERR_OK);
    }

    /// \brief Set integer value
    void SetInt(const char* key, int value) {
        m_Values[key] = ConfigValue(value);
        m_IsDirty = true;
    }

    /// \brief Set float value
    void SetFloat(const char* key, float value) {
        m_Values[key] = ConfigValue(value);
        m_IsDirty = true;
    }

    /// \brief Set string value
    void SetString(const char* key, const char* value) {
        m_Values[key] = ConfigValue(value);
        m_IsDirty = true;
    }

    /// \brief Set boolean value
    void SetBool(const char* key, bool value) {
        m_Values[key] = ConfigValue(value);
        m_IsDirty = true;
    }

    /// \brief Get integer value
    int GetInt(const char* key, int defaultValue = 0) const {
        auto it = m_Values.find(key);
        if (it != m_Values.end() && it->second.type == ConfigValue::Type::Int) {
            return it->second.intVal;
        }
        return defaultValue;
    }

    /// \brief Get float value
    float GetFloat(const char* key, float defaultValue = 0.0f) const {
        auto it = m_Values.find(key);
        if (it != m_Values.end() && it->second.type == ConfigValue::Type::Float) {
            return it->second.floatVal;
        }
        return defaultValue;
    }

    /// \brief Get string value
    std::string GetString(const char* key, const char* defaultValue = "") const {
        auto it = m_Values.find(key);
        if (it != m_Values.end() && it->second.type == ConfigValue::Type::String) {
            return it->second.strVal;
        }
        return defaultValue;
    }

    /// \brief Get boolean value
    bool GetBool(const char* key, bool defaultValue = false) const {
        auto it = m_Values.find(key);
        if (it != m_Values.end() && it->second.type == ConfigValue::Type::Bool) {
            return it->second.boolVal;
        }
        return defaultValue;
    }

    /// \brief Check if key exists
    bool HasKey(const char* key) const {
        return m_Values.find(key) != m_Values.end();
    }

    /// \brief Remove a key
    void Remove(const char* key) {
        m_Values.erase(key);
        m_IsDirty = true;
    }

    /// \brief Clear all values
    void Clear() {
        m_Values.clear();
        m_IsDirty = true;
    }

    /// \brief Get config file path
    const std::string& GetFilePath() const { return m_FilePath; }
};

// Default configuration values
namespace ConfigDefaults {
    // Window settings
    constexpr int WindowWidth = 1280;
    constexpr int WindowHeight = 720;
    constexpr const char* WindowTitle = "HGEngine Game";
    constexpr bool WindowFullscreen = false;

    // Rendering settings
    constexpr int TargetFPS = 60;
    constexpr bool VSync = true;

    // Physics settings
    constexpr float GravityX = 0.0f;
    constexpr float GravityY = 9.8f;

    // Logging settings
    constexpr const char* LogPath = "./Log/";
    constexpr int LogLevel = 0; // Debug
}

// Convenience macros
#define HG_CONFIG_GET_INT(KEY, DEFAULT) HG::Config::GetInstance()->GetInt(KEY, DEFAULT)
#define HG_CONFIG_GET_FLOAT(KEY, DEFAULT) HG::Config::GetInstance()->GetFloat(KEY, DEFAULT)
#define HG_CONFIG_GET_STRING(KEY, DEFAULT) HG::Config::GetInstance()->GetString(KEY, DEFAULT).c_str()
#define HG_CONFIG_GET_BOOL(KEY, DEFAULT) HG::Config::GetInstance()->GetBool(KEY, DEFAULT)

#define HG_CONFIG_SET_INT(KEY, VALUE) HG::Config::GetInstance()->SetInt(KEY, VALUE)
#define HG_CONFIG_SET_FLOAT(KEY, VALUE) HG::Config::GetInstance()->SetFloat(KEY, VALUE)
#define HG_CONFIG_SET_STRING(KEY, VALUE) HG::Config::GetInstance()->SetString(KEY, VALUE)
#define HG_CONFIG_SET_BOOL(KEY, VALUE) HG::Config::GetInstance()->SetBool(KEY, VALUE)
