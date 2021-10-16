#include "Animation.h"
#include "GameObject2D.h"
#include "Transform.hpp"

using namespace HGEngine::V1SDL;
using namespace HG::Math;


bool HGEngine::V1SDL::Animator2D::CheckFramesValid() {
	auto size = static_cast< GameObject2D* >( m_pGameObject )->GetTextureSize();
	// if ( Col * tSingleFrameRect.W > size.W || countRow * tSingleFrameRect.H > size.H)
	//{
	//	HG_LOG_FAILED(std::format("GameObject [{}] => animation component frame size is out of range", m_pGameObject->GetName()).c_str());
	//	return false;
	//}
	return true;
}

Animator2D::Animator2D( const char* strName ) : HGComponent( strName ), m_vecFrames() { }

HGEngine::V1SDL::Animator2D::Animator2D( const char* strName, const HG::Math::HGSize<un32>& tSingleFrameRect, const un32 countCol, const un32 countRow, const un32 idleIndex, const float f32Interval, const bool isIdle )
	: HGComponent( strName ), m_vecFrames(), m_unIdleFrameIndex( idleIndex ), m_f32Delta( 0.f ), f32Interval( f32Interval ), IsIdle( isIdle ), isPosPlay( true ) {
	for( size_t i = 0; i < countRow; i++ ) {
		m_vecFrames.push_back( std::vector<Frame>() );
		for( size_t j = 0; j < countCol; j++ ) {
			Frame f;
			f.tPos.X = tSingleFrameRect.W * j;
			f.tPos.Y = tSingleFrameRect.H * i;
			f.tRect = tSingleFrameRect;
			m_vecFrames[i].push_back( f );
		}
	}
}

void HGEngine::V1SDL::Animator2D::Play( const f32 f32DeltaTime, const char cMode ) {
	if( IsIdle ) {
		m_f32Delta = 0.f;
		ForceSetFrame();
		return;
	}
	m_f32Delta += f32DeltaTime;
	if( m_f32Delta >= f32Interval ) {
		auto tf = m_vecFrames[Row].size();
		m_f32Delta = 0.f;
		switch( cMode ) {
		case HG_ANIMATOR_PLAYMODE_REWIND:
		Col = isPosPlay ? Col + 1 : Col - 1;
		if( Col >= tf - 1 ) {
			Col = tf - 1;
			isPosPlay = false;
		}
		if( Col == 0 ) {
			isPosPlay = true;
		}
		break;
		case HG_ANIMATOR_PLAYMODE_LOOP:
		++Col;
		Row = ( Row >= m_vecFrames.size() ) ? 0 : Row;
		Col = ( Col >= m_vecFrames[Row].size() ) ? 0 : Col;
		break;
		}
		ForceSetFrame();
	}
}


void HGEngine::V1SDL::Animator2D::ForceSetFrame() {
	auto tr = m_pGameObject->GetComponent<Transform>();
	if( IsIdle ) {
		auto f = m_vecFrames[Row][m_unIdleFrameIndex];
		tr->tLocalPos = f.tPos;
		tr->tLocalRect = f.tRect;
	} else {
		auto f = m_vecFrames[Row][Col];
		tr->tLocalPos = f.tPos;
		tr->tLocalRect = f.tRect;
	}
}

Animator2D::~Animator2D() { }