#pragma once

#include "../Engine/HGEvent.h"
#include "../Engine/HGComponent.h"
#include "../Core/Math.h"
#include "GameObject.h"
#include "Layer.h"

namespace __HGImpl {
namespace V1SDL {
/// \brief 碰撞组件
/// \note 
/// * 如果要以父类的形式获取，则代码必须写作 GetComponent<Collision>("Collision")
/// 且 Collision 在初始化时必须
class Collision : public HG::HGComponent {
protected:
	Layer *getTargetLayer() const { return m_pGameObject->GetLayer(); }
	void procCollided( bool collided, GameObject* pObj );
	std::list<GameObject *> m_lColList;

public:
	/// \brief
	/// 以 GameObject 的 Transform 来构造碰撞形体的大小
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