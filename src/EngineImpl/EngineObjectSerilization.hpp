#pragma once 
#include <Serialization.h>
#include <Memory.h>
#include "EngineImpl.h"
#include "Scene.h"
#include "GameObject2D.h"
#include "Transform.hpp"
#include "Animation.h"
#include "Collision.h"
#include "Timer.hpp"
#include "RigidBody.h"

namespace HGEngine {
namespace V1SDL {
class GameObjectFactory : HG::Memory::NonCopyable {
public:
	template<typename _T>
	HG_INLINE static _T* CreateByJson( std::string& strJson, const bool isEnable, const char* strObjNewName = "", const char* strObjKey = "Obj" ) {
		rapidjson::Document d;
		d.Parse( strJson.c_str() );
		if( d.HasParseError() ) {
			HG_LOG_FAILED( std::format( "Parse Error: {}", std::to_string( d.GetParseError() ) ).c_str() );
			return nullptr;
		}
		_T* g = new _T();
		HG::Serialization::Unmarshal( ( GameObject& ) *g, strObjKey, d[strObjKey], d );
		if( strcmp( strObjNewName, "" ) != 0 ) {
			g->SetName( strObjNewName );
		}
		auto ps = HGEngine::V1SDL::EngineImpl::GetEngine()->GetCurrentScene();
		if( ps != nullptr ) {
			ps->AttachGameObject( g );
		}
		if( isEnable ) {
			g->Enable();
		}
		return g;
	}
	
};

}
}
namespace HG {
namespace Serialization {

HG_MARSHAL_FULLSPEC( HGEngine::V1SDL::Transform ) {
	HG_MARSHAL_OBJECT_START;
	HG_MARSHAL_OBJECT_SETPROP( t.f64Angle );
	HG_MARSHAL_OBJECT_SETPROP( t.tLocalPos );
	HG_MARSHAL_OBJECT_SETPROP( t.tLocalRect );
	HG_MARSHAL_OBJECT_SETPROP( t.tPosition );
	HG_MARSHAL_OBJECT_SETPROP( t.tRect );
	HG_MARSHAL_OBJECT_SETPROP( t.tRotateCenter );
	HG_MARSHAL_OBJECT_END;
}

HG_UNMARSHAL_FULLSPEC( HGEngine::V1SDL::Transform ) {
	HG_UNMARSHAL_OBJECT_START;
	HG_UNMARSHAL_GETOBJ( t.f64Angle );
	HG_UNMARSHAL_GETOBJ( t.tLocalPos );
	HG_UNMARSHAL_GETOBJ( t.tLocalRect );
	HG_UNMARSHAL_GETOBJ( t.tPosition );
	HG_UNMARSHAL_GETOBJ( t.tRect );
	HG_UNMARSHAL_GETOBJ( t.tRotateCenter );
	HG_UNMARSHAL_OBJECT_END;
}

HG_MARSHAL_FULLSPEC( HGEngine::V1SDL::Animator2D::Frame ) {
	HG_MARSHAL_OBJECT_START;
	HG_MARSHAL_OBJECT_SETPROP( t.tPos );
	HG_MARSHAL_OBJECT_SETPROP( t.tRect );
	HG_MARSHAL_OBJECT_END;
}

HG_UNMARSHAL_FULLSPEC( HGEngine::V1SDL::Animator2D::Frame ) {
	HG_UNMARSHAL_OBJECT_START;
	HG_UNMARSHAL_GETOBJ( t.tPos );
	HG_UNMARSHAL_GETOBJ( t.tRect );
	HG_UNMARSHAL_OBJECT_END;
}

HG_MARSHAL_FULLSPEC( HGEngine::V1SDL::Animator2D ) {
	HG_MARSHAL_OBJECT_START;
	HG_MARSHAL_OBJECT_SETPROP( t.Col );
	HG_MARSHAL_OBJECT_SETPROP( t.Row );
	HG_MARSHAL_OBJECT_SETPROP( t.f32Interval );
	HG_MARSHAL_OBJECT_SETPROP( t.IsIdle );
	HG_MARSHAL_OBJECT_SETPROP( t.m_unIdleFrameIndex );
	HG_MARSHAL_OBJECT_SETPROP( t.m_vecFrames );
	HG_MARSHAL_OBJECT_END;
}

HG_UNMARSHAL_FULLSPEC( HGEngine::V1SDL::Animator2D ) {
	HG_UNMARSHAL_OBJECT_START;
	HG_UNMARSHAL_GETOBJ( t.Col );
	HG_UNMARSHAL_GETOBJ( t.Row );
	HG_UNMARSHAL_GETOBJ( t.f32Interval );
	HG_UNMARSHAL_GETOBJ( t.IsIdle );
	HG_UNMARSHAL_GETOBJ( t.m_unIdleFrameIndex );
	HG_UNMARSHAL_GETOBJ( t.m_vecFrames );
	HG_UNMARSHAL_OBJECT_END;
}

HG_MARSHAL_FULLSPEC( HGEngine::V1SDL::Timer ) {
	HG_MARSHAL_OBJECT_START;
	HG_MARSHAL_OBJECT_SETPROP( t.Active );
	HG_MARSHAL_OBJECT_SETPROP( t.f32Delay );
	HG_MARSHAL_OBJECT_SETPROP( t.f32DelayRest );
	HG_MARSHAL_OBJECT_SETPROP( t.f32Elapsed );
	HG_MARSHAL_OBJECT_SETPROP( t.f32ElpasedLoop );
	HG_MARSHAL_OBJECT_SETPROP( t.f32Interval );
	HG_MARSHAL_OBJECT_END;
}

HG_UNMARSHAL_FULLSPEC( HGEngine::V1SDL::Timer ) {
	HG_UNMARSHAL_OBJECT_START;
	HG_UNMARSHAL_GETOBJ( t.Active );
	HG_UNMARSHAL_GETOBJ( t.f32Delay );
	HG_UNMARSHAL_GETOBJ( t.f32DelayRest );
	HG_UNMARSHAL_GETOBJ( t.f32Elapsed );
	HG_UNMARSHAL_GETOBJ( t.f32ElpasedLoop );
	HG_UNMARSHAL_GETOBJ( t.f32Interval );
	HG_UNMARSHAL_OBJECT_END;
}

HG_MARSHAL_FULLSPEC( HGEngine::V1SDL::RigidBody ) {
	HG_MARSHAL_OBJECT_START;
	HG_MARSHAL_OBJECT_SETPROP( t.AngularDrag );
	HG_MARSHAL_OBJECT_SETPROP( t.GravityDrag );
	HG_MARSHAL_OBJECT_SETPROP( t.IsFrozen );
	HG_MARSHAL_OBJECT_SETPROP( t.LinearDrag );
	HG_MARSHAL_OBJECT_SETPROP( t.Mass );
	HG_MARSHAL_OBJECT_SETPROP( t.Velocity );
	HG_MARSHAL_OBJECT_END;
}

HG_UNMARSHAL_FULLSPEC( HGEngine::V1SDL::RigidBody ) {
	HG_UNMARSHAL_OBJECT_START;
	HG_UNMARSHAL_GETOBJ( t.AngularDrag );
	HG_UNMARSHAL_GETOBJ( t.GravityDrag );
	HG_UNMARSHAL_GETOBJ( t.IsFrozen );
	HG_UNMARSHAL_GETOBJ( t.LinearDrag );
	HG_UNMARSHAL_GETOBJ( t.Mass );
	HG_UNMARSHAL_GETOBJ( t.Velocity );
	HG_UNMARSHAL_OBJECT_END;
}

HG_MARSHAL_FULLSPEC( HGEngine::V1SDL::BoxCollision ) {
	HG_MARSHAL_OBJECT_START;
	HG_MARSHAL_OBJECT_SETPROP( t.Rect );
	HG_MARSHAL_OBJECT_END;
}

HG_UNMARSHAL_FULLSPEC( HGEngine::V1SDL::BoxCollision ) {
	HG_UNMARSHAL_OBJECT_START;
	HG_UNMARSHAL_GETOBJ( t.Rect );
	HG_UNMARSHAL_OBJECT_END;
}


HG_MARSHAL_FULLSPEC( HGEngine::V1SDL::GameObject ) {
	HG_MARSHAL_OBJECT_START;
	auto n = std::string( t.GetName() );
	Marshal( n, "Name", writer );
	Marshal( t.m_vecComponents, "Components", writer );
	HG_MARSHAL_OBJECT_END;
}

HG_UNMARSHAL_FULLSPEC( HGEngine::V1SDL::GameObject ) {
	HG_UNMARSHAL_OBJECT_START;
	std::string Name;
	HG_UNMARSHAL_GETOBJ( Name );
	t.SetName( Name.c_str() );
	Unmarshal( t.m_vecComponents, "Components", d["Components"], rd );
	for( auto& c : t.m_vecComponents ) {
		c->SetGameObject( &t );
	}
	HG_UNMARSHAL_OBJECT_END;
}

#define HG_UNMARSHAL_COMPONENT( COMP_TYPE, NODE_NAME )	\
	if( d.HasMember( NODE_NAME ) ) {					\
		t = new COMP_TYPE(NODE_NAME);					\
		Unmarshal( *static_cast< COMP_TYPE* >( t ), NODE_NAME, d[NODE_NAME], rd );	\
		goto END;										\
	}

#define HG_MARSHAL_GAMEOBJECTSETPROP( COMP_TYPE, NODE_NAME ) \
		if( typeid( COMP_TYPE ).hash_code() == typeid( *t ).hash_code() ) { \
Marshal( *static_cast< COMP_TYPE* >( t ), NODE_NAME, writer ); \
		}

HG_MARSHAL_FULLSPEC( HG::HGComponent* ) {
	HG_MARSHAL_OBJECT_START;
	HG_MARSHAL_GAMEOBJECTSETPROP( HGEngine::V1SDL::Transform, "Transform" );
	HG_MARSHAL_GAMEOBJECTSETPROP( HGEngine::V1SDL::Animator2D, "Animator2D" );
	HG_MARSHAL_GAMEOBJECTSETPROP( HGEngine::V1SDL::RigidBody, "RigidBody" );
	HG_MARSHAL_GAMEOBJECTSETPROP( HGEngine::V1SDL::BoxCollision, "BoxCollision" );
	HG_MARSHAL_GAMEOBJECTSETPROP( HGEngine::V1SDL::Timer, "Timer" );
	HG_MARSHAL_OBJECT_END;
}

HG_UNMARSHAL_FULLSPEC( HG::HGComponent* ) {
	HG_UNMARSHAL_OBJECT_START;
	HG_UNMARSHAL_COMPONENT( HGEngine::V1SDL::Transform, "Transform" );
	HG_UNMARSHAL_COMPONENT( HGEngine::V1SDL::Animator2D, "Animator2D" );
	HG_UNMARSHAL_COMPONENT( HGEngine::V1SDL::RigidBody, "RigidBody" );
	HG_UNMARSHAL_COMPONENT( HGEngine::V1SDL::BoxCollision, "BoxCollision" );
	HG_UNMARSHAL_OBJECT_END;
}

HG_MARSHAL_FULLSPEC( SDL_Color ) {
	HG_MARSHAL_OBJECT_START;
	HG_MARSHAL_SETOBJ( t.a );
	HG_MARSHAL_SETOBJ( t.r );
	HG_MARSHAL_SETOBJ( t.g );
	HG_MARSHAL_SETOBJ( t.b );
	HG_MARSHAL_OBJECT_END;
}

HG_UNMARSHAL_FULLSPEC( SDL_Color ) {
	HG_UNMARSHAL_OBJECT_START;
	HG_UNMARSHAL_GETOBJ( t.a );
	HG_UNMARSHAL_GETOBJ( t.r );
	HG_UNMARSHAL_GETOBJ( t.g );
	HG_UNMARSHAL_GETOBJ( t.b );
	HG_UNMARSHAL_OBJECT_END;
}

HG_MARSHAL_FULLSPEC( HGEngine::V1SDL::GameObjectText ) {
	HG_MARSHAL_OBJECT_START;
	HG_MARSHAL_OBJECT_SETPROP( t.tColor );
	HG_MARSHAL_OBJECT_SETPROP( t.Text );
	HG_MARSHAL_OBJECT_END;
}

HG_UNMARSHAL_FULLSPEC( HGEngine::V1SDL::GameObjectText ) {
	HG_UNMARSHAL_OBJECT_START;
	HG_UNMARSHAL_GETOBJ( t.tColor );
	HG_UNMARSHAL_GETOBJ( t.Text );
	HG_UNMARSHAL_GETOBJ( t.m_vecComponents );
	HG_UNMARSHAL_OBJECT_END;
}

}

}