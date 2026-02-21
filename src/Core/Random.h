//
// Created by cyf-m on 2020/11/28.
//

#ifndef HONEYGAME_RANDOM_H
#define HONEYGAME_RANDOM_H

#include "Type.h"

namespace HG {
namespace Random {
class Random {
public:
	virtual un32 GetRandUInt() = 0;

	un32 GetRandUInt( un32 max );
	/// \brief rounded to 6 decimals
	f32	GetRandFloat();
	f32	GetRandFloat( f32 minBound, f32 maxBound );
};

class RandomXORSHIFT : public Random {
public:
	un32 GetRandUInt() override;

public:
	RandomXORSHIFT() = default;
	~RandomXORSHIFT() = default;
	static RandomXORSHIFT Random;
};

/// \brief Get random unsigned 32-bit integer
inline un32 RandomUInt()
{
    return HG::Random::RandomXORSHIFT::Random.GetRandUInt();
}

#define HG_RANDOM_UN32 RandomUInt()

#endif //HONEYGAME_RANDOM_H
