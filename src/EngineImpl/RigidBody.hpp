#pragma once

#include "../Engine/HGEvent.h"
#include "../Engine/HGComponent.h"

namespace __HGImpl {
namespace V1SDL {
/// \brief 刚体组件
/// \note
/// * 对于一个 RigidBody，附加该组件的 GameObject 必须添加了 Collision 和 Transform
/// * 在添加 RigidBody 后，移动物体不应该直接操作 RigidBody
class RigidBody : public HG::HGComponent {
public:
	double Mass;
	double LinearDrag;
	double AngularDrag;
	double GravityScale;
};
}
}