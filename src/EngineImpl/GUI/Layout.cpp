#include "GUI/Layout.h"

namespace HGEngine {
namespace V1SDL {

Layout::Layout(const char* name)
    : HG::HGComponent(name)
    , m_fSpacing(5.0f)
    , m_fPaddingLeft(0.0f)
    , m_fPaddingRight(0.0f)
    , m_fPaddingTop(0.0f)
    , m_fPaddingBottom(0.0f)
    , m_eAlignment(LayoutAlignment::TopLeft)
{
}

Layout::~Layout() {
    m_Children.clear();
}

void Layout::AddChild(HG::HGComponent* child) {
    if (child == nullptr) return;
    
    LayoutNode* node = new LayoutNode();
    node->m_pComponent = child;
    node->m_nIndex = (int)m_Children.size();
    m_Children.push_back(node);
}

void Layout::RemoveChild(HG::HGComponent* child) {
    if (child == nullptr) return;
    
    for (auto it = m_Children.begin(); it != m_Children.end(); ++it) {
        if ((*it)->m_pComponent == child) {
            delete *it;
            m_Children.erase(it);
            break;
        }
    }
    
    for (size_t i = 0; i < m_Children.size(); i++) {
        m_Children[i]->m_nIndex = (int)i;
    }
}

HG::HGComponent* Layout::GetChild(int index) const {
    if (index >= 0 && index < (int)m_Children.size()) {
        return m_Children[index]->m_pComponent;
    }
    return nullptr;
}

void Layout::SetPadding(float left, float right, float top, float bottom) {
    m_fPaddingLeft = left;
    m_fPaddingRight = right;
    m_fPaddingTop = top;
    m_fPaddingBottom = bottom;
}

void Layout::SetPadding(float padding) {
    m_fPaddingLeft = padding;
    m_fPaddingRight = padding;
    m_fPaddingTop = padding;
    m_fPaddingBottom = padding;
}

void Layout::SetAlignment(LayoutAlignment alignment) {
    m_eAlignment = alignment;
}

void Layout::CalculateLayout(float width, float height) {
    (void)width;
    (void)height;
}

void Layout::ApplyLayout() {
}

HorizontalLayout::HorizontalLayout(const char* name)
    : Layout(name)
    , m_bFillHeight(false)
{
}

HorizontalLayout::~HorizontalLayout() {
}

void HorizontalLayout::CalculateLayout(float width, float height) {
    float totalSpacing = m_fSpacing * (m_Children.size() - 1);
    float availableWidth = width - m_fPaddingLeft - m_fPaddingRight - totalSpacing;
    float y = m_fPaddingTop;
    float contentHeight = height - m_fPaddingTop - m_fPaddingBottom;
    
    float x = m_fPaddingLeft;
    
    for (size_t i = 0; i < m_Children.size(); i++) {
        LayoutNode* node = m_Children[i];
        
        float childWidth = node->m_fPreferredWidth;
        float childHeight = m_bFillHeight ? contentHeight : node->m_fPreferredHeight;
        
        if (node->m_bExpand && !m_bFillHeight) {
            childHeight = contentHeight;
        }
        
        switch (m_eAlignment) {
            case LayoutAlignment::MiddleLeft:
            case LayoutAlignment::MiddleCenter:
            case LayoutAlignment::MiddleRight:
                y = m_fPaddingTop + (contentHeight - childHeight) / 2.0f;
                break;
            case LayoutAlignment::BottomLeft:
            case LayoutAlignment::BottomCenter:
            case LayoutAlignment::BottomRight:
                y = height - m_fPaddingBottom - childHeight;
                break;
            default:
                y = m_fPaddingTop;
                break;
        }
        
        GameObject* obj = node->m_pComponent->GetGameObject();
        if (obj != nullptr) {
            Transform* tf = obj->GetTransform();
            if (tf != nullptr) {
                tf->tPosition.X = x;
                tf->tPosition.Y = y;
                tf->tRect.W = (int)childWidth;
                tf->tRect.H = (int)childHeight;
            }
        }
        
        x += childWidth + m_fSpacing;
    }
}

VerticalLayout::VerticalLayout(const char* name)
    : Layout(name)
    , m_bFillWidth(false)
{
}

VerticalLayout::~VerticalLayout() {
}

void VerticalLayout::CalculateLayout(float width, float height) {
    float totalSpacing = m_fSpacing * (m_Children.size() - 1);
    float availableHeight = height - m_fPaddingTop - m_fPaddingBottom - totalSpacing;
    float x = m_fPaddingLeft;
    float contentWidth = width - m_fPaddingLeft - m_fPaddingRight;
    
    float y = m_fPaddingTop;
    
    for (size_t i = 0; i < m_Children.size(); i++) {
        LayoutNode* node = m_Children[i];
        
        float childWidth = m_bFillWidth ? contentWidth : node->m_fPreferredWidth;
        float childHeight = node->m_fPreferredHeight;
        
        if (node->m_bExpand && !m_bFillWidth) {
            childWidth = contentWidth;
        }
        
        switch (m_eAlignment) {
            case LayoutAlignment::TopCenter:
            case LayoutAlignment::MiddleCenter:
            case LayoutAlignment::BottomCenter:
                x = m_fPaddingLeft + (contentWidth - childWidth) / 2.0f;
                break;
            case LayoutAlignment::TopRight:
            case LayoutAlignment::MiddleRight:
            case LayoutAlignment::BottomRight:
                x = width - m_fPaddingRight - childWidth;
                break;
            default:
                x = m_fPaddingLeft;
                break;
        }
        
        GameObject* obj = node->m_pComponent->GetGameObject();
        if (obj != nullptr) {
            Transform* tf = obj->GetTransform();
            if (tf != nullptr) {
                tf->tPosition.X = x;
                tf->tPosition.Y = y;
                tf->tRect.W = (int)childWidth;
                tf->tRect.H = (int)childHeight;
            }
        }
        
        y += childHeight + m_fSpacing;
    }
}

GridLayout::GridLayout(const char* name)
    : Layout(name)
    , m_nColumns(2)
    , m_nRows(2)
    , m_fCellWidth(100.0f)
    , m_fCellHeight(100.0f)
{
}

GridLayout::~GridLayout() {
}

void GridLayout::SetColumns(int cols) {
    m_nColumns = cols > 0 ? cols : 1;
}

void GridLayout::SetRows(int rows) {
    m_nRows = rows > 0 ? rows : 1;
}

void GridLayout::SetCellSize(float width, float height) {
    m_fCellWidth = width;
    m_fCellHeight = height;
}

void GridLayout::CalculateLayout(float width, float height) {
    (void)width;
    (void)height;
    
    float x = m_fPaddingLeft;
    float y = m_fPaddingTop;
    
    for (size_t i = 0; i < m_Children.size(); i++) {
        LayoutNode* node = m_Children[i];
        
        int col = i % m_nColumns;
        int row = i / m_nColumns;
        
        float cellX = x + col * (m_fCellWidth + m_fSpacing);
        float cellY = y + row * (m_fCellHeight + m_fSpacing);
        
        GameObject* obj = node->m_pComponent->GetGameObject();
        if (obj != nullptr) {
            Transform* tf = obj->GetTransform();
            if (tf != nullptr) {
                tf->tPosition.X = cellX;
                tf->tPosition.Y = cellY;
                tf->tRect.W = (int)m_fCellWidth;
                tf->tRect.H = (int)m_fCellHeight;
            }
        }
    }
}

RelativeLayout::RelativeLayout(const char* name)
    : Layout(name)
{
}

RelativeLayout::~RelativeLayout() {
}

void RelativeLayout::CalculateLayout(float width, float height) {
    (void)width;
    (void)height;
}

FlowLayout::FlowLayout(const char* name)
    : Layout(name)
    , m_fWrapWidth(800.0f)
{
}

FlowLayout::~FlowLayout() {
}

void FlowLayout::SetWrapWidth(float width) {
    m_fWrapWidth = width;
}

void FlowLayout::CalculateLayout(float width, float height) {
    (void)height;
    
    float x = m_fPaddingLeft;
    float y = m_fPaddingTop;
    float rowHeight = 0.0f;
    
    for (size_t i = 0; i < m_Children.size(); i++) {
        LayoutNode* node = m_Children[i];
        
        if (x + node->m_fPreferredWidth > m_fWrapWidth - m_fPaddingRight && x > m_fPaddingLeft) {
            x = m_fPaddingLeft;
            y += rowHeight + m_fSpacing;
            rowHeight = 0.0f;
        }
        
        GameObject* obj = node->m_pComponent->GetGameObject();
        if (obj != nullptr) {
            Transform* tf = obj->GetTransform();
            if (tf != nullptr) {
                tf->tPosition.X = x;
                tf->tPosition.Y = y;
                tf->tRect.W = (int)node->m_fPreferredWidth;
                tf->tRect.H = (int)node->m_fPreferredHeight;
            }
        }
        
        if (node->m_fPreferredHeight > rowHeight) {
            rowHeight = node->m_fPreferredHeight;
        }
        
        x += node->m_fPreferredWidth + m_fSpacing;
    }
}

} // namespace V1SDL
} // namespace HGEngine
