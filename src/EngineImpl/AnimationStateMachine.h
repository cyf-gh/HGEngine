#pragma once

#include <string>
#include <vector>
#include <map>

#include "Animation.h"

namespace HGEngine {
namespace V1SDL {

/// \brief Animation state
struct AnimState {
    std::string name;
    int startFrame;
    int endFrame;
    float duration;
    bool loop;
    
    AnimState() : name(""), startFrame(0), endFrame(0), duration(1.0f), loop(true) {}
    AnimState(const char* n, int start, int end, float dur, bool lp = true)
        : name(n), startFrame(start), endFrame(end), duration(dur), loop(lp) {}
};

/// \brief Animation transition
struct AnimTransition {
    std::string fromState;
    std::string toState;
    float exitTime;      // Normalized time (0-1) to allow transition
    bool hasCondition;
    
    // Simple condition: can be extended
    bool (*condition)(void*);
    void* conditionData;
    
    AnimTransition() : fromState(""), toState(""), exitTime(0.9f), 
                      hasCondition(false), condition(nullptr), conditionData(nullptr) {}
};

/// \brief Animation state machine
class AnimStateMachine : public HG::HGComponent {
private:
    std::map<std::string, AnimState> m_States;
    std::vector<AnimTransition> m_Transitions;
    
    std::string m_strCurrentState;
    std::string m_strPreviousState;
    
    float m_fStateTime;
    int m_nCurrentFrame;
    bool m_bIsPlaying;
    bool m_bJustEntered;

public:
    HG_COMPONENT_MUTILABLE
    
    AnimStateMachine(const char* name);
    virtual ~AnimStateMachine();

    /// \brief Add animation state
    void AddState(const char* name, int startFrame, int endFrame, float duration, bool loop = true);
    
    /// \brief Add transition
    void AddTransition(const char* from, const char* to, float exitTime = 0.9f);
    
    /// \brief Set initial state
    void SetInitialState(const char* name);
    
    /// \brief Play animation
    void Play() { m_bIsPlaying = true; m_bJustEntered = true; }
    
    /// \brief Stop animation
    void Stop() { m_bIsPlaying = false; }
    
    /// \brief Get current state
    const char* GetCurrentState() const { return m_strCurrentState.c_str(); }
    
    /// \brief Get previous state
    const char* GetPreviousState() const { return m_strPreviousState.c_str(); }
    
    /// \brief Get current frame
    int GetCurrentFrame() const { return m_nCurrentFrame; }
    
    /// \brief Check if playing
    bool IsPlaying() const { return m_bIsPlaying; }
    
    /// \brief Force set state
    void SetState(const char* name);
    
    /// \brief Update state machine
    void Update(float deltaTime);
    
    /// \brief Get component type name
    const char* GetTypeName() const override { return "AnimStateMachine"; }
    
    /// \brief Get component type id
    size_t GetTypeId() const override { return typeid(AnimStateMachine).hash_code(); }

private:
    /// \brief Check transitions
    bool CheckTransitions();
    
    /// \brief Enter state
    void EnterState(const char* name);
    
    /// \brief Exit state
    void ExitState(const char* name);
    
    /// \brief Calculate current frame
    void CalculateFrame();
};

/// \brief Animation blend tree base
class BlendTree {
public:
    virtual ~BlendTree() {}
    
    /// \brief Calculate blend weight
    virtual float CalculateWeight(float deltaTime) = 0;
    
    /// \brief Get blended frame
    virtual int GetBlendedFrame() = 0;
};

/// \brief Simple 1D blend node
class BlendNode1D : public BlendTree {
private:
    std::vector<AnimState*> m_Childs;
    float m_fBlendParameter;
    int m_nSelectedIndex;

public:
    BlendNode1D() : m_fBlendParameter(0.0f), m_nSelectedIndex(0) {}
    
    /// \brief Add child animation
    void AddChild(AnimState* state, float threshold) {
        // Store child with threshold
        m_Childs.push_back(state);
    }
    
    /// \brief Set blend parameter
    void SetBlendParameter(float param) { m_fBlendParameter = param; }
    
    /// \brief Calculate weight
    float CalculateWeight(float deltaTime) override;
    
    /// \brief Get blended frame
    int GetBlendedFrame() override;
};

} // namespace V1SDL
} // namespace HGEngine
