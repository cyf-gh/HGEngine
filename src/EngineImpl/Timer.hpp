#pragma once

#include "../Engine/HGEvent.hpp"
#include "../Engine/HGComponent.h"

namespace HGEngine {
namespace V1SDL {
/// \brief ��ʱ�����
class Timer : public HG::HGComponent {
public:
	bool Active;
	/// @brief unRepeatCount == unCurrentRepeatCount ʱ������
	HG::pEvent OnComplete;
	/// @brief ���״�ִ��OnIntervalǰ������
	HG::pEvent OnStart;
	/// @brief ÿ�� f32Interval �뱻����
	HG::pEvent OnInterval;
	/// @brief ���õ�Delay����
	f32 f32Delay;
	/// @brief ʣ���Delay����
	f32 f32DelayRest;
	/// @brief �Կ�ʼ������ʱ�𾭹���ʱ��
	f32 f32Elapsed;
	/// @brief �Կ�ʼ������ʱ�𾭹���ʱ��
	f32 f32ElpasedLoop;
	/// @brief ���� OnInterval �ļ��
	f32 f32Interval;
	/// @brief ��Ҫ�ظ��Ĵ��� < 0 Ϊ����
	un32 unRepeatCount;
	/// @brief �Կ�ʼ���������ظ����Ĵ���
	un32 unCurrentRepeatCount;

	/// @brief �� timer ����ʱ������
	/// @param f32TimeDelta 
	/// @sa HGEngine::V1SDL::Scene::Update
	HG_INLINE void TikTok( const f32 f32TimeDelta ) {
		if( !Active ) {
			return;
		}
		f32DelayRest -= f32TimeDelta;
		if( f32DelayRest > 0 ) {
			return; // δ����ӳ�
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
					++unCurrentRepeatCount; // �ظ�
				} else {
					return; // �ظ����������������ظ�
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