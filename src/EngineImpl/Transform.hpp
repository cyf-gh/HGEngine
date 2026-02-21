#pragma once

#include <Math.hpp>
#include <SDL.h>
#include "../Engine/HGComponent.h"
#include "../engine/HGInput.hpp"
#include "EngineImpl.h"
#include "Scene.h"
#include "Camera.h"

namespace HGEngine {
namespace V1SDL {

/// \brief 
/// ����任������λ�����С��Ϣ<br>
/// figure, which contains position and size infomation
class Transform : public HG::HGComponent {
protected:
	HG::Math::HGRect m_tRectLocal;
	HG::Math::HGRect m_tRectGlobal;
	SDL_Rect m_tSRLocal;
	SDL_Rect m_tSRGlobal;
	SDL_Point m_tPt;
	
	/// \brief Dirty flag - true when transform data changed
	bool m_bIsDirty;
	
	/// \brief Mark transform as dirty (needs recalculation)
	inline void MarkDirty() { m_bIsDirty = true; }
	
	/// \brief Recalculate cached values if dirty
	inline void UpdateCache() {
		if (m_bIsDirty) {
			ToSDLRectGlobal();
			ToSDLRectLocal();
			ToHGRectGlobal();
			ToHGRectLocal();
			m_bIsDirty = false;
		}
	}
	
public:
	/// @brief �ж�����Ƿ����ڸ�Transform��
	/// @return �Ƿ��������� 
	bool IsMouseIn() {
		UpdateCache();
		if( m_pGameObject->IsFixedToCamera() ) {
			return ( ToHGRectGlobal().IsIn( HG_ENGINE_INPUT()->GetGlobalMousePos( 0, 0 ) ) );
		} else {
			auto pc = HG_ENGINE_CURRENT_SCENE()->GetMainCamera();
			HG::Math::HGVec2<float>& gpos = pc->GetComponent<Transform>()->tPosition;
			return ( ToHGRectGlobal().IsIn( HG_ENGINE_INPUT()->GetGlobalMousePos( gpos.X, gpos.Y ) ) );
		}
	}
	/// \brief 
	/// �ֲ�����ϵ�е�λ�ã�Ҳ������Ϊ��ԭ����Ĳü�λ��<br>
	/// size in local area, also it's the clip start point of this object
	HG::Math::HGVec2<float> tLocalPos;
	/// \brief 
	/// �ֲ�����ϵ�еĴ�С��Ҳ������Ϊ��ԭ����Ĳü���С<br>
	/// size in global area,  also it's the clip size of this object
	HG::Math::HGSize<un32> tLocalRect;
	/// \brief 
	/// ��������ϵ�е�λ��<br>
	/// position in global area
	HG::Math::HGVec2<float> tPosition;
	/// \brief 
	/// ��������ϵ�еĴ�С��Ҳ������Ϊ��Ⱦ��ȫ�ֵĴ�С<br>
	/// size in global area, also it's the ouput rendering size
	HG::Math::HGSize<un32> tRect;

	double f64Angle;
	HG::Math::HGPos tRotateCenter;

	void SetGlobalRect( const HG::Math::HGRect& rect ) {
		MarkDirty();
		tRect.H = rect.H;
		tRect.W = rect.W;
		tPosition.X = static_cast< float >( rect.X );
		tPosition.Y = static_cast< float >( rect.Y );
	}
	
	void SetGlobalRect( f32 x, f32 y, un32 w, un32 h ) {
		MarkDirty();
		tRect.H = h;
		tRect.W = w;
		tPosition.X = x;
		tPosition.Y = y;
	}

	void SetLocalRect( const HG::Math::HGRect& rect ) {
		MarkDirty();
		tLocalRect.H = rect.H;
		tLocalRect.W = rect.W;
		tLocalPos.X = static_cast<float>( rect.X );
		tLocalPos.Y = static_cast<float>( rect.Y );
	}

	bool IsSameGlobalRect( const HG::Math::HGRect& rect ) const {
		return tRect.H == rect.H &&
			tRect.W == rect.W &&
			tPosition.X == rect.X &&
			tPosition.Y == rect.Y;
	}
public:
	/// \brief 
	/// * ��ʼ����ת����ΪRect����
	/// * reset the rotate center to rect center
	void ResetRotateCenter() {
		MarkDirty();
		// HG::Math::Center( tPosition, tRect, tRotateCenter );
		tRotateCenter.X = tRect.W / 2;
		tRotateCenter.Y = tRect.H / 2;
	}

	/// \brief 
	///	* ��ȡ��ת��ľ���
	/// * get rect after being rotated
	///	\param tRect 
	///	* ��ת��ľ��� ���ĸ���ά������Ϊ����
	/// * rect after being rotated which contains 4 vectors
	void GetRotatedRectGlobal(
		HG::Math::HGPolygon<float>& tRect ) {
		this->ToHGRectGlobal().ToShape<float>( tRect );
		tRect.Rotate( f64Angle, tRotateCenter.ToVec2<float>() );
	}

	HG_INLINE SDL_Point& ToSDLPoint() {
		m_tPt.x = tRotateCenter.X;
		m_tPt.y = tRotateCenter.Y;
		return m_tPt;
	}

	HG_INLINE SDL_Rect& ToSDLRectGlobal() {
		m_tSRGlobal.x = ( int ) tPosition.X;
		m_tSRGlobal.y = ( int ) tPosition.Y;
		m_tSRGlobal.w = static_cast< int >( tRect.W );
		m_tSRGlobal.h = static_cast< int >( tRect.H );
		return m_tSRGlobal;
	}
	HG_INLINE SDL_Rect& ToSDLRectLocal() {
		m_tSRLocal.x = ( int ) tLocalPos.X;
		m_tSRLocal.y = ( int ) tLocalPos.Y;
		m_tSRLocal.w = static_cast< int >( tLocalRect.W );
		m_tSRLocal.h = static_cast< int >( tLocalRect.H );
		return m_tSRLocal;
	}


	HG_INLINE HG::Math::HGRect& ToHGRectLocal() {
		m_tRectLocal.X = static_cast< n32 >( tLocalPos.X );
		m_tRectLocal.Y = static_cast< n32 >( tLocalPos.Y );
		m_tRectLocal.H = static_cast< un32 >( tLocalRect.H );
		m_tRectLocal.W = static_cast< un32 >( tLocalRect.W );
		return m_tRectLocal;
	}
	HG_INLINE HG::Math::HGRect& ToHGRectGlobal() {
		m_tRectGlobal.X = static_cast< n32 >( tPosition.X );
		m_tRectGlobal.Y = static_cast< n32 >( tPosition.Y );
		m_tRectGlobal.H = static_cast< un32 >( tRect.H );
		m_tRectGlobal.W = static_cast< un32 >( tRect.W );
		return m_tRectGlobal;
	}

	HG_INLINE HG::Math::HGShape *GetLocalShape() { return &m_tRectLocal; }
	HG_INLINE HG::Math::HGShape* GetGlobalShape() { return &m_tRectGlobal; }

	/// \brief 
	/// * ��������������������С����
	/// * reset the global position and size to zero
	void ZeroGlobal() {
		MarkDirty();
		tPosition.X = 0;
		tPosition.Y = 0;
		tRect.H = 0;
		tRect.W = 0;
	}
	/// \brief 
	/// * �������ڱ��ص��������С����
	/// * reset the local position and size to zero
	void ZeroLocal() {
		MarkDirty();
		tLocalPos.X = 0;
		tLocalPos.Y = 0;
		tLocalRect.H = 0;
		tLocalRect.W = 0;
	}

	Transform() : HG::HGComponent(), m_bIsDirty(true) { }
	Transform( const char* strName ) : HG::HGComponent( strName ), m_bIsDirty(true) {
		ZeroLocal();
		ZeroGlobal();
	}
	bool IsZeroGlobal() const {
		return
			tPosition.X == 0 &&
			tPosition.Y == 0 &&
			tRect.H == 0 &&
			tRect.W == 0;
	}
	bool IsZeroLocal() const {
		return
			tLocalPos.X == 0 &&
			tLocalPos.Y == 0 &&
			tLocalRect.H == 0 &&
			tLocalRect.W == 0;
	}
};

}
}