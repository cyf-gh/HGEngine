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

namespace HGEngine {
namespace V1SDL {
class GUI : HG::Memory::NonCopyable {
protected:
	friend Scene;
	un32 unUIIndex;
	un32 unCurrentHoverGUIUID;
protected:
	std::vector<GameObject*> m_vecGUIs;
	Scene *m_pScene;

public:
	std::string strFontName;
	HG::Math::HGPos tGlobalMousePos;
	un32 GetCurrentUIIndex() const { return unUIIndex; }

	bool Button( const char* strText, const HG::Math::HGRect& tRect ) {
		bool isClick = false;
		bool islfDown = HG_ENGINE_INPUT()->IsLeftMousePressed();
		const un32 UICount = unUIIndex + 1;
		
		if( UICount > m_vecGUIs.size() ) {
			GameObject* pbtn = new GameObject( std::format("ui_btn_{}", unUIIndex ).c_str() );
			auto ptr = pbtn->GetComponent<Transform>();
			ptr->SetGlobalRect( tRect );
			pbtn->AddComponent( new Label( "Label", strText, strFontName.c_str() ) );
			pbtn->Enable();
			m_vecGUIs.push_back( pbtn );
		} else {
			if( !islfDown ) {
				unCurrentHoverGUIUID = 0;
				return false;
			}
			auto ptr = m_vecGUIs[unUIIndex]->GetComponent<Transform>();
			auto plb = m_vecGUIs[unUIIndex]->GetComponent<Label>();
			if( !( ptr->IsSameGlobalRect( tRect ) && plb->Text._Equal( strText ) ) ) {
				ptr->SetGlobalRect( tRect );
				plb->Text = strText;
			}
			isClick = ptr->IsMouseIn() && islfDown;
			if( unCurrentHoverGUIUID == 0 ) {
				unCurrentHoverGUIUID = m_vecGUIs[unUIIndex]->UID;
			} else {
				isClick = false;
			}
		}
		++unUIIndex;
		return isClick;
	}
	GUI( Scene* pSc ) : unUIIndex( 0 ), m_pScene( pSc ), unCurrentHoverGUIUID( 0 ) { }

private:

};

}
}