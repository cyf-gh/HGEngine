#pragma once

#include <Math.hpp>
#include <SDL.h>
#include "../Engine/HGComponent.h"

namespace HGEngine {
namespace V1SDL {

/// \brief 
/// ����任������λ�����С��Ϣ<br>
/// figure, which contains position and size infomation
class Transform : public HG::HGComponent {
public:
	/// \brief 
	/// �ֲ�����ϵ�е�λ�ã�Ҳ�����Ϊ��ԭ����Ĳü�λ��<br>
	/// size in local area, also it's the clip start point of this object
	HG::Math::HGVec2<float> tLocalPos;
	/// \brief 
	/// �ֲ�����ϵ�еĴ�С��Ҳ�����Ϊ��ԭ����Ĳü���С<br>
	/// size in global area,  also it's the clip size of this object
	HG::Math::HGSize<un32> tLocalRect;
	/// \brief 
	/// ��������ϵ�е�λ��<br>
	/// position in global area
	HG::Math::HGVec2<float> tPosition;
	/// \brief 
	/// ��������ϵ�еĴ�С��Ҳ�����Ϊ��Ⱦ��ȫ�ֵĴ�С<br>
	/// size in global area, also it's the ouput rendering size
	HG::Math::HGSize<un32> tRect;

	double f64Angle;
	HG::Math::HGPos tRotateCenter;

public:
	/// \brief
	/// * ������ת����ΪRect����
	/// * reset the rotate center to rect center
	void ResetRotateCenter() {
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
		HG::Math::HGShape<float>& tRect ) {
		this->ToHGRectGlobal().ToShape<float>( tRect );
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
			.x = ( int ) tPosition.X,
			.y = ( int ) tPosition.Y,
			.w = static_cast< int >( tRect.W ),
			.h = static_cast< int >( tRect.H ),
		};
	}
	HG_INLINE SDL_Rect ToSDLRectLocal() {
		return SDL_Rect {
			.x = ( int ) tLocalPos.X,
			.y = ( int ) tLocalPos.Y,
			.w = static_cast< int >( tLocalRect.W ),
			.h = static_cast< int >( tLocalRect.H ),
		};
	}

	HG_INLINE HG::Math::HGRect ToHGRectLocal() {
		return HG::Math::HGRect {
			.X = static_cast< n32 >( tLocalPos.X ),
			.Y = static_cast< n32 >( tLocalPos.Y ),
			.H = static_cast< un32 >( tLocalRect.H ),
			.W = static_cast< un32 >( tLocalRect.W ),
		};
	}
	HG_INLINE HG::Math::HGRect ToHGRectGlobal() {
		return HG::Math::HGRect {
			.X = static_cast< n32 >( tPosition.X ),
			.Y = static_cast< n32 >( tPosition.Y ),
			.H = static_cast< un32 >( tRect.H ),
			.W = static_cast< un32 >( tRect.W ),
		};
	}
	/// \brief 
	/// * ��������������������С����
	/// * reset the global position and size to zero
	void ZeroGlobal() {
		tPosition.X = 0;
		tPosition.Y = 0;
		tRect.H = 0;
		tRect.W = 0;
	}
	/// \brief 
	/// * �������ڱ��ص��������С����
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