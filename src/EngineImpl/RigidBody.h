#pragma once

#include "../Engine/HGEvent.hpp"
#include "../Engine/HGComponent.h"

namespace HGEngine {
namespace V1SDL {
/// \brief �������
/// \note
/// * ����һ�� RigidBody�����Ӹ������ GameObject ��������� Collision �� Transform
/// * ����� RigidBody ���ƶ����岻Ӧ��ֱ�Ӳ��� RigidBody
class RigidBody : public HG::HGComponent {
public:
	f32 Mass;
	f32 LinearDrag;
	f32 AngularDrag;
	f32 GravityDrag;
	bool IsFrozen;
	HG::Math::HGVec2<f32> Velocity;
	void Proc( f32 deltaTime );
	void MovePosition( HG::Math::HGVec2<f32> vec2d );
	void MovePosition( const f32 x, const f32 y );

	RigidBody( const char* strName ) : HGComponent( strName ), IsFrozen( false ) {

	}
};
}
}