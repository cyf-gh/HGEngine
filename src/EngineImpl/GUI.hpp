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
	std::vector<GameObject*> m_vecGUIs;
	Scene *m_pScene;

public:
	std::string strFontName;
	un32 UIIndex;
	HG::Math::HGPos tGlobalMousePos;

	bool Button( const char* strText, const HG::Math::HGRect& tRect ) {
		const un32 UICount = UIIndex + 1;
		if( UICount > m_vecGUIs.size() ) {
			GameObject* pbtn = new GameObject( std::format("ui_btn_{}", UIIndex ).c_str() );
			auto ptr = pbtn->GetComponent<Transform>();
			ptr->SetGlobalRect( tRect );
			pbtn->AddComponent( new Label( "Label", strText, strFontName.c_str() ) );
		} else {
			auto ptr = m_vecGUIs[UIIndex]->GetComponent<Transform>();
			auto plb = m_vecGUIs[UIIndex]->GetComponent<Label>();
			if( !( ptr->IsSameGlobalRect( tRect ) && plb->Text._Equal( strText ) ) ) {
				ptr->SetGlobalRect( tRect );
				plb->Text = strText;
			}
			return ptr->IsMouseIn();
		}
		++UIIndex;
	}
	GUI( Scene* pSc ) : UIIndex( 0 ), m_pScene( pSc ) { }

private:

};

}
}