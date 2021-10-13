#pragma once
#include <vector>
#include "../Engine/HGComponent.h"
#include "../Core/Math.h"

namespace __HGImpl {
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
protected:
	float m_f32Delta;
	void setFrame();

public:
	std::vector<std::vector<Frame>> m_vecFrames;
	FrameIndex m_IdleFrameIndex;
	float f32Interval;
	bool IsIdle;
	un32 Row;
	un32 Col;
	Animator2D( const char* strName );
	Animator2D( 
		const char* strName, 
		const HG::Math::HGSize<un32>&tSingleFrameRect, 
		const un32 countCol, 
		const un32 countRow, 
		const FrameIndex &IdleIndex,
		const float f32Interval,
		const bool isIdle = true );
	void Play( const un32 f32DeltaTime );

	~Animator2D();
private:

};

}
}