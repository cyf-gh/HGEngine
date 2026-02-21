#include "GUI/UIComponents.h"
#include "Texture.h"
#include "Renderer2D.h"

namespace HGEngine {
namespace V1SDL {

InputField::InputField(const char* name)
    : HG::HGComponent(name)
    , m_pLabel(nullptr)
    , m_strText("")
    , m_strPlaceholder("")
    , m_bIsFocused(false)
    , m_bIsPassword(false)
    , m_nMaxLength(256)
    , m_nCursorPosition(0)
    , m_pOnTextChanged(nullptr)
    , m_pOnEnterPressed(nullptr)
{
    m_tPlaceholderColor.r = 128;
    m_tPlaceholderColor.g = 128;
    m_tPlaceholderColor.b = 128;
    m_tPlaceholderColor.a = 255;
}

InputField::~InputField() {
}

void InputField::SetText(const char* text) {
    if (text == nullptr) return;
    
    m_strText = text;
    if ((int)m_strText.length() > m_nMaxLength) {
        m_strText = m_strText.substr(0, m_nMaxLength);
    }
    m_nCursorPosition = (int)m_strText.length();
    UpdateLabel();
    
    if (m_pOnTextChanged != nullptr) {
        m_pOnTextChanged(m_strText.c_str());
    }
}

void InputField::SetFocused(bool focused) {
    m_bIsFocused = focused;
    if (focused && m_pLabel != nullptr) {
        SDL_StartTextInput();
    } else {
        SDL_StopTextInput();
    }
}

void InputField::HandleKeyEvent(const SDL_Event& event) {
    if (!m_bIsFocused) return;
    
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_BACKSPACE:
                if (m_nCursorPosition > 0) {
                    m_strText.erase(m_nCursorPosition - 1, 1);
                    m_nCursorPosition--;
                    UpdateLabel();
                    if (m_pOnTextChanged != nullptr) {
                        m_pOnTextChanged(m_strText.c_str());
                    }
                }
                break;
            case SDLK_LEFT:
                if (m_nCursorPosition > 0) {
                    m_nCursorPosition--;
                }
                break;
            case SDLK_RIGHT:
                if (m_nCursorPosition < (int)m_strText.length()) {
                    m_nCursorPosition++;
                }
                break;
            case SDLK_HOME:
                m_nCursorPosition = 0;
                break;
            case SDLK_END:
                m_nCursorPosition = (int)m_strText.length();
                break;
            case SDLK_RETURN:
            case SDLK_KP_ENTER:
                if (m_pOnEnterPressed != nullptr) {
                    m_pOnEnterPressed(m_strText.c_str());
                }
                break;
            default:
                break;
        }
    }
}

void InputField::HandleTextInputEvent(const SDL_Event& event) {
    if (!m_bIsFocused) return;
    
    if (event.type == SDL_TEXTINPUT) {
        if ((int)m_strText.length() + (int)strlen(event.text.text) <= m_nMaxLength) {
            m_strText.insert(m_nCursorPosition, event.text.text);
            m_nCursorPosition += (int)strlen(event.text.text);
            UpdateLabel();
            if (m_pOnTextChanged != nullptr) {
                m_pOnTextChanged(m_strText.c_str());
            }
        }
    }
}

void InputField::UpdateLabel() {
    if (m_pLabel != nullptr) {
        m_pLabel->SetText(m_strText.c_str());
    }
}

Texture* InputField::GetRenderTarget(Renderer2D* pRd) {
    (void)pRd;
    if (m_pLabel != nullptr) {
        return m_pLabel->GetRenderTarget(pRd);
    }
    return nullptr;
}

Button::Button(const char* name)
    : HG::HGComponent(name)
    , m_pLabel(nullptr)
    , m_bIsPressed(false)
    , m_bIsHovered(false)
    , m_pOnClick(nullptr)
    , m_pOnPressed(nullptr)
    , m_pOnReleased(nullptr)
{
}

Button::~Button() {
}

void Button::SetText(const char* text) {
    if (m_pLabel != nullptr) {
        m_pLabel->SetText(text);
    }
}

const char* Button::GetText() const {
    if (m_pLabel != nullptr) {
        return m_pLabel->GetText();
    }
    return "";
}

Texture* Button::GetRenderTarget(Renderer2D* pRd) {
    (void)pRd;
    if (m_pLabel != nullptr) {
        return m_pLabel->GetRenderTarget(pRd);
    }
    return nullptr;
}

void Button::HandlePointerDown() {
    m_bIsPressed = true;
    if (m_pOnPressed != nullptr) {
        m_pOnPressed();
    }
}

void Button::HandlePointerUp() {
    if (m_bIsPressed && m_bIsHovered) {
        if (m_pOnClick != nullptr) {
            m_pOnClick();
        }
    }
    m_bIsPressed = false;
    if (m_pOnReleased != nullptr) {
        m_pOnReleased();
    }
}

void Button::HandlePointerEnter() {
    m_bIsHovered = true;
}

void Button::HandlePointerExit() {
    m_bIsHovered = false;
    m_bIsPressed = false;
}

Slider::Slider(const char* name)
    : HG::HGComponent(name)
    , m_fValue(0.5f)
    , m_fMinValue(0.0f)
    , m_fMaxValue(1.0f)
    , m_fStep(0.01f)
    , m_bIsDragging(false)
    , m_pLabel(nullptr)
    , m_pOnValueChanged(nullptr)
{
    m_tTrackRect.X = 0;
    m_tTrackRect.Y = 0;
    m_tTrackRect.W = 100;
    m_tTrackRect.H = 10;
    m_tThumbRect.X = 0;
    m_tThumbRect.Y = 0;
    m_tThumbRect.W = 20;
    m_tThumbRect.H = 20;
}

Slider::~Slider() {
}

void Slider::SetValue(float value) {
    if (value < m_fMinValue) value = m_fMinValue;
    if (value > m_fMaxValue) value = m_fMaxValue;
    
    m_fValue = value;
    UpdateThumbPosition();
    UpdateLabel();
    
    if (m_pOnValueChanged != nullptr) {
        m_pOnValueChanged(m_fValue);
    }
}

void Slider::UpdateThumbPosition() {
    float ratio = (m_fValue - m_fMinValue) / (m_fMaxValue - m_fMinValue);
    m_tThumbRect.X = (int)(ratio * (m_tTrackRect.W - m_tThumbRect.W));
    m_tThumbRect.Y = m_tTrackRect.Y - (m_tThumbRect.H - m_tTrackRect.H) / 2;
}

void Slider::UpdateLabel() {
    if (m_pLabel != nullptr) {
        char buffer[32];
        snprintf(buffer, sizeof(buffer), "%.2f", m_fValue);
        m_pLabel->SetText(buffer);
    }
}

Texture* Slider::GetRenderTarget(Renderer2D* pRd) {
    (void)pRd;
    if (m_pLabel != nullptr) {
        return m_pLabel->GetRenderTarget(pRd);
    }
    return nullptr;
}

void Slider::HandlePointerDown(int x, int y) {
    (void)x;
    (void)y;
    m_bIsDragging = true;
}

void Slider::HandlePointerUp() {
    m_bIsDragging = false;
}

void Slider::HandlePointerMove(int x, int y) {
    if (!m_bIsDragging) return;
    
    (void)y;
    float ratio = (float)(x - m_tTrackRect.X) / (float)m_tTrackRect.W;
    if (ratio < 0.0f) ratio = 0.0f;
    if (ratio > 1.0f) ratio = 1.0f;
    
    float newValue = m_fMinValue + ratio * (m_fMaxValue - m_fMinValue);
    newValue = ((int)(newValue / m_fStep)) * m_fStep;
    
    if (newValue != m_fValue) {
        SetValue(newValue);
    }
}

Listbox::Listbox(const char* name)
    : HG::HGComponent(name)
    , m_nSelectedIndex(-1)
    , m_nVisibleCount(5)
    , m_nTopIndex(0)
    , m_fItemHeight(20.0f)
    , m_bIsScrolling(false)
    , m_pOnSelectionChanged(nullptr)
    , m_pOnItemClicked(nullptr)
{
    m_tViewRect.X = 0;
    m_tViewRect.Y = 0;
    m_tViewRect.W = 100;
    m_tViewRect.H = 100;
    
    m_tScrollBarRect.X = 0;
    m_tScrollBarRect.Y = 0;
    m_tScrollBarRect.W = 10;
    m_tScrollBarRect.H = 100;
}

Listbox::~Listbox() {
    m_Items.clear();
}

void Listbox::AddItem(const char* text, void* userData) {
    ListboxItem item(text, userData);
    item.selected = false;
    m_Items.push_back(item);
}

void Listbox::RemoveItem(int index) {
    if (index >= 0 && index < (int)m_Items.size()) {
        m_Items.erase(m_Items.begin() + index);
        if (m_nSelectedIndex >= (int)m_Items.size()) {
            m_nSelectedIndex = (int)m_Items.size() - 1;
        }
    }
}

void Listbox::Clear() {
    m_Items.clear();
    m_nSelectedIndex = -1;
    m_nTopIndex = 0;
}

const char* Listbox::GetSelectedItem() const {
    if (m_nSelectedIndex >= 0 && m_nSelectedIndex < (int)m_Items.size()) {
        return m_Items[m_nSelectedIndex].text.c_str();
    }
    return "";
}

void Listbox::SetSelectedIndex(int index) {
    if (index >= 0 && index < (int)m_Items.size()) {
        for (size_t i = 0; i < m_Items.size(); i++) {
            m_Items[i].selected = (i == (size_t)index);
        }
        
        int oldIndex = m_nSelectedIndex;
        m_nSelectedIndex = index;
        
        if (index < m_nTopIndex) {
            m_nTopIndex = index;
        } else if (index >= m_nTopIndex + m_nVisibleCount) {
            m_nTopIndex = index - m_nVisibleCount + 1;
        }
        
        if (m_pOnSelectionChanged != nullptr && oldIndex != index) {
            m_pOnSelectionChanged(index, m_Items[index].text.c_str());
        }
    }
}

Texture* Listbox::GetRenderTarget(Renderer2D* pRd) {
    (void)pRd;
    return nullptr;
}

void Listbox::HandlePointerDown(int x, int y) {
    (void)x;
    int index = m_nTopIndex + (int)((y - m_tViewRect.Y) / m_fItemHeight);
    if (index >= 0 && index < (int)m_Items.size()) {
        SetSelectedIndex(index);
        if (m_pOnItemClicked != nullptr) {
            m_pOnItemClicked(index, m_Items[index].text.c_str());
        }
    }
}

void Listbox::HandlePointerWheel(int delta) {
    if (delta > 0 && m_nTopIndex > 0) {
        m_nTopIndex--;
    } else if (delta < 0 && m_nTopIndex < (int)m_Items.size() - m_nVisibleCount) {
        m_nTopIndex++;
    }
}

Checkbox::Checkbox(const char* name)
    : HG::HGComponent(name)
    , m_bIsChecked(false)
    , m_pLabel(nullptr)
    , m_pOnChanged(nullptr)
{
}

Checkbox::~Checkbox() {
}

void Checkbox::SetChecked(bool checked) {
    if (m_bIsChecked != checked) {
        m_bIsChecked = checked;
        if (m_pOnChanged != nullptr) {
            m_pOnChanged(m_bIsChecked);
        }
    }
}

void Checkbox::Toggle() {
    SetChecked(!m_bIsChecked);
}

void Checkbox::SetText(const char* text) {
    if (m_pLabel != nullptr) {
        m_pLabel->SetText(text);
    }
}

const char* Checkbox::GetText() const {
    if (m_pLabel != nullptr) {
        return m_pLabel->GetText();
    }
    return "";
}

Texture* Checkbox::GetRenderTarget(Renderer2D* pRd) {
    (void)pRd;
    if (m_pLabel != nullptr) {
        return m_pLabel->GetRenderTarget(pRd);
    }
    return nullptr;
}

void Checkbox::HandleClick() {
    Toggle();
}

} // namespace V1SDL
} // namespace HGEngine
