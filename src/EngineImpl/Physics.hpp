#pragma once

#include <vector>
#include <box2d\box2d.h>
#include "RigidBody.h"

namespace HGEngine {
namespace V1SDL {
class HGWorld {
public:
	bool Enable;
	b2World Handle;
	std::vector<RigidBodyB2*> vecRbs;
	f32 TimeStep;
	n32 VelocityIterations;
	n32 PositionIterations;
	HGWorld( b2Vec2 g, f32 ts, n32 vi, n32 pi ) 
		: Enable( true ), Handle( g ), TimeStep( ts ), VelocityIterations( vi ), PositionIterations( pi ) {
		Handle.SetAllowSleeping( false );
		vecRbs = std::vector<RigidBodyB2*>();
	}
	~HGWorld() {
		for( RigidBodyB2* rb : vecRbs ) {
			HG_SAFE_DEL( rb );
		}
	}
	void Step() {
		if( !Enable ) {
			return;
		}
		Handle.Step( TimeStep, VelocityIterations, PositionIterations );
		for( RigidBodyB2* pRb : vecRbs ) {
			if( pRb->IsSyncTransform ) { pRb->Sync2Transform(); }
		}
	}
};
class Physics {
public:
	std::vector<HGWorld*> Worlds;
	explicit Physics() {
		Worlds = std::vector<HGWorld*>();
	}
	~Physics() {
		for( HGWorld* w : Worlds ) {
			HG_SAFE_DEL( w );
		}
	}
	void AddWorld( b2Vec2 g, f32 timeStep, n32 velocityIterations, n32 positionIterations ) {
		Worlds.push_back( new HGWorld( g, timeStep, velocityIterations, positionIterations ) );
	}
	HGWorld* FirstWorld() {
		return Worlds[0];
	}
};
}
}