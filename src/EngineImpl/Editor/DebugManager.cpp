#include "Editor/DebugManager.h"
#include "Scene.h"
#include "GameObject.h"

#ifdef HGENGINE_IMGUI
#include <imgui.h>
#endif

namespace HGEngine {
namespace V1SDL {

DebugManager* DebugManager::s_Instance = nullptr;

DebugManager::DebugManager()
    : m_bShowHierarchy(true)
    , m_bShowProperties(true)
    , m_bShowPerformance(true)
    , m_bShowScene(true)
    , m_bShowConsole(true)
    , m_bShowMetrics(false)
    , m_nFrameTimeIndex(0)
    , m_fFpsUpdateTimer(0.0f)
    , m_nFrameCount(0)
{
    for (int i = 0; i < 60; i++) {
        m_fFrameTimeHistory[i] = 0.0f;
    }
}

void DebugManager::BeginFrame() {
    m_nFrameCount++;
}

void DebugManager::EndFrame() {
}

void DebugManager::Render() {
#ifdef HGENGINE_IMGUI
    if (m_bShowHierarchy) {
        RenderHierarchy();
    }
    if (m_bShowPerformance) {
        RenderPerformance();
    }
    if (m_bShowScene) {
        RenderScene();
    }
    if (m_bShowConsole) {
        RenderConsole();
    }
    if (m_bShowMetrics) {
        RenderMetricsOverlay();
    }
#endif
}

void DebugManager::TogglePanel(DebugPanelType type) {
    switch (type) {
        case DebugPanelType::Hierarchy:
            m_bShowHierarchy = !m_bShowHierarchy;
            break;
        case DebugPanelType::Properties:
            m_bShowProperties = !m_bShowProperties;
            break;
        case DebugPanelType::Performance:
            m_bShowPerformance = !m_bShowPerformance;
            break;
        case DebugPanelType::Scene:
            m_bShowScene = !m_bShowScene;
            break;
        case DebugPanelType::Console:
            m_bShowConsole = !m_bShowConsole;
            break;
        default:
            break;
    }
}

bool DebugManager::IsPanelVisible(DebugPanelType type) const {
    switch (type) {
        case DebugPanelType::Hierarchy:
            return m_bShowHierarchy;
        case DebugPanelType::Properties:
            return m_bShowProperties;
        case DebugPanelType::Performance:
            return m_bShowPerformance;
        case DebugPanelType::Scene:
            return m_bShowScene;
        case DebugPanelType::Console:
            return m_bShowConsole;
        default:
            return false;
    }
}

void DebugManager::UpdateMetrics(float frameTime, int drawCalls, int gameObjects, int components) {
    m_Metrics.frameTime = frameTime;
    m_Metrics.drawCalls = drawCalls;
    m_Metrics.gameObjects = gameObjects;
    m_Metrics.components = components;
    
    m_fFpsUpdateTimer += frameTime;
    m_nFrameCount++;
    
    if (m_fFpsUpdateTimer >= 1.0f) {
        m_Metrics.fps = (float)m_nFrameCount / m_fFpsUpdateTimer;
        m_nFrameCount = 0;
        m_fFpsUpdateTimer = 0.0f;
    }
    
    m_fFrameTimeHistory[m_nFrameTimeIndex] = frameTime;
    m_nFrameTimeIndex = (m_nFrameTimeIndex + 1) % 60;
}

void DebugManager::Log(const char* message) {
    m_ConsoleLogs.push_back(message);
    if (m_ConsoleLogs.size() > 1000) {
        m_ConsoleLogs.erase(m_ConsoleLogs.begin());
    }
}

void DebugManager::RenderMenuBar() {
#ifdef HGENGINE_IMGUI
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Debug")) {
            ImGui::MenuItem("Hierarchy", nullptr, &m_bShowHierarchy);
            ImGui::MenuItem("Properties", nullptr, &m_bShowProperties);
            ImGui::MenuItem("Performance", nullptr, &m_bShowPerformance);
            ImGui::MenuItem("Scene", nullptr, &m_bShowScene);
            ImGui::MenuItem("Console", nullptr, &m_bShowConsole);
            ImGui::Separator();
            ImGui::MenuItem("Metrics Overlay", nullptr, &m_bShowMetrics);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
#endif
}

void DebugManager::RenderHierarchy() {
#ifdef HGENGINE_IMGUI
    ImGui::SetNextWindowSize(ImVec2(250, 400), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Hierarchy", &m_bShowHierarchy)) {
        HG::HGScene* scene = HG::HGGetActiveScene();
        if (scene != nullptr) {
            auto& children = scene->GetChildren();
            for (size_t i = 0; i < children.size(); i++) {
                GameObject* obj = children[i];
                ImGui::Text("%s", obj->GetName());
            }
        }
    }
    ImGui::End();
#endif
}

void DebugManager::RenderPerformance() {
#ifdef HGENGINE_IMGUI
    ImGui::SetNextWindowSize(ImVec2(250, 200), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Performance", &m_bShowPerformance)) {
        ImGui::Text("FPS: %.1f", m_Metrics.fps);
        ImGui::Text("Frame Time: %.2f ms", m_Metrics.frameTime * 1000.0f);
        ImGui::Text("Draw Calls: %d", m_Metrics.drawCalls);
        ImGui::Text("GameObjects: %d", m_Metrics.gameObjects);
        ImGui::Text("Components: %d", m_Metrics.components);
        
        ImGui::Separator();
        ImGui::Text("Frame Time History:");
        ImGui::PlotLines("##fps", m_fFrameTimeHistory, 60, m_nFrameTimeIndex, nullptr, 0.0f, 100.0f, ImVec2(0, 50));
    }
    ImGui::End();
#endif
}

void DebugManager::RenderScene() {
#ifdef HGENGINE_IMGUI
    ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Scene", &m_bShowScene)) {
        ImGui::Text("Scene Information");
        HG::HGScene* scene = HG::HGGetActiveScene();
        if (scene != nullptr) {
            ImGui::Text("Name: %s", scene->GetName());
            ImGui::Text("Objects: %d", scene->GetChildren().size());
        }
    }
    ImGui::End();
#endif
}

void DebugManager::RenderConsole() {
#ifdef HGENGINE_IMGUI
    ImGui::SetNextWindowSize(ImVec2(500, 300), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Console", &m_bShowConsole)) {
        for (size_t i = 0; i < m_ConsoleLogs.size(); i++) {
            ImGui::Text("%s", m_ConsoleLogs[i].c_str());
        }
        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
            ImGui::SetScrollHereY(1.0f);
        }
    }
    ImGui::End();
#endif
}

void DebugManager::RenderMetricsOverlay() {
#ifdef HGENGINE_IMGUI
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(150, 80), ImGuiCond_Always);
    ImGui::Begin("Metrics", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    ImGui::Text("FPS: %.1f", m_Metrics.fps);
    ImGui::Text("Draw Calls: %d", m_Metrics.drawCalls);
    ImGui::Text("Objects: %d", m_Metrics.gameObjects);
    ImGui::End();
#endif
}

} // namespace V1SDL
} // namespace HGEngine
