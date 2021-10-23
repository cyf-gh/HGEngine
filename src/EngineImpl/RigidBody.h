#pragma once

#include "../Engine/HGEvent.hpp"
#include "../Engine/HGComponent.h"

namespace HGEngine {
namespace V1SDL {
/// \brief 刚体组件
/// \note
/// * 对于一个 RigidBody，附加该组件的 GameObject 必须添加了 Collision 和 Transform
/// * 在添加 RigidBody 后，移动物体不应该直接操作 RigidBody
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