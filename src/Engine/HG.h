//
// Created by cyf-m on 2020/11/28.
//

#ifndef HONEYGAME_HG_H
#define HONEYGAME_HG_H

namespace HG { 
    class HGEngine {
    public:
        /// \brief ��Ϸ���̣߳����̶߳����ݿɶ���д <br>this is main thread of game; this is a Read-Write thread
        /// \note 
        /// * ����Ⱦ�̲߳�ͬ�� <br> async with render thread 
        /// * OnFixedUpdate�ڴ˺����д���
        virtual int Run() = 0;
    };
}

#define HGMain( ARGC, ARGV ) int main( ARGC, ARGV )


#endif //HONEYGAME_HG_H
