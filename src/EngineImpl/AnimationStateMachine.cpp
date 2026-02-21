#include "AnimationStateMachine.h"

namespace HGEngine {
namespace V1SDL {

AnimStateMachine::AnimStateMachine(const char* name)
    : HG::HGComponent(name)
    , m_strCurrentState("")
    , m_strPreviousState("")
    , m_fStateTime(0.0f)
    , m_nCurrentFrame(0)
    , m_bIsPlaying(false)
    , m_bJustEntered(false)
{
}

AnimStateMachine::~AnimStateMachine() {
    m_States.clear();
    m_Transitions.clear();
}

void AnimStateMachine::AddState(const char* name, int startFrame, int endFrame, float duration, bool loop) {
    AnimState state(name, startFrame, endFrame, duration, loop);
    m_States[name] = state;
}

void AnimStateMachine::AddTransition(const char* from, const char* to, float exitTime) {
    AnimTransition trans;
    trans.fromState = from;
    trans.toState = to;
    trans.exitTime = exitTime;
    trans.hasCondition = false;
    trans.condition = nullptr;
    trans.conditionData = nullptr;
    m_Transitions.push_back(trans);
}

void AnimStateMachine::SetInitialState(const char* name) {
    if (m_States.find(name) != m_States.end()) {
        m_strCurrentState = name;
        m_strPreviousState = "";
        m_fStateTime = 0.0f;
        m_nCurrentFrame = 0;
        m_bJustEntered = true;
    }
}

void AnimStateMachine::SetState(const char* name) {
    if (m_States.find(name) != m_States.end()) {
        ExitState(m_strCurrentState.c_str());
        m_strPreviousState = m_strCurrentState;
        m_strCurrentState = name;
        m_fStateTime = 0.0f;
        m_nCurrentFrame = 0;
        m_bJustEntered = true;
        EnterState(name);
    }
}

void AnimStateMachine::Update(float deltaTime) {
    if (!m_bIsPlaying) return;
    if (m_strCurrentState.empty()) return;

    auto it = m_States.find(m_strCurrentState);
    if (it == m_States.end()) return;

    AnimState& state = it->second;
    m_fStateTime += deltaTime;

    float normalizedTime = m_fStateTime / state.duration;
    
    if (normalizedTime >= 1.0f) {
        if (state.loop) {
            m_fStateTime = 0.0f;
            m_bJustEntered = true;
        } else {
            m_fStateTime = state.duration;
        }
    }

    CalculateFrame();

    if (m_bJustEntered) {
        m_bJustEntered = false;
    }

    if (!CheckTransitions()) {
        if (!state.loop && m_fStateTime >= state.duration) {
            Stop();
        }
    }
}

bool AnimStateMachine::CheckTransitions() {
    if (m_strCurrentState.empty()) return false;

    auto it = m_States.find(m_strCurrentState);
    if (it == m_States.end()) return false;

    AnimState& state = it->second;
    float normalizedTime = m_fStateTime / state.duration;

    for (size_t i = 0; i < m_Transitions.size(); i++) {
        AnimTransition& trans = m_Transitions[i];
        
        if (trans.fromState != m_strCurrentState) continue;

        bool canTransition = false;

        if (trans.hasCondition && trans.condition != nullptr) {
            canTransition = trans.condition(trans.conditionData);
        } else {
            canTransition = (normalizedTime >= trans.exitTime);
        }

        if (canTransition) {
            SetState(trans.toState.c_str());
            return true;
        }
    }

    return false;
}

void AnimStateMachine::EnterState(const char* name) {
    m_bJustEntered = true;
}

void AnimStateMachine::ExitState(const char* name) {
}

void AnimStateMachine::CalculateFrame() {
    if (m_strCurrentState.empty()) return;

    auto it = m_States.find(m_strCurrentState);
    if (it == m_States.end()) return;

    const AnimState& state = it->second;
    float normalizedTime = m_fStateTime / state.duration;
    
    if (normalizedTime > 1.0f) normalizedTime = 1.0f;
    
    int frameCount = state.endFrame - state.startFrame + 1;
    int frameIndex = (int)(normalizedTime * frameCount);
    
    if (frameIndex >= frameCount) frameIndex = frameCount - 1;
    if (frameIndex < 0) frameIndex = 0;
    
    m_nCurrentFrame = state.startFrame + frameIndex;
}

float BlendNode1D::CalculateWeight(float deltaTime) {
    (void)deltaTime;
    return m_fBlendParameter;
}

int BlendNode1D::GetBlendedFrame() {
    if (m_Childs.empty()) return 0;
    
    int idx = m_nSelectedIndex;
    if (idx < 0) idx = 0;
    if (idx >= (int)m_Childs.size()) idx = (int)m_Childs.size() - 1;
    
    return m_Childs[idx]->startFrame;
}

} // namespace V1SDL
} // namespace HGEngine
