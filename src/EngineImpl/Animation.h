#pragma once
#include <Math.hpp>
#include <vector>
#include "../Engine/HGComponent.h"

namespace HGEngine {
namespace V1SDL {

class Animator2D : public HG::HGComponent
{
public:
class Frame {
public:
	HG::Math::HGVec2<float> tPos;
	HG::Math::HGSize<un32> tRect;
};
class FrameIndex {
public:
	un32 Row;
	un32 Col;
};

enum HG_ANIMATOR_PLAYMODE : char {
	HG_ANIMATOR_PLAYMODE_LOOP,
	HG_ANIMATOR_PLAYMODE_REWIND,
	HG_ANIMATOR_PLAYMODES_LENGTH
};
protected:
	float m_f32Delta;
	bool isPosPlay;

public:
	std::vector<std::vector<Frame>> m_vecFrames;
	un32 m_unIdleFrameIndex;
	float f32Interval;
	bool IsIdle;
	un32 Row;
	un32 Col;
	bool CheckFramesValid();
	Animator2D( const char* strName );
	Animator2D();
	Animator2D( 
		const char* strName, 
		const HG::Math::HGSize<un32>&tSingleFrameRect, 
		const un32 countCol, 
		const un32 countRow, 
		const un32 idleIndex,
		const float f32Interval,
		const bool isIdle = true );
	void Play( const f32 f32DeltaTime, const char cMode = HG_ANIMATOR_PLAYMODE_REWIND);
	void ForceSetFrame();

	~Animator2D();
private:

};

}
}