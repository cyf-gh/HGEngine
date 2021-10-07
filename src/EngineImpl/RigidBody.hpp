#pragma once

#include "../Engine/HGEvent.h"
#include "../Engine/HGComponent.h"

namespace __HGImpl {
namespace V1SDL {
/// \brief �������
/// \note
/// * ����һ�� RigidBody�����Ӹ������ GameObject ��������� Collision �� Transform
/// * ����� RigidBody ���ƶ����岻Ӧ��ֱ�Ӳ��� RigidBody
class RigidBody : public HG::HGComponent {
public:
	double Mass;
	double LinearDrag;
	double AngularDrag;
	double GravityScale;
};
}
}