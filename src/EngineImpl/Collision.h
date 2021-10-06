#pragma once

#include "../Engine/HGEvent.h"
#include "../Engine/HGComponent.h"
#include "../Core/Math.h"
#include "GameObject.h"
#include "Layer.h"

namespace __HGImpl {
namespace V1SDL {
/// \brief Åö×²×é¼þ
class Collision : public HG::HGComponent {
protected:
	Layer *getTargetLayer() const { return m_pGameObject->GetLayer(); }
	void procCollided( bool collided, GameObject* pObj );
	std::list<GameObject *> m_lColList;

public:
	virtual bool DoCheck( GameObject* pTarget ) = 0;
	explicit Collision( const char* strName ) : HGComponent( strName ), m_lColList() { }
	virtual ~Collision() {}
};
class BoundingCollision : public Collision {
public:
	HG::Math::HGRect Rect;

	bool DoCheck( GameObject* pTarget ) override;
	explicit BoundingCollision( const char* strName ) : Collision( strName ) {}
	virtual ~BoundingCollision(){}
};

class CircleCollision : public Collision {
public:
	HG::Math::HGCircle<double> Circle;
	
	bool DoCheck( GameObject* pTarget ) override;

	explicit CircleCollision( const char* strName ) : Collision( strName ) {}
	virtual ~CircleCollision(){}
};

}

}