//
// Created by cyf-m on 2020/11/28.
//

#ifndef HONEYGAME_HG_H
#define HONEYGAME_HG_H

namespace HG { 
    class HGEngine {
    public:
        /// \brief 游戏主线程，该线程对数据可读可写 <br>this is main thread of game; this is a Read-Write thread
        /// \note 
        /// * 与渲染线程不同步 <br> async with render thread 
        /// * OnFixedUpdate于此函数中处理
        virtual int Run() = 0;
    };
}

#define HGMain( ARGC, ARGV ) int main( ARGC, ARGV )


#endif //HONEYGAME_HG_H
