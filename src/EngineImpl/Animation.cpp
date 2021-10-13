#include "Animation.h"
#include "GameObject2D.h"
#include "Transform.hpp"

using namespace __HGImpl::V1SDL;
using namespace HG::Math;


Animator2D::Animator2D( const char* strName ) : HGComponent(strName), m_vecFrames()
{
}

__HGImpl::V1SDL::Animator2D::Animator2D(const char* strName, const HG::Math::HGSize<un32>& tSingleFrameRect, const un32 countCol, const un32 countRow, const FrameIndex& IdleIndex, const float f32Interval, const bool isIdle )
	: HGComponent(strName), m_vecFrames(), m_IdleFrameIndex( IdleIndex ), m_f32Delta( 0.f ), f32Interval( f32Interval ), IsIdle( isIdle )
{
	auto size = static_cast<GameObject2D*>(m_pGameObject)->GetTextureSize();
	if ( countCol * tSingleFrameRect.W > size.W || countRow * tSingleFrameRect.H > size.H )
	{
		HG_LOG_FAILED( std::format("GameObject [{}] => animation component frame size is out of range", m_pGameObject->GetName() ).c_str() );
		return;
	}
	for (size_t i = 0; i < countRow; i++)
	{
		m_vecFrames.push_back( std::vector<Frame>() );
		for (size_t j = 0; j < countCol; j++)
		{
			Frame f;
			f.tPos.X = tSingleFrameRect.W * j;
			f.tPos.Y = tSingleFrameRect.H * i;
			f.tRect = tSingleFrameRect;
			m_vecFrames[i].push_back( f );
		}
	}
}

void __HGImpl::V1SDL::Animator2D::Play(const un32 f32DeltaTime)
{
	if ( IsIdle )
	{
		m_f32Delta = 0.f; 
		setFrame();
		return;
	}
	m_f32Delta += f32DeltaTime;
	if ( m_f32Delta >= f32Interval )
	{
		m_f32Delta = 0.f;
		++Col;
		setFrame();
	}
}


void __HGImpl::V1SDL::Animator2D::setFrame()
{
	auto tr = m_pGameObject->GetComponent<Transform>();
	if (IsIdle)
	{
		auto f = m_vecFrames[m_IdleFrameIndex.Row][m_IdleFrameIndex.Col];
		tr->tLocalPos = f.tPos;
		tr->tLocalRect = f.tRect;
	} else {
		Row = (Row > m_vecFrames.size()) ? 0 : Row;
		Col = (Col > m_vecFrames[Row].size()) ? 0 : Col;
		auto f = m_vecFrames[Row][Col];
		tr->tLocalPos = f.tPos;
		tr->tLocalRect = f.tRect;
	}
}

Animator2D::~Animator2D()
{
}