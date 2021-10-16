#pragma once

#include "../Engine/HGEvent.h"
#include "../Engine/HGComponent.h"

namespace HGEngine {
namespace V1SDL {
/// \brief 计时器组件
class Timer : public HG::HGComponent {
public:
	HG::pEvent pOnComplete;
	HG::pEvent pOnInterval;
	double Delay;
	void Stop() { }
	void Start( float f32Interval, n32 unRepeatCount, HG::pEvent pOnInterval, HG::pEvent pOnComplete ) { }
	void DelayStart( float f32Delay, HG::pEvent pOnComplete ) { }
	void Reset() { }
	void Restart() { }
};
}
}