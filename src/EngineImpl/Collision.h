#pragma once

#include <Math.hpp>
#include "../Engine/HGEvent.h"
#include "../Engine/HGComponent.h"
#include "GameObject.h"
#include "Layer.h"

namespace HGEngine {
namespace V1SDL {
/// \brief 碰撞组件
/// \note 
/// * 如果要以父类的形式获取，则代码必须写作 GetComponent<Collision>("Collision")
/// 且 Collision 在初始化时必须
class Collision : public HG::HGComponent {
protected:
	virtual char checkWhichSideCol( GameObject* pRectColObj ) = 0;
	virtual bool CanLeave(GameObject* pRectColObj) = 0;

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
protected: 
	char checkWhichSideCol( GameObject* pRectColObj ) override;
	bool CanLeave(GameObject* pRectColObj) override;

public:
	HG::Math::HGRect Rect;
	enum HG_BOXCOLLISION_SIDE : char {
		HG_BC_TOP,
		HG_BC_BOTTOM,
		HG_BC_LEFT,
		HG_BC_RIGHT,
		HG_BC_NONE,
	};
	bool DoCheck( GameObject* pTarget ) override;
	void SetCollisionBoundingByTransform() override;
	explicit BoxCollision( const char* strName ) : Collision( strName ), Rect() {}
	virtual ~BoxCollision(){}
};

class CircleCollision : public Collision {
protected:
	char checkWhichSideCol( GameObject* pRectColObj ) override { return 0; };
	bool CanLeave(GameObject* pRectColObj) override { return false; };

public:
	HG::Math::HGCircle<double> Circle;
	
	bool DoCheck( GameObject* pTarget ) override;
	void SetCollisionBoundingByTransform() override{}
	explicit CircleCollision( const char* strName ) : Collision( strName ), Circle() {}
	virtual ~CircleCollision(){}
};

}

}