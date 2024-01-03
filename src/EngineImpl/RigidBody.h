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
/// \brief box2d�������
/// \note
/// \see 
/// HGEngine::V1SDL::HGWorld
/// \todo
/// �����ײ
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
	/// \brief ��body������ͬ����Transform���
	/// \param pTr ϣ��ͬ��������
	void Sync2Transform( Transform* pTr = nullptr );
	void ApplyForce( const b2Vec2& vec );
	void SetAwake( bool isAwake );
	/// \brief ������һ����Transformͬ���ߴ�λ�õľ���Rigidbody
	/// \param pworld		����������
	/// \param tr			Transform���
	/// \param isDynmaic	��Ϊfalse�򲻶�
	/// \param strName		component����
	explicit RigidBodyB2( HGWorld *pworld, Transform *tr, bool isDynmaic, const char* strName );
	explicit RigidBodyB2( HGWorld* pworld, const b2BodyDef& tBodyDef, const b2FixtureDef& tFixtureDef, HG::Math::HGShape *pShape, const char* strName );
	virtual ~RigidBodyB2();
};
#ifdef HGENGINE_EXPERIMENT
#pragma region HG
/// \brief �������
/// \note
/// * ����һ�� RigidBody�����Ӹ������ GameObject ��������� Collision �� Transform
/// * ����� RigidBody ���ƶ����岻Ӧ��ֱ�Ӳ��� RigidBody
/// \deprecated 
/// �����ã���ʹ��RigidBodyB2���
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