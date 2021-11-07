#pragma once

#include <Math.hpp>
#include <vector>
#include "../Engine/HGComponent.h"
#include "GameObject.h"
#include "Geometry.hpp"
#include "Transform.hpp"

namespace HGEngine {
namespace V1SDL {

class Effect : public HG::HGComponent {
	RdRect m_tRr;
	f32 m_f32PlayedTime;
public:
	enum Fading : un8 {
		HG_EFFECT_FADING_OUT,
		HG_EFFECT_FADING_IN
	};
	HG_COMPONENT_RENDERABLE
	un8 Alpha;
	HG::Math::HGColor m_tSolidColor;
	/// @brief 播放特效时长
	f32 m_f32PlayTime;
	HGEngine::V1SDL::Texture* GetRenderTarget( HGEngine::V1SDL::Renderer2D* pRd ) override {
		auto& gr = m_pGameObject->GetComponent<Transform>()->ToHGRectGlobal();
		m_tRr.Set( gr );
		pRd->SetDrawColor( m_tSolidColor );
		m_tRr.Fill( pRd );
		return nullptr;
	}
	void Reset( un8 a = 255 ) { m_f32PlayedTime = 0.f; m_tSolidColor.A = a; }
	/// @brief 
	/// @param f32DeltaTime 
	/// @param eFlag 
	/// @return 
	///	* is playing finished
	/// * 播放是否完成
	bool Play( const f32 f32DeltaTime, const un8 eFlag = HG_EFFECT_FADING_OUT ) {
		if( m_f32PlayedTime >= m_f32PlayTime ) {
			return true;
		}
		m_f32PlayedTime += f32DeltaTime;
		m_f32PlayedTime = m_f32PlayedTime > m_f32PlayTime ? m_f32PlayTime : m_f32PlayedTime;
		auto r = m_f32PlayedTime / m_f32PlayTime;
		switch( eFlag ) {
		case HG_EFFECT_FADING_IN:
			m_tSolidColor.A = 255 * ( 1.f - r );
		break;
		case HG_EFFECT_FADING_OUT:
			m_tSolidColor.A = 255 * r;
		break;
		default:
		break;
		}
		return false;
	}
	Effect( const f32 f32PlayTime = 1.8f ) : HG::HGComponent(), Alpha( 255 ), m_tSolidColor(), m_f32PlayTime( f32PlayTime ), m_f32PlayedTime( 99999.f ) {
		nRenderIndex = HG::HGRenderableComponentSeq::EFFECT;
	}
	Effect( const char* strName, const f32 f32PlayTime = 1.8f ) : HG::HGComponent( strName ), Alpha( 255 ), m_tSolidColor(), m_f32PlayTime( f32PlayTime ), m_f32PlayedTime( 99999.f ) {
		nRenderIndex = HG::HGRenderableComponentSeq::EFFECT;
	}
};

}
}