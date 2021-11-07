#pragma once

#include <format>
#include <Memory.h>
#include <Math.hpp>
#include "../engine/HGInput.hpp"
#include "Camera.h"
#include "GameObject.h"
#include "Scene.h"
#include "Transform.hpp"
#include "Label.hpp"
#include "EngineImpl.h"
#include "Geometry.hpp"
#include "GUI/GUIContent.hpp"

namespace HGEngine {
namespace V1SDL {
class GUI : HG::Memory::NonCopyable {
protected:
	friend Scene;
	un32 m_unUIIndex;
	un32 m_unCurrentHoverGUIUID;
protected:
	std::vector<GameObject*> m_vecGUIs;
	Scene *m_pScene;

	template<class _uicT>
	HG_INLINE void TryCreateGUI( const char* strUIName, const char* strText, const HG::Math::HGRect& tRect,  bool isFixed ) {
		const un32 UICount = m_unUIIndex + 1;
		if( UICount > m_vecGUIs.size() ) {
			GameObject* pUI = new GameObject( std::format( "ui_{}_{}", strUIName, m_unUIIndex ).c_str(), nullptr, isFixed, true );
			auto* ptr = pUI->GetComponent<Transform>();
			ptr->SetGlobalRect( tRect );
			pUI->AddComponent( new Label( "Label", strText, strFontName.c_str() ) );
			pUI->AddComponent( new _uicT( UI::UIContent::IDF() ) );
			pUI->GetComponent<_uicT>()->SetLabelRect();
			pUI->Enable();
			HG_EVENT_BIND( pUI, OnRender ) {
				static_cast< GameObject* >( pThis )->GetComponent<_uicT>( UI::UIContent::IDF() )->OnRender();
				return 0;
			};
			m_vecGUIs.push_back( pUI );
		}
	}
	
	HG_INLINE bool ProcGUI( const char* strText, const HG::Math::HGRect& tRect, HG::pEvent OnLeave, HG::pEvent OnClick, HG::pEvent OnContentUpdate, HG::pEvent OnHover ) {
		bool isClick = false;
		bool islfDown = HG_ENGINE_INPUT()->IsLeftMousePressed();
		auto &ui = m_vecGUIs[m_unUIIndex];

		if( !islfDown ) {
			// OnLeave
			HG_EVENT_CALLRAW_NO_DATA( OnLeave, ui );
			m_unCurrentHoverGUIUID = 0;
			return false;
		}
		auto ptr = m_vecGUIs[m_unUIIndex]->GetComponent<Transform>();
		auto plb = m_vecGUIs[m_unUIIndex]->GetComponent<Label>();
		auto isMouseIn = ptr->IsMouseIn();
		if( !( ptr->IsSameGlobalRect( tRect ) && plb->Text._Equal( strText ) ) ) {
			// GUIUpdate
			HG_EVENT_CALLRAW_NO_DATA( OnContentUpdate, ui );
			ptr->SetGlobalRect( tRect );
			plb->Text = strText;
		}
		isClick = isMouseIn && islfDown;
		if( isClick ) {
			if( m_unCurrentHoverGUIUID != m_vecGUIs[m_unUIIndex]->UID ) {
				// OnClick
				m_unCurrentHoverGUIUID = m_vecGUIs[m_unUIIndex]->UID;
				HG_EVENT_CALLRAW_NO_DATA( OnClick, ui );
			} else {
				// OnHover
				HG_EVENT_CALLRAW_NO_DATA( OnHover, ui );
				isClick = false;
			}
		}
		return isClick;
	}

public:
	std::string strFontName;
	HG::Math::HGPos tGlobalMousePos;
	un32 GetCurrentUIIndex() const { return m_unUIIndex; }
	bool IsVisiable;
	HG::pEvent OnGUI;

	bool Button( const char* strText, const HG::Math::HGRect& tRect, bool isFixed = true ) {
		TryCreateGUI<UI::ButtonContent>( "btn", strText, tRect, isFixed );
		auto isclick = ProcGUI( strText, tRect, nullptr, nullptr ,nullptr, nullptr );
		++m_unUIIndex;
		return isclick;
	}
	bool Checkbox( const char* strText, const HG::Math::HGRect& tRect, bool &isChecked, bool isFixed = true ) {
		TryCreateGUI<UI::CheckboxContent>( "tgbtn", strText, tRect, isFixed );
		bool isClicked = ProcGUI( strText, tRect, nullptr, HG_EVENT_IMPL {
			auto& isC = (static_cast<GameObject*>(pThis)->GetComponent<UI::CheckboxContent>(UI::UIContent::IDF()))->IsChecked; 
			isC = !isC;
			return 0;
		}, nullptr, nullptr );
		auto& ui = m_vecGUIs[m_unUIIndex];
		isChecked = ( static_cast< GameObject* >( ui )->GetComponent<UI::CheckboxContent>( UI::UIContent::IDF() ) )->IsChecked;
		++m_unUIIndex;
		return isClicked;
	}
	GUI( Scene* pSc, bool isVisiable = false ) : m_unUIIndex( 0 ), m_pScene( pSc ), m_unCurrentHoverGUIUID( 0 ), IsVisiable( isVisiable ), OnGUI( nullptr ) { }

private:

};

}
}