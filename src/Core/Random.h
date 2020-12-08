//
// Created by cyf-m on 2020/11/28.
//

#ifndef HONEYGAME_RANDOM_H
#define HONEYGAME_RANDOM_H

#include "Type.h"

namespace HGCore {
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

    class RandomWELL512 : public Random {
    private:
        n32				m_state[16];
        n32				m_index;
        n32				m_rdmSeed;

    public:
        un32			GetRandUInt() override;

        un32			GetRandSeed();
        void			SetSeed( un32 seed );

        RandomWELL512();
        explicit RandomWELL512( un32 seed );
        ~RandomWELL512() = default;
    };
}


#endif //HONEYGAME_RANDOM_H
