#pragma once

#include "../Geometry.hpp"
#include "../GameObject.h"
#include "../Transform.hpp"

#ifdef HGENGINE_GUI
namespace HGEngine {
namespace V1SDL {
namespace UI {
//enum UIStatus : un8 {
//	HGUI_IDLE,
//	HGUI_ON_CONTENT_UPDATE,
//	HGUI_ON_CLICK,
//	HGUI_ON_HOVER,
//	HGUI_ON_LEAVE,
//};

class UIContent {
public:
	HG_INLINE static const char* IDF() {
		return "HGUIContent";	
	}
};

class ButtonContent : public HG::HGComponent {
private:
public:
	virtual void SetLabelRect() {
		auto* r = &m_pGameObject->GetComponent<Label>()->m_tRect;
		*r = m_pGameObject->GetComponent<Transform>()->ToHGRectGlobal();
		r->X = 0; r->Y = 0;
		m_pGameObject->GetComponent<Label>()->m_tRect.Set( r->Scale( 0.7f ) );
	}
	RdRect m_tRR;
	HG::Math::HGColor m_tBackgroundColor;
	HG::Math::HGColor m_tBorderColor;
	HG::Math::HGColor m_tClickedColor;
	HG::Math::HGColor m_tHoverColor;
	virtual int OnRender() {
		auto tr = m_pGameObject->GetComponent<Transform>();
		m_tRR.Set( tr->ToHGRectGlobal() );
		if( tr->IsMouseIn() ) {
			if( HG_ENGINE_INPUT()->IsLeftMousePressed() ) {
				HG_ENGINE_RENDERER2D->SetDrawColor( m_tClickedColor );
			} else {
				HG_ENGINE_RENDERER2D->SetDrawColor( m_tHoverColor );
			}
		} else {
			HG_ENGINE_RENDERER2D->SetDrawColor( m_tBackgroundColor );
		}
		m_tRR.Fill( HG_ENGINE_RENDERER2D );
		HG_ENGINE_RENDERER2D->SetDrawColor( m_tBorderColor );
		m_tRR.Draw( HG_ENGINE_RENDERER2D );
		return 0;
	}
	ButtonContent() : HG::HGComponent(), m_tRR(), m_tBackgroundColor(), m_tBorderColor(), m_tClickedColor(), m_tHoverColor() { 
	}
	ButtonContent( const char* strName ) : HG::HGComponent( strName ), m_tRR(), m_tBackgroundColor(), m_tBorderColor(), m_tClickedColor(), m_tHoverColor() {
		m_tBackgroundColor.Set( 100, 100, 100, 255 );
		m_tBorderColor.Set( 88, 88, 88, 255 );
		m_tClickedColor.Set( 66, 66, 66, 255 );
		m_tHoverColor.Set( 88, 88, 88, 255 );
	}
};

class CheckboxContent : public ButtonContent {
private:
	n32 m_paddingCheckboxX;
	n32 m_centerY;
public:
	void SetLabelRect() override {
		auto* r = &m_pGameObject->GetComponent<Label>()->m_tRect;
		auto& trr = m_pGameObject->GetComponent<Transform>()->ToHGRectGlobal();
		m_centerY = trr.Y;
		trr.Scale( 0.7f );
		trr.X = 30; trr.Y = trr.Y - m_centerY;
		m_centerY = trr.Y;
		m_pGameObject->GetComponent<Label>()->m_tRect.Set( trr );
	}
	RdRect m_tRRCheckbox;
	int OnRender() override {
		auto tr = m_pGameObject->GetComponent<Transform>();
		m_tRR.Set( tr->ToHGRectGlobal() );
		m_tRRCheckbox.Set( tr->ToHGRectGlobal() );
		m_tRRCheckbox.X += m_paddingCheckboxX;
		m_tRRCheckbox.Y += m_centerY;
		m_tRRCheckbox.W = 20;
		m_tRRCheckbox.H = 20;
		if( tr->IsMouseIn() ) {
			if( HG_ENGINE_INPUT()->IsLeftMousePressed() ) {
				HG_ENGINE_RENDERER2D->SetDrawColor( m_tClickedColor );
			} else {
				HG_ENGINE_RENDERER2D->SetDrawColor( m_tHoverColor );
			}
		} else {
			HG_ENGINE_RENDERER2D->SetDrawColor( m_tBackgroundColor );
		}
		m_tRR.Fill( HG_ENGINE_RENDERER2D );
		HG_ENGINE_RENDERER2D->SetDrawColor( m_tBorderColor );
		m_tRR.Draw( HG_ENGINE_RENDERER2D );
		
		// draw checkbox
		HG_ENGINE_RENDERER2D->SetDrawColor( 0,0,0,255 );
		m_tRRCheckbox.Draw( HG_ENGINE_RENDERER2D );

		if( IsChecked ) {
			HG_ENGINE_RENDERER2D->DrawLine( 
				m_tRRCheckbox.Left(), m_tRRCheckbox.Top(),
				m_tRRCheckbox.Right(), m_tRRCheckbox.Bottom() );
			HG_ENGINE_RENDERER2D->DrawLine(
				m_tRRCheckbox.Right(), m_tRRCheckbox.Top(),
				m_tRRCheckbox.Left(), m_tRRCheckbox.Bottom() );
		}
		return 0;
	}
	bool IsChecked;
	CheckboxContent() : IsChecked( false ), m_paddingCheckboxX( 5 ) {
	}
	CheckboxContent( const char* strName ) : ButtonContent( strName ), IsChecked( false ), m_paddingCheckboxX( 5 ) {
	}
};

}
}
}
#endif