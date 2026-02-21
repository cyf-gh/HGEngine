#pragma once

#include <string>
#include <vector>
#include <ctime>

struct SDL_Window;
struct SDL_Renderer;
struct ImGuiIO;

namespace HGEngine {
namespace V1SDL {

/// \brief Debug panel types
enum class DebugPanelType {
    None = 0,
    Hierarchy = 1,
    Properties = 2,
    Performance = 3,
    Scene = 4,
    Console = 5
};

/// \brief Performance metrics
struct PerformanceMetrics {
    float frameTime;
    float fps;
    int drawCalls;
    int gameObjects;
    int components;
    float memoryUsage;
    
    PerformanceMetrics() : frameTime(0.0f), fps(0.0f), drawCalls(0), 
                          gameObjects(0), components(0), memoryUsage(0.0f) {}
};

/// \brief Debug manager for runtime debugging
class DebugManager {
private:
    static DebugManager* s_Instance;
    
    bool m_bShowHierarchy;
    bool m_bShowProperties;
    bool m_bShowPerformance;
    bool m_bShowScene;
    bool m_bShowConsole;
    bool m_bShowMetrics;
    
    PerformanceMetrics m_Metrics;
    std::vector<std::string> m_ConsoleLogs;
    
    // FPS tracking
    float m_fFrameTimeHistory[60];
    int m_nFrameTimeIndex;
    float m_fFpsUpdateTimer;
    int m_nFrameCount;

public:
    static DebugManager* GetInstance() {
        if (s_Instance == nullptr) {
            s_Instance = new DebugManager();
        }
        return s_Instance;
    }
    
    static void Destroy() {
        if (s_Instance != nullptr) {
            delete s_Instance;
            s_Instance = nullptr;
        }
    }

    DebugManager();
    
    /// \brief Begin frame
    void BeginFrame();
    
    /// \brief End frame
    void EndFrame();
    
    /// \brief Render debug UI
    void Render();
    
    /// \brief Toggle debug panel visibility
    void TogglePanel(DebugPanelType type);
    
    /// \brief Get performance metrics
    const PerformanceMetrics& GetMetrics() const { return m_Metrics; }
    
    /// \brief Update performance metrics
    void UpdateMetrics(float frameTime, int drawCalls, int gameObjects, int components);
    
    /// \brief Log message to console
    void Log(const char* message);
    
    /// \brief Check if panel is visible
    bool IsPanelVisible(DebugPanelType type) const;
    
    /// \brief Show main menu bar
    void RenderMenuBar();
    
    /// \brief Show hierarchy panel
    void RenderHierarchy();
    
    /// \brief Show performance panel
    void RenderPerformance();
    
    /// \brief Show scene panel
    void RenderScene();
    
    /// \brief Show console panel
    void RenderConsole();
    
    /// \brief Show metrics overlay
    void RenderMetricsOverlay();
};

/// \brief Get debug manager
inline DebugManager* GetDebugManager() {
    return DebugManager::GetInstance();
}

} // namespace V1SDL
} // namespace HGEngine
