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
/// 形体变换，包含位置与大小信息<br>
/// figure, which contains position and size infomation
class Transform : public HG::HGComponent {
protected:
	HG::Math::HGRect m_tRectLocal;
	HG::Math::HGRect m_tRectGlobal;
	SDL_Rect m_tSRLocal;
	SDL_Rect m_tSRGlobal;
	SDL_Point m_tPt;
public:
	/// @brief 判断鼠标是否落在该Transform中
	/// @return 是否落在其中 
	bool IsMouseIn() {
		auto pc = HG_ENGINE_CURRENT_SCENE()->GetMainCamera();
		HG::Math::HGVec2<float>& gpos = pc->GetComponent<Transform>()->tPosition;
		return ( ToHGRectGlobal().IsIn( HG_ENGINE_INPUT()->GetGlobalMousePos( gpos.X, gpos.Y ) ) );
	}
	/// \brief 
	/// 局部坐标系中的位置，也可理解为对原物体的裁剪位置<br>
	/// size in local area, also it's the clip start point of this object
	HG::Math::HGVec2<float> tLocalPos;
	/// \brief 
	/// 局部坐标系中的大小，也可理解为对原物体的裁剪大小<br>
	/// size in global area,  also it's the clip size of this object
	HG::Math::HGSize<un32> tLocalRect;
	/// \brief 
	/// 世界坐标系中的位置<br>
	/// position in global area
	HG::Math::HGVec2<float> tPosition;
	/// \brief 
	/// 世界坐标系中的大小，也可理解为渲染至全局的大小<br>
	/// size in global area, also it's the ouput rendering size
	HG::Math::HGSize<un32> tRect;

	double f64Angle;
	HG::Math::HGPos tRotateCenter;

	void SetGlobalRect( const HG::Math::HGRect& rect ) {
		tRect.H = rect.H;
		tRect.W = rect.W;
		tPosition.X = rect.X;
		tPosition.Y = rect.Y;
	}
	void SetLocalRect( const HG::Math::HGRect& rect ) {
		tLocalRect.H = rect.H;
		tLocalRect.W = rect.W;
		tLocalPos.X = rect.X;
		tLocalPos.Y = rect.Y;
	}

	bool IsSameGlobalRect( const HG::Math::HGRect& rect ) const {
		return tRect.H == rect.H &&
			tRect.W == rect.W &&
			tPosition.X == rect.X &&
			tPosition.Y == rect.Y;
	}
public:
	/// \brief
	/// * 重置旋转中心为Rect中心
	/// * reset the rotate center to rect center
	void ResetRotateCenter() {
		// HG::Math::Center( tPosition, tRect, tRotateCenter );
		tRotateCenter.X = tRect.W / 2;
		tRotateCenter.Y = tRect.H / 2;
	}

	/// \brief 
	///	* 获取旋转后的矩形
	/// * get rect after being rotated
	///	\param tRect 
	///	* 旋转后的矩形 以四个二维向量作为保存
	/// * rect after being rotated which contains 4 vectors
	void GetRotatedRectGlobal(
		HG::Math::HGShape<float>& tRect ) {
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
	/// \brief 
	/// * 将物体于世界的坐标与大小归零
	/// * reset the global position and size to zero
	void ZeroGlobal() {
		tPosition.X = 0;
		tPosition.Y = 0;
		tRect.H = 0;
		tRect.W = 0;
	}
	/// \brief 
	/// * 将物体于本地的坐标与大小归零
	/// * reset the local position and size to zero
	void ZeroLocal() {
		tLocalPos.X = 0;
		tLocalPos.Y = 0;
		tLocalRect.H = 0;
		tLocalRect.W = 0;
	}
	Transform() : HG::HGComponent() { }
	Transform( const char* strName ) : HG::HGComponent( strName ) {
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