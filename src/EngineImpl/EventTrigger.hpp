#pragma once

#include <Log.h>
#include "../Engine/HGEvent.hpp"
#include "../Engine/HGComponent.h"
#include "../engineImpl/EngineImpl.h"
#include "../engine/HGInput.hpp"
#include "../engineImpl/Scene.h"
#include "Transform.hpp"

namespace HGEngine {
namespace V1SDL {
class EventTrigger : public HG::HGComponent {
private:
	bool m_isFirstDown = true;
	bool m_isDragging = false;
	bool m_isEnter = false;
	bool m_isLfDown = false;
	HG::Math::HGRect tDragBegin;
	HG::Math::HGRect tDragLast;
	inline static GameObject* s_pDragging = nullptr;
	inline static GameObject* s_pForcus = nullptr;

public:
	static GameObject* GetDraggingGameObject() { return s_pDragging; }
	static GameObject* GetFocusingGameObject() { return s_pForcus; }
	/// @brief 
	/// 当使用指针点击时触发
	HG::pEvent PointerClick;
	HG::pEvent PointerExit;
	HG::pEvent PointerEnter;
	HG::pEvent PointerUp;
	HG::pEvent PointerDown;
	/// @brief 每次鼠标移动时触发
	HG::pEvent Drag;
	HG::pEvent Drop;
	HG::pEvent BeginDrag;
	HG::pEvent EndDrag;
	void ProcEvents() {
		auto *input = HG_ENGINE_INPUT();
		auto *pc = HG_ENGINE_CURRENT_SCENE()->GetMainCamera();
		auto *tr = m_pGameObject->GetComponent<Transform>();
		auto& gpos = pc->GetComponent<Transform>()->tPosition;
		auto& currentGPos = input->GetGlobalMousePos( gpos.X, gpos.Y );
		bool islfDown = input->IsLeftMousePressed();
		auto isMouseIn = tr->IsMouseIn();
		if( isMouseIn ) {
			if( !m_isEnter ) {
				s_pForcus = m_pGameObject;
				HG_EVENT_CALLRAW_NO_DATA( PointerEnter, m_pGameObject );
				HG_LOG_DEBUGF( "GameObject[{}] => PointerEnter", m_pGameObject->GetName() );
			}
			m_isEnter = true;
		} else {
			if( m_isEnter ) {
				s_pForcus = nullptr;
				HG_EVENT_CALLRAW_NO_DATA( PointerExit, m_pGameObject );
				HG_LOG_DEBUGF( "GameObject[{}] => PointerExit", m_pGameObject->GetName() );
			}
			m_isEnter = false;
		}
		if( !m_isDragging ) {
			if( isMouseIn && islfDown && !tDragBegin.IsEqual( currentGPos ) && EventTrigger::GetDraggingGameObject() == nullptr ) {
				EventTrigger::s_pDragging = m_pGameObject;
				HG_EVENT_CALLRAW_NO_DATA( BeginDrag, m_pGameObject );
				HG_LOG_DEBUGF( "GameObject[{}] => BeginDrag", m_pGameObject->GetName() );
				m_isDragging = true;
			}
		} else {
			if( !tDragLast.IsEqual( currentGPos ) && ( EventTrigger::GetDraggingGameObject() == m_pGameObject ) ) {
				tDragLast = currentGPos;
				// IsDragging
				HG_EVENT_CALLRAW_NO_DATA( Drag, m_pGameObject );
				HG_LOG_DEBUGF( "GameObject[{}] => Drag", m_pGameObject->GetName() );
			}
		}
		if( isMouseIn ) {
			if( EventTrigger::GetDraggingGameObject() == nullptr ) {
				if( islfDown ) {
					HG_EVENT_CALLRAW_NO_DATA( PointerDown, m_pGameObject );
					//HG_LOG_DEBUGF( "GameObject[{}] => PointerDown", m_pGameObject->GetName() );
					if( m_isFirstDown ) {
						HG_EVENT_CALLRAW_NO_DATA( PointerClick, m_pGameObject );
						HG_LOG_DEBUGF( "GameObject[{}] => PointerClick", m_pGameObject->GetName() );
						tDragBegin = currentGPos;
						tDragLast = tDragBegin;
						m_isFirstDown = false;
					}
				} else {
					if( m_isLfDown ) {
						HG_EVENT_CALLRAW_NO_DATA( PointerUp, m_pGameObject );
						HG_LOG_DEBUGF( "GameObject[{}] => PointerUp", m_pGameObject->GetName() );
						m_isDragging = false;
						m_isFirstDown = true;
						m_isLfDown = false;
					}
				}
			} else {
				if( !islfDown ) {
					HG_EVENT_CALLRAW_NO_DATA( Drop, m_pGameObject );
					HG_LOG_DEBUGF( "GameObject[{}] => Drop", m_pGameObject->GetName() );
					m_isDragging = false;
					m_isFirstDown = true;
					m_isLfDown = false;
				}
			}
		} 
		if( m_isDragging && !islfDown ) {
			HG_EVENT_CALLRAW_NO_DATA( EndDrag, m_pGameObject );
			EventTrigger::s_pDragging = nullptr;
			HG_LOG_DEBUGF( "GameObject[{}] => EndDrag", m_pGameObject->GetName() );
			m_isDragging = false;
			m_isFirstDown = true;
			m_isLfDown = false;
		}
		m_isLfDown = islfDown;
	}
};
}
}