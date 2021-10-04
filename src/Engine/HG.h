//
// Created by cyf-m on 2020/11/28.
//

#ifndef HONEYGAME_HG_H
#define HONEYGAME_HG_H

namespace HG { 
    class HGEngine {
    public:
        /// \brief this is main thread of game; this is a Read-Write thread
        /// \note process for user input.
        virtual int Run() = 0;
    };
}

#define HGMain( ARGC, ARGV ) int main( ARGC, ARGV )


#endif //HONEYGAME_HG_H
