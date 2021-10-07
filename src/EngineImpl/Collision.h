#pragma once

#include "../Engine/HGEvent.h"
#include "../Engine/HGComponent.h"
#include "../Core/Math.h"
#include "GameObject.h"
#include "Layer.h"

namespace __HGImpl {
namespace V1SDL {
/// \brief ��ײ���
/// \note 
/// * ���Ҫ�Ը������ʽ��ȡ����������д�� GetComponent<Collision>("Collision")
/// �� Collision �ڳ�ʼ��ʱ����
class Collision : public HG::HGComponent {
protected:
	Layer *getTargetLayer() const { return m_pGameObject->GetLayer(); }
	void procCollided( bool collided, GameObject* pObj );
	std::list<GameObject *> m_lColList;

public:
	/// \brief
	/// �� GameObject �� Transform ��������ײ����Ĵ�С
	virtual void SetCollisionBoundingByTransform() = 0;
	virtual bool DoCheck( GameObject* pTarget ) = 0;
	explicit Collision( const char* strName ) : HGComponent( strName ), m_lColList() { }
	virtual ~Collision() {}
};
class BoxCollision : public Collision {
public:
	HG::Math::HGRect Rect;

	bool DoCheck( GameObject* pTarget ) override;
	void SetCollisionBoundingByTransform() override;
	explicit BoxCollision( const char* strName ) : Collision( strName ), Rect() {}
	virtual ~BoxCollision(){}
};

class CircleCollision : public Collision {
public:
	HG::Math::HGCircle<double> Circle;
	
	bool DoCheck( GameObject* pTarget ) override;
	void SetCollisionBoundingByTransform() override{}
	explicit CircleCollision( const char* strName ) : Collision( strName ), Circle() {}
	virtual ~CircleCollision(){}
};

}

}