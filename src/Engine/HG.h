//
// Created by cyf-m on 2020/11/28.
//

#ifndef HONEYGAME_HG_H
#define HONEYGAME_HG_H

namespace HG { 
    class IEngine {
    public:
        /// \brief ��Ϸ���̣߳����̶߳����ݿɶ���д <br>this is main thread of game; this is a Read-Write thread
        /// \note 
        /// * ����Ⱦ�̲߳�ͬ�� <br> async with render thread 
        /// * OnFixedUpdate�ڴ˺����д���
        virtual int Render() = 0;
    };
}

#undef HGENGINE_GUI         // use imgui
#undef HGENGINE_PHYSICS     // use box2d

#define HGMain( ARGC, ARGV ) int main( ARGC, ARGV )

#define HG_ERASE_IN_VEC( pt, vec ) vec.erase( std::remove( vec.begin(), vec.end(), pt ), vec.end() );

#endif //HONEYGAME_HG_H
