#pragma once

#include <vector>
#include <string>

#include "../Engine/HGComponent.h"
#include "Transform.hpp"

namespace HGEngine {
namespace V1SDL {

/// \brief Layout alignment
enum class LayoutAlignment {
    TopLeft,
    TopCenter,
    TopRight,
    MiddleLeft,
    MiddleCenter,
    MiddleRight,
    BottomLeft,
    BottomCenter,
    BottomRight,
    Stretch
};

/// \brief Layout node base
class LayoutNode {
public:
    HG::HGComponent* m_pComponent;
    int m_nIndex;
    float m_fMinWidth;
    float m_fMinHeight;
    float m_fMaxWidth;
    float m_fMaxHeight;
    float m_fPreferredWidth;
    float m_fPreferredHeight;
    float m_fLayoutWeight;
    bool m_bExpand;
    
    LayoutNode() : m_pComponent(nullptr), m_nIndex(0)
        , m_fMinWidth(0), m_fMinHeight(0)
        , m_fMaxWidth(9999), m_fMaxHeight(9999)
        , m_fPreferredWidth(100), m_fPreferredHeight(100)
        , m_fLayoutWeight(1.0f), m_bExpand(false) {}
};

/// \brief Base layout class
class Layout : public HG::HGComponent {
protected:
    std::vector<LayoutNode*> m_Children;
    float m_fSpacing;
    float m_fPaddingLeft;
    float m_fPaddingRight;
    float m_fPaddingTop;
    float m_fPaddingBottom;
    LayoutAlignment m_eAlignment;

public:
    Layout(const char* name);
    virtual ~Layout();

    /// \brief Add child
    void AddChild(HG::HGComponent* child);
    
    /// \brief Remove child
    void RemoveChild(HG::HGComponent* child);
    
    /// \brief Get child count
    int GetChildCount() const { return (int)m_Children.size(); }
    
    /// \brief Get child at index
    HG::HGComponent* GetChild(int index) const;
    
    /// \brief Set spacing
    void SetSpacing(float spacing) { m_fSpacing = spacing; }
    
    /// \brief Get spacing
    float GetSpacing() const { return m_fSpacing; }
    
    /// \brief Set padding
    void SetPadding(float left, float right, float top, float bottom) {
        m_fPaddingLeft = left; m_fPaddingRight = right;
        m_fPaddingTop = top; m_fPaddingBottom = bottom;
    }
    
    /// \brief Set alignment
    void SetAlignment(LayoutAlignment align) { m_eAlignment = align; }
    
    /// \brief Get alignment
    LayoutAlignment GetAlignment() const { return m_eAlignment; }
    
    /// \brief Perform layout
    virtual void PerformLayout() = 0;

    /// \brief Get component type name
    const char* GetTypeName() const override { return "Layout"; }

    /// \brief Get component type id
    size_t GetTypeId() const override { return typeid(Layout).hash_code(); }
};

/// \brief Horizontal layout - arranges children in a row
class HorizontalLayout : public Layout {
private:
    float m_fChildHeight;

public:
    HorizontalLayout(const char* name);
    virtual ~HorizontalLayout();

    /// \brief Set child height (for alignment)
    void SetChildHeight(float height) { m_fChildHeight = height; }
    
    /// \brief Get child height
    float GetChildHeight() const { return m_fChildHeight; }
    
    /// \brief Perform layout
    void PerformLayout() override;
};

/// \brief Vertical layout - arranges children in a column
class VerticalLayout : public Layout {
private:
    float m_fChildWidth;

public:
    VerticalLayout(const char* name);
    virtual ~VerticalLayout();

    /// \brief Set child width (for alignment)
    void SetChildWidth(float width) { m_fChildWidth = width; }
    
    /// \brief Get child width
    float GetChildWidth() const { return m_fChildWidth; }
    
    /// \brief Perform layout
    void PerformLayout() override;
};

/// \brief Grid layout - arranges children in a grid
class GridLayout : public Layout {
private:
    int m_nColumns;
    int m_nRows;
    float m_fCellWidth;
    float m_fCellHeight;

public:
    GridLayout(const char* name);
    virtual ~GridLayout();

    /// \brief Set grid dimensions
    void SetGridSize(int columns, int rows) { m_nColumns = columns; m_nRows = rows; }
    
    /// \brief Get columns
    int GetColumns() const { return m_nColumns; }
    
    /// \brief Get rows
    int GetRows() const { return m_nRows; }
    
    /// \brief Set cell size
    void SetCellSize(float width, float height) { m_fCellWidth = width; m_fCellHeight = height; }
    
    /// \brief Get cell width
    float GetCellWidth() const { return m_fCellWidth; }
    
    /// \brief Get cell height
    float GetCellHeight() const { return m_fCellHeight; }
    
    /// \brief Perform layout
    void PerformLayout() override;
};

/// \brief Relative layout - positions children relative to each other or parent
class RelativeLayout : public Layout {
public:
    RelativeLayout(const char* name);
    virtual ~RelativeLayout();

    /// \brief Perform layout
    void PerformLayout() override;
};

/// \brief Flow layout - wraps children to next line when space runs out
class FlowLayout : public Layout {
private:
    float m_fWrapWidth;
    float m_fWrapHeight;
    int m_nMaxPerLine;

public:
    FlowLayout(const char* name);
    virtual ~FlowLayout();

    /// \brief Set wrap bounds
    void SetWrapBounds(float width, float height) { m_fWrapWidth = width; m_fWrapHeight = height; }
    
    /// \brief Set max items per line
    void SetMaxPerLine(int max) { m_nMaxPerLine = max; }
    
    /// \brief Get max per line
    int GetMaxPerLine() const { return m_nMaxPerLine; }
    
    /// \brief Perform layout
    void PerformLayout() override;
};

} // namespace V1SDL
} // namespace HGEngine
