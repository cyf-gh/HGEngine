#pragma once

#include "../Engine/HGEvent.hpp"
#include "../Engine/HGComponent.h"

namespace HGEngine {
namespace V1SDL {
/// \brief 计时器组件
class Timer : public HG::HGComponent {
public:
	bool Active;
	/// @brief unRepeatCount == unCurrentRepeatCount 时被调用
	HG::pEvent OnComplete;
	/// @brief 在首次执行OnInterval前被调用
	HG::pEvent OnStart;
	/// @brief 每隔 f32Interval 秒被调用
	HG::pEvent OnInterval;
	/// @brief 设置的Delay秒数
	f32 f32Delay;
	/// @brief 剩余的Delay秒数
	f32 f32DelayRest;
	/// @brief 自开始或重置时起经过的时间
	f32 f32Elapsed;
	/// @brief 自开始或重置时起经过的时间
	f32 f32ElpasedLoop;
	/// @brief 调用 OnInterval 的间隔
	f32 f32Interval;
	/// @brief 需要重复的次数 < 0 为永久
	un32 unRepeatCount;
	/// @brief 自开始或重置起重复过的次数
	un32 unCurrentRepeatCount;

	/// @brief 对 timer 进行时间流逝
	/// @param f32TimeDelta 
	/// @sa HGEngine::V1SDL::Scene::Update
	HG_INLINE void TikTok( const f32 f32TimeDelta ) {
		if( !Active ) {
			return;
		}
		f32DelayRest -= f32TimeDelta;
		if( f32DelayRest > 0 ) {
			return; // 未完成延迟
		}
		if( f32Elapsed == 0 && f32DelayRest <= 0 ) {
			HG_EVENT_CALLRAW_NO_DATA( OnStart, this );
		}
		f32Elapsed += f32TimeDelta;
		f32ElpasedLoop += f32TimeDelta;
		if( f32ElpasedLoop >= f32Interval ) {
			if( unRepeatCount > 0 ) {
				if( unCurrentRepeatCount == unRepeatCount ) {
					HG_EVENT_CALLRAW_NO_DATA( OnComplete, this );
					++unCurrentRepeatCount;
					return;
				} else if( unCurrentRepeatCount < unRepeatCount ) {
					++unCurrentRepeatCount; // 重复
				} else {
					return; // 重复次数已满，不再重复
				}
			}
			f32ElpasedLoop = 0.f;
			HG_EVENT_CALLRAW_NO_DATA( OnInterval, this );
			return;
		}
	}

	void Stop() { 
		Pause();
		Reset();
	}
	void Pause() {
		Active = false;
	}
	
	void Start( float f32Interval, un32 unRepeatCount, HG::pEvent OnInterval, HG::pEvent OnComplete = nullptr, HG::pEvent OnStart = nullptr ) {
		DelayStart( 0, f32Interval, unRepeatCount, OnInterval, OnComplete, OnStart );
	}
	void DelayStart( float f32Delay, float f32Interval, un32 unRepeatCount, HG::pEvent OnInterval, HG::pEvent OnComplete = nullptr, HG::pEvent OnStart = nullptr ) {
		this->f32Interval = f32Interval;

		this->f32Delay = f32Delay;
		f32DelayRest = f32Delay;

		this->OnComplete = OnComplete;
		this->OnInterval = OnInterval;

		this->unRepeatCount = unRepeatCount;
		this->unCurrentRepeatCount = 0;

		this->f32Elapsed = 0.f;
		this->Active = true;

		this->OnStart = OnStart;
	}

	void Reset( const f32 f32Elpased = 0.f, const f32 f32ElpasedLoop = 0.f, const un32 unRep = 0 ) {
		this->f32Elapsed = f32Elpased;
		this->f32ElpasedLoop = f32ElpasedLoop;
		this->unCurrentRepeatCount = unRep;
	}
	void Restart() { 
		DelayStart( f32Delay, f32Interval, unRepeatCount, OnInterval, OnComplete, OnStart );
	}

	Timer() : Active( false ) { }
};
}
}