#pragma once

#include <unordered_map>
#include <box2d\box2d.h>
#include "../Engine/HGEvent.hpp"
#include "../Engine/HGComponent.h"

namespace HG { namespace Math { struct HGShape; } }

namespace HGEngine {
namespace V1SDL {

class HGWorld;
class Transform;
/// \brief box2d刚体组件
/// \note
/// \see 
/// HGEngine::V1SDL::HGWorld
/// \todo
/// 检测碰撞
class RigidBodyB2 : public HG::HGComponent {
public:
private:
	b2Body*				pBody;
	b2Fixture*			pFixture;
	b2FixtureDef		tFixtureDef;
	b2BodyDef			tBodyDef;

	b2PolygonShape		tBox;
	b2CircleShape		tCircle;

	HGWorld*			pWorld;
	HG::Math::HGShape*	pShape;	

public:
	bool IsSyncTransform;
	/// \brief 将body的数据同步至Transform组件
	/// \param pTr 希望同步的数据
	void Sync2Transform( Transform* pTr = nullptr );
	void ApplyForce( const b2Vec2& vec );
	void SetAwake( bool isAwake );
	/// \brief 将构建一个与Transform同样尺寸位置的矩形Rigidbody
	/// \param pworld		所处的世界
	/// \param tr			Transform组件
	/// \param isDynmaic	如为false则不动
	/// \param strName		component名字
	explicit RigidBodyB2( HGWorld *pworld, Transform *tr, bool isDynmaic, const char* strName );
	explicit RigidBodyB2( HGWorld* pworld, const b2BodyDef& tBodyDef, const b2FixtureDef& tFixtureDef, HG::Math::HGShape *pShape, const char* strName );
	virtual ~RigidBodyB2();
};
#ifdef HGENGINE_EXPERIMENT
#pragma region HG
/// \brief 刚体组件
/// \note
/// * 对于一个 RigidBody，附加该组件的 GameObject 必须添加了 Collision 和 Transform
/// * 在添加 RigidBody 后，移动物体不应该直接操作 RigidBody
/// \deprecated 
/// 已弃用，请使用RigidBodyB2替代
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
		HG_ASSERT( false );
	}
	RigidBody() :HGComponent() {}
};
#pragma endregion
#endif
}
}