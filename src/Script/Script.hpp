#pragma once

#include <string>
#include <vector>
#include <cstddef>

// Forward declarations
struct SDL_Event;

namespace HG {
namespace Script {

/// \brief Script function pointer types
typedef int (*ScriptInitFunc)();
typedef int (*ScriptUpdateFunc)(void*);
typedef int (*ScriptRenderFunc)(void*);
typedef int (*ScriptEventFunc)(void*, void*);
typedef void (*ScriptDestroyFunc)();

/// \brief Script instance data
struct ScriptContext {
    void* userData;
    ScriptUpdateFunc onUpdate;
    ScriptRenderFunc onRender;
    ScriptEventFunc onEvent;
    ScriptDestroyFunc onDestroy;
};

/// \brief Script type info
struct ScriptInfo {
    const char* name;
    ScriptInitFunc initFunc;
    size_t userDataSize;
};

/// \brief Script manager for registering and running scripts
class ScriptManager {
private:
    static ScriptManager* s_Instance;
    std::vector<ScriptInfo*> m_Scripts;

public:
    static ScriptManager* GetInstance() {
        if (s_Instance == nullptr) {
            s_Instance = new ScriptManager();
        }
        return s_Instance;
    }

    static void Destroy() {
        if (s_Instance != nullptr) {
            delete s_Instance;
            s_Instance = nullptr;
        }
    }

    /// \brief Register a script type
    void RegisterScript(ScriptInfo* info) {
        m_Scripts.push_back(info);
    }

    /// \brief Get script by name
    ScriptInfo* GetScript(const char* name) {
        for (size_t i = 0; i < m_Scripts.size(); i++) {
            if (m_Scripts[i] != nullptr && 
                std::string(m_Scripts[i]->name) == std::string(name)) {
                return m_Scripts[i];
            }
        }
        return nullptr;
    }

    /// \brief Get all registered scripts
    const std::vector<ScriptInfo*>& GetAllScripts() const {
        return m_Scripts;
    }
};

/// \brief Helper macro to declare a script
#define HG_SCRIPT_DECLARE(NAME) \
    namespace NAME##_Script { \
        extern ScriptInfo g_ScriptInfo; \
        int Init(); \
        int Update(void*); \
        int Render(void*); \
        int OnEvent(void*, void*); \
        void Destroy(); \
    }

/// \brief Helper macro to implement a script
#define HG_SCRIPT_IMPLEMENT(NAME, USER_DATA_TYPE) \
    namespace NAME##_Script { \
        static int Init_() { return Init(); } \
        static int Update_(void* p) { return Update(p); } \
        static int Render_(void* p) { return Render(p); } \
        static int OnEvent_(void* e, void* p) { return OnEvent(e, p); } \
        static void Destroy_() { Destroy(); } \
        \
        ScriptInfo g_ScriptInfo = { \
            #NAME, \
            Init_, \
            sizeof(USER_DATA_TYPE) \
        }; \
        \
        static int Register() { \
            HG::Script::ScriptManager::GetInstance()->RegisterScript(&g_ScriptInfo); \
            return 0; \
        } \
        static int g_Register = Register(); \
    } \
    int NAME##_Script::Init()
    
/// \brief Get script user data pointer
#define HG_SCRIPT_GET_DATA(CTX, TYPE) ((TYPE*)((char*)(CTX)->userData))

} // namespace Script
} // namespace HG
