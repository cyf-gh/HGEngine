#pragma once

#include "../Core/Math.h"
#include "../Engine/HGComponent.h"

namespace __HGImpl {
namespace V1SDL {

/// \~chinese \brief 形体变换，包含位置与大小信息 
/// \~english \brief figure, which contains position and size infomation
class Transform : public HG::V1SDL::HGComponent {
public:
	__HGImpl::Math::HGVec2<float> tLocalPos;
	__HGImpl::Math::HGSize tLocalRect;
	__HGImpl::Math::HGVec2<float> tPosition;
	/// \~chinese \brief 世界坐标系中的大小，也可理解为渲染至全局的大小
	/// \~english \brief size in global area, also it's the ouput rendering size 
	__HGImpl::Math::HGSize tRect;

	double f64Angle;
	__HGImpl::Math::HGPos tRotateCenter;

	void ResetRotateCenter() {
		__HGImpl::Math::Center( tPosition, tRect, tRotateCenter );
	}

	/// \~chinese \brief 获取旋转后的矩形
	/// \~english \brief get rect after being rotated
	///	\~chinese \param tRect 旋转后的矩形 以四个二维向量作为保存
	/// \~english \param tRect rect after being rotated which contains 4 vectors
	void GetRotatedRectGlobal( 
		__HGImpl::Math::HGShape<float>& tRect ) {
		this->ToHGRectGlobal().ToShape<float>(tRect);
		tRect.Rotate( f64Angle, tRotateCenter.ToVec2<float>() );
	}

	HG_INLINE SDL_Point ToSDLPoint() {
		return SDL_Point {
			.x = tRotateCenter.X,
			.y = tRotateCenter.Y
		};
	}

	HG_INLINE SDL_Rect ToSDLRectGlobal() {
		return SDL_Rect {
			.x = (int)tPosition.X,
			.y = (int)tPosition.Y,
			.w = static_cast< int >( tRect.W ),
			.h = static_cast< int >( tRect.H ),
		};
	}
	HG_INLINE SDL_Rect ToSDLRectLocal() {
		return SDL_Rect {
			.x = (int)tLocalPos.X,
			.y = (int)tLocalPos.Y,
			.w = static_cast< int >( tLocalRect.W ),
			.h = static_cast< int >( tLocalRect.H ),
		};
	}

	HG_INLINE __HGImpl::Math::HGRect ToHGRectLocal() {
		return __HGImpl::Math::HGRect {
			.X = static_cast< n32 >( tLocalPos.X ),
			.Y = static_cast< n32 >( tLocalPos.Y ),
			.H = static_cast< un32 >( tLocalRect.H ),
			.W = static_cast< un32 >( tLocalRect.W ),
		};
	}
	HG_INLINE __HGImpl::Math::HGRect ToHGRectGlobal() {
		return __HGImpl::Math::HGRect {
			.X = static_cast< n32 >( tPosition.X ),
			.Y = static_cast< n32 >( tPosition.Y ),
			.H = static_cast< un32 >( tRect.H ),
			.W = static_cast< un32 >( tRect.W ),
		};
	}

	void ZeroGlobal() {
		tPosition.X = 0;
		tPosition.Y = 0;
		tRect.H = 0;
		tRect.W = 0;
	}
	void ZeroLocal() {
		tLocalPos.X = 0;
		tLocalPos.Y = 0;
		tLocalRect.H = 0;
		tLocalRect.W = 0;
	}
	Transform( const char* strName ) : HG::V1SDL::HGComponent( strName ) {
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

}}