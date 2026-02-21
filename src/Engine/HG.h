//
// Created by cyf-m on 2020/11/28.
//

#ifndef HONEYGAME_HG_H
#define HONEYGAME_HG_H

#include <algorithm>
#include <vector>

namespace HG { 
    class IEngine {
    public:
        virtual int Render() = 0;
    };
    /// \brief Erase element from vector
    template<typename T>
    static void EraseInVec(T* pt, std::vector<T>& vec)
    {
        vec.erase(std::remove(vec.begin(), vec.end(), pt), vec.end());
    }
    template<typename T>
    static void EraseInVec(T* pt, std::vector<T*>& vec)
    {
        vec.erase(std::remove(vec.begin(), vec.end(), pt), vec.end());
    }
}
#undef HGENGINE_GUI         // use imgui
#undef HGENGINE_PHYSICS     // use box2d

#define HGMain( ARGC, ARGV ) int main( ARGC, ARGV )

// Keep macro for compatibility
#define HG_ERASE_IN_VEC( pt, vec ) HG::EraseInVec(pt, vec)

#endif //HONEYGAME_HG_H
