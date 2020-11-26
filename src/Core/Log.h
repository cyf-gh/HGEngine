#pragma once

#include <SDL.h>
#include <string>
#include <fstream>
#include <ctime>
#include <iostream>
#include "Memory.h"
#include "Error.h"

namespace HG {
    class HGLog {
    private:
        const char *getErrorDesc( const char * strFuncName ) {
            std::string desc( strFuncName );
            desc += " failed: %s";
            desc += SDL_GetError();
            return desc.c_str();
        }
        inline const char *getDateStr() {
            time_t now = time(nullptr );

            ptm = localtime(&now);
            sprintf_s( strDate, 11, "%d-%d-%d", 1900 + ptm->tm_year, 1 + ptm->tm_mon, ptm->tm_mday );
            return strDate;
        }
        inline const char *getTimeStr() {
            time_t now = time(nullptr );

            ptm = localtime(&now);
            sprintf_s( strTime, 9, "%d:%d:%d", ptm->tm_hour, ptm->tm_min, ptm->tm_sec );
            return strTime;
        }
    public:
        /// \param strLogFilePath log file path, like "./Log/"
        /// \note rather ./Log/ than ./Log
        explicit HGLog( const char* strLogFilePath = "./Log/")
            :ptm( nullptr ), strTime( new char[9] ), strDate( new char[11] ) {
            std::string strLogFile( strLogFilePath );
            strLogFile += getDateStr();
            strLogFile += ".txt";
            tOfs.open( strLogFile.c_str(), std::ofstream::out | std::ofstream::app );
            if ( !tOfs.is_open() ) {
                std::cerr<<"failed to open file: "<< SYS_ERROR() <<std::endl;
                std::cerr<<"log component may do not work";
            }
        };
        ~HGLog() {
            tOfs.close();
            delete ptm;
            delete[] strTime;
            delete[] strDate;
        }
        void Log2File( const int priority, const char* message, const char* category ) {
            tOfs << category << "[" << getTimeStr() << "]" << "("<< priority << ")\t" << message << std::endl;
            tOfs.flush();
        }
        void LogEnter2File() {
            tOfs << std::endl;
            tOfs.flush();
        }
        void Log2Console(  const int priority, const char* message, const char* category ) {
            std::cout << category  << "[" << getTimeStr() << "]" << "("<< priority << ")\t" << message << std::endl;
        }
        void Failed( const int priority, const char* strFuncName ) {
            Log2File( priority, getErrorDesc( strFuncName ), "FAILED" );
            Log2Console( priority, getErrorDesc( strFuncName ), "FAILED" );
        }
        void Success( const int priority, const char* message ) {
            Log2File( priority, message, "SUCCESS" );
            Log2Console( priority, message, "SUCCESS" );
        }
        void Info( const int priority, const char* message ) {
            Log2File( priority, message, "INFO" );
            Log2Console( priority, message, "INFO" );
        }
        void Warning( const int priority, const char* message ) {
            Log2File( priority, message, "WARNING" );
            Log2Console( priority, message, "WARNING" );
        }
    private:
        std::ofstream tOfs;
        char *strDate;
        char *strTime;
        tm *ptm;
    };
    static auto* Log = new HGLog();
}