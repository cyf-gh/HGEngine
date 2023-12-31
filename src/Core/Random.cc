//
// Created by cyf-m on 2020/11/28.
//
// \ref https://github.com/cyf-gh/stLib/blob/alpha-develop/src/Random.cpp
//

#include "Random.h"

using namespace HG;
using namespace Random;

RandomWELL512::RandomWELL512() {
    SetSeed( 123 );
}

RandomWELL512::RandomWELL512( un32 seed ) {
    SetSeed( seed );
}

un32 RandomWELL512::GetRandSeed()  {
    return m_rdmSeed;
}

un32 RandomWELL512::GetRandUInt() {
    un32 a, b, c, d;
    a = m_state[ m_index ];
    c = m_state[ ( m_index + 13 ) & 15 ];
    b = a ^ c ^ ( a << 16 ) ^ ( c << 15 );
    c = m_state[ ( m_index+ 9 ) & 15 ];
    c ^= ( c >> 11 );
    a = m_state[ m_index ] = b ^ c;
    d = a ^ ( ( a << 5 ) & 0xDA442D24UL );
    m_index= ( m_index+ 15 ) & 15;
    a = m_state[ m_index ];
    m_state[ m_index ] = a ^ b ^ d ^ ( a << 2 ) ^ ( b << 18 ) ^ ( c << 28 );
    return m_state[ m_index ];
}

void RandomWELL512::SetSeed( un32 seed ) {
    const un32 mask = ~0u;

    m_rdmSeed = seed;
    m_index = 0;
    m_state[ 0 ] = seed & mask;
    for ( un32 i = 1; i < 16; ++i ) {
        m_state[i] = ( 1812433253UL * ( m_state[ i - 1 ] ^ ( m_state[ i - 1 ] >> 30 ) ) + i ) & mask;
    }
}
RandomXORSHIFT RandomXORSHIFT::Random = RandomXORSHIFT();

un32 RandomXORSHIFT::GetRandUInt() {
    static un32 X = 123456789;
    static un32 Y = 362436069;
    static un32 z = 521288629;
    un32 t;

    X ^= X << 16;
    X ^= X >> 5;
    X ^= X << 1;

    t = X;
    X = Y;
    Y = z;
    z = t ^ X ^ Y;

    return z;
}


un32 HG::Random::Random::GetRandUInt( un32 max ) {
    return GetRandUInt() % ( max + 1 );
}


f32 HG::Random::Random::GetRandFloat() {
    return GetRandUInt( 1000000 ) / 1000000.f;
}

f32 HG::Random::Random::GetRandFloat( f32 minBound, f32 maxBound ) {
    return GetRandFloat() * ( maxBound - minBound ) + minBound;
}
