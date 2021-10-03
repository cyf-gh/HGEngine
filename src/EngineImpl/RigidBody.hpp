/// https://segmentfault.com/a/1190000015325310
#pragma once

#include "../Engine/HGComponent.h"
#include "../Engine/HGEvent.h"

namespace __HGImpl {
namespace V1SDL {
class Timer : public HG::HGComponent {
public:
	HG::V1SDL::pEvent pOnComplete;
	HG::V1SDL::pEvent pOnInterval;
	double Delay;
	void Stop();
	void Start( float f32Interval, n32 unRepeatCount, HG::V1SDL::pEvent pOnInterval, HG::V1SDL::pEvent pOnComplete );
	void DelayStart( float f32Delay, HG::V1SDL::pEvent pOnComplete );
	void Reset();
	void Restart();
};
}
}