#pragma once

#include <unordered_map>
#include <box2d\box2d.h>
#include "../Engine/HGEvent.hpp"
#include "../Engine/HGComponent.h"

namespace HGEngine {
namespace V1SDL {

class HGWorld;
class Transform;

class RigidBodyB2 : public HG::HGComponent {
public:
	const float PPM = 32.f;
private:
	b2Body* pBody;
	b2Fixture* pFixture;
	b2FixtureDef tFixtureDef;
	b2BodyDef tBodyDef;
	b2PolygonShape tBox;
	HGWorld* pWorld;
public:
	bool IsSyncTransform;
	void Sync2Transform();
	void ApplyForce( const b2Vec2& vec );
	RigidBodyB2( HGWorld *pworld, Transform *tr, bool isDynmaic, const char* strName );
	~RigidBodyB2();
};
/// \brief 刚体组件
/// \note
/// * 对于一个 RigidBody，附加该组件的 GameObject 必须添加了 Collision 和 Transform
/// * 在添加 RigidBody 后，移动物体不应该直接操作 RigidBody
class RigidBody : public HG::HGComponent {
public:
	enum Modes : char {
		Horizon2D = 0,
		Plain2D = 1
	};
	char Mode;
	f32 Mass;
	f32 LinearDrag;
	f32 AngularDrag;
	f32 GravityDrag;
	bool IsFrozen;
	HG::Math::HGVec2<f32> Velocity;
	void Proc( f32 deltaTime );
	void MovePosition( HG::Math::HGVec2<f32> vec2d );
	void MovePosition( const f32 x, const f32 y );

	RigidBody( const char* strName ) : HGComponent( strName ), IsFrozen( false ), Mode( Modes::Plain2D ) {

	}
	RigidBody() :HGComponent() {}
};
}
}