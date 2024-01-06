#include <string>
#include <EngineImpl\Log.hpp>
#include <engineImpl/EngineImpl.h>
#include <engineImpl/Scene.h>

using namespace std;
using namespace HGEngine::V1SDL;
using namespace HG;
//
//#pragma data_seg(".CRT$XIU")
//

#undef main


HGMain( int argc, char** argv ) {
	// scenes -> engine -> game objects
    //SDL_RendererInfo info;
    //SDL_GetRendererInfo(renderer, &info);
    //SDL_Log("Current SDL_Renderer: %s", info.name);
	auto* pScene = new Scene( "Default" );
	auto* tEngine = new EngineImpl( argc, argv );
	// tEngine->GetEditor()->Hide();
	HG_SCRIPT_BINDALL();

	tEngine->Render();
	HG_SAFE_DEL( pScene );
	HG_SAFE_DEL( tEngine );
	return 0;
}


void B2DExample() {
	b2Vec2 gravity( 0.0f, -10.0f );

	// Construct a world object, which will hold and simulate the rigid bodies.
	b2World world( gravity );

	// Define the ground body.
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set( 0.0f, -10.0f );

	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	b2Body* groundBody = world.CreateBody( &groundBodyDef );

	// Define the ground box shape.
	b2PolygonShape groundBox;

	// The extents are the half-widths of the box.
	groundBox.SetAsBox( 50.0f, 10.0f );

	// Add the ground fixture to the ground body.
	groundBody->CreateFixture( &groundBox, 0.0f );

	// Define the dynamic body. We set its position and call the body factory.
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set( 0.0f, 4.0f );
	b2Body* body = world.CreateBody( &bodyDef );

	// Define another box shape for our dynamic body.
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox( 1.0f, 1.0f );

	// Define the dynamic body fixture.
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;

	// Set the box density to be non-zero, so it will be dynamic.
	fixtureDef.density = 1.0f;

	// Override the default friction.
	fixtureDef.friction = 0.3f;

	// Add the shape to the body.
	body->CreateFixture( &fixtureDef );

	// Prepare for simulation. Typically we use a time step of 1/60 of a
	// second (60Hz) and 10 iterations. This provides a high quality simulation
	// in most game scenarios.
	f32 timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	// This is our little game loop.
	for( int32 i = 0; i < 60; ++i ) {
		// Instruct the world to perform a single step of simulation.
		// It is generally best to keep the time step and iterations fixed.
		world.Step( timeStep, velocityIterations, positionIterations );

		// Now print the position and angle of the body.
		b2Vec2 position = body->GetPosition();
		f32 angle = body->GetAngle();

		HG_LOG_INFO( std::format( "{} {} {}\n", position.x, position.y, angle ).c_str() );
	}
}
