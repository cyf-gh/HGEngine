#pragma once

#include <string>
#include <vector>
#include <functional>

#include "Label.hpp"
#include "EventTrigger.hpp"

namespace HGEngine {
namespace V1SDL {

/// \brief Text input field
class InputField : public HG::HGComponent {
private:
    Label* m_pLabel;
    std::string m_strText;
    std::string m_strPlaceholder;
    bool m_bIsFocused;
    bool m_bIsPassword;
    int m_nMaxLength;
    int m_nCursorPosition;
    SDL_Color m_tPlaceholderColor;
    
    // Callbacks
    std::function<void(const char*)> m_pOnTextChanged;
    std::function<void(const char*)> m_pOnEnterPressed;

public:
    HG_COMPONENT_RENDERABLE

    InputField(const char* name);
    virtual ~InputField();

    /// \brief Get render target
    Texture* GetRenderTarget(Renderer2D* pRd) override;

    /// \brief Set text
    void SetText(const char* text);

    /// \brief Get text
    const char* GetText() const { return m_strText.c_str(); }

    /// \brief Set placeholder text
    void SetPlaceholder(const char* placeholder) { m_strPlaceholder = placeholder; }

    /// \brief Get placeholder
    const char* GetPlaceholder() const { return m_strPlaceholder.c_str(); }

    /// \brief Set max length
    void SetMaxLength(int maxLength) { m_nMaxLength = maxLength; }

    /// \brief Get max length
    int GetMaxLength() const { return m_nMaxLength; }

    /// \brief Set password mode
    void SetPasswordMode(bool password) { m_bIsPassword = password; }

    /// \brief Check if password mode
    bool IsPasswordMode() const { return m_bIsPassword; }

    /// \brief Set focused
    void SetFocused(bool focused);

    /// \brief Check if focused
    bool IsFocused() const { return m_bIsFocused; }

    /// \brief Set text changed callback
    void SetOnTextChanged(std::function<void(const char*)> callback) { m_pOnTextChanged = callback; }

    /// \brief Set enter pressed callback
    void SetOnEnterPressed(std::function<void(const char*)> callback) { m_pOnEnterPressed = callback; }

    /// \brief Handle key event
    void HandleKeyEvent(const SDL_Event& event);

    /// \brief Handle text input event
    void HandleTextInputEvent(const SDL_Event& event);

    /// \brief Get component type name
    const char* GetTypeName() const override { return "InputField"; }

    /// \brief Get component type id
    size_t GetTypeId() const override { return typeid(InputField).hash_code(); }

private:
    void UpdateLabel();
};

/// \brief Button UI component
class Button : public HG::HGComponent {
private:
    Label* m_pLabel;
    bool m_bIsPressed;
    bool m_bIsHovered;
    
    std::function<void()> m_pOnClick;
    std::function<void()> m_pOnPressed;
    std::function<void()> m_pOnReleased;

public:
    HG_COMPONENT_RENDERABLE

    Button(const char* name);
    virtual ~Button();

    /// \brief Get render target
    Texture* GetRenderTarget(Renderer2D* pRd) override;

    /// \brief Set text
    void SetText(const char* text);

    /// \brief Get text
    const char* GetText() const;

    /// \brief Check if pressed
    bool IsPressed() const { return m_bIsPressed; }

    /// \brief Check if hovered
    bool IsHovered() const { return m_bIsHovered; }

    /// \brief Set click callback
    void SetOnClick(std::function<void()> callback) { m_pOnClick = callback; }

    /// \brief Set pressed callback
    void SetOnPressed(std::function<void()> callback) { m_pOnPressed = callback; }

    /// \brief Set released callback
    void SetOnReleased(std::function<void()> callback) { m_pOnReleased = callback; }

    /// \brief Handle pointer events
    void HandlePointerDown();
    void HandlePointerUp();
    void HandlePointerEnter();
    void HandlePointerExit();

    /// \brief Get component type name
    const char* GetTypeName() const override { return "Button"; }

    /// \brief Get component type id
    size_t GetTypeId() const override { return typeid(Button).hash_code(); }
};

/// \brief Slider UI component
class Slider : public HG::HGComponent {
private:
    float m_fValue;
    float m_fMinValue;
    float m_fMaxValue;
    float m_fStep;
    bool m_bIsDragging;
    
    Label* m_pLabel;
    HG::Math::HGRect m_tTrackRect;
    HG::Math::HGRect m_tThumbRect;
    
    std::function<void(float)> m_pOnValueChanged;

public:
    HG_COMPONENT_RENDERABLE

    Slider(const char* name);
    virtual ~Slider();

    /// \brief Get render target
    Texture* GetRenderTarget(Renderer2D* pRd) override;

    /// \brief Set value
    void SetValue(float value);

    /// \brief Get value
    float GetValue() const { return m_fValue; }

    /// \brief Set range
    void SetRange(float min, float max) { m_fMinValue = min; m_fMaxValue = max; }

    /// \brief Get min value
    float GetMinValue() const { return m_fMinValue; }

    /// \brief Get max value
    float GetMaxValue() const { return m_fMaxValue; }

    /// \brief Set step
    void SetStep(float step) { m_fStep = step; }

    /// \brief Get step
    float GetStep() const { return m_fStep; }

    /// \brief Set value changed callback
    void SetOnValueChanged(std::function<void(float)> callback) { m_pOnValueChanged = callback; }

    /// \brief Handle pointer events
    void HandlePointerDown(int x, int y);
    void HandlePointerUp();
    void HandlePointerMove(int x, int y);

    /// \brief Get component type name
    const char* GetTypeName() const override { return "Slider"; }

    /// \brief Get component type id
    size_t GetTypeId() const override { return typeid(Slider).hash_code(); }

private:
    void UpdateThumbPosition();
    void UpdateLabel();
};

/// \brief Listbox item
struct ListboxItem {
    std::string text;
    void* userData;
    bool selected;
    
    ListboxItem() : userData(nullptr), selected(false) {}
    ListboxItem(const char* t) : text(t), userData(nullptr), selected(false) {}
};

/// \brief Listbox UI component
class Listbox : public HG::HGComponent {
private:
    std::vector<ListboxItem> m_Items;
    int m_nSelectedIndex;
    int m_nVisibleCount;
    int m_nTopIndex;
    float m_fItemHeight;
    
    HG::Math::HGRect m_tViewRect;
    HG::Math::HGRect m_tScrollBarRect;
    bool m_bIsScrolling;
    
    std::function<void(int, const char*)> m_pOnSelectionChanged;
    std::function<void(int, const char*)> m_pOnItemClicked;

public:
    HG_COMPONENT_RENDERABLE

    Listbox(const char* name);
    virtual ~Listbox();

    /// \brief Get render target
    Texture* GetRenderTarget(Renderer2D* pRd) override;

    /// \brief Add item
    void AddItem(const char* text, void* userData = nullptr);

    /// \brief Remove item
    void RemoveItem(int index);

    /// \brief Clear all items
    void Clear();

    /// \brief Get selected index
    int GetSelectedIndex() const { return m_nSelectedIndex; }

    /// \brief Get selected item
    const char* GetSelectedItem() const;

    /// \brief Set selected index
    void SetSelectedIndex(int index);

    /// \brief Set visible count
    void SetVisibleCount(int count) { m_nVisibleCount = count; }

    /// \brief Get item count
    int GetItemCount() const { return (int)m_Items.size(); }

    /// \brief Set selection changed callback
    void SetOnSelectionChanged(std::function<void(int, const char*)> callback) { m_pOnSelectionChanged = callback; }

    /// \brief Set item clicked callback
    void SetOnItemClicked(std::function<void(int, const char*)> callback) { m_pOnItemClicked = callback; }

    /// \brief Handle pointer events
    void HandlePointerDown(int x, int y);
    void HandlePointerWheel(int delta);

    /// \brief Get component type name
    const char* GetTypeName() const override { return "Listbox"; }

    /// \brief Get component type id
    size_t GetTypeId() const override { return typeid(Listbox).hash_code(); }
};

/// \brief Checkbox UI component
class Checkbox : public HG::HGComponent {
private:
    bool m_bIsChecked;
    Label* m_pLabel;
    
    std::function<void(bool)> m_pOnChanged;

public:
    HG_COMPONENT_RENDERABLE

    Checkbox(const char* name);
    virtual ~Checkbox();

    /// \brief Get render target
    Texture* GetRenderTarget(Renderer2D* pRd) override;

    /// \brief Set checked
    void SetChecked(bool checked);

    /// \brief Toggle
    void Toggle();

    /// \brief Check if checked
    bool IsChecked() const { return m_bIsChecked; }

    /// \brief Set text
    void SetText(const char* text);

    /// \brief Get text
    const char* GetText() const;

    /// \brief Set changed callback
    void SetOnChanged(std::function<void(bool)> callback) { m_pOnChanged = callback; }

    /// \brief Handle pointer click
    void HandleClick();

    /// \brief Get component type name
    const char* GetTypeName() const override { return "Checkbox"; }

    /// \brief Get component type id
    size_t GetTypeId() const override { return typeid(Checkbox).hash_code(); }
};

} // namespace V1SDL
} // namespace HGEngine
