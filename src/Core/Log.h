#pragma once

#include <SDL.h>
#include <string>
#include <fstream>
#include <ctime>
#include <iostream>
#include "Memory.h"
#include "Error.h"

namespace HGCore {
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
            strLogFile.append( getDateStr() ).append(".txt" );
            tOfs.open( strLogFile.c_str(), std::ofstream::out | std::ofstream::app );
            if ( !tOfs.is_open() ) {
                std::cerr << "failed to open file: " << HG_ERR_SYS_ERROR() << std::endl;
                std::cerr<<"log component may do not work";
            }
        };
        ~HGLog() {
            tOfs.close();
            HG_SAFE_DEL( ptm );
            HG_SAFE_DEL_ARR( strTime );
            HG_SAFE_DEL_ARR( strDate );
        }
        void Log2File( const int priority, const char* message, const char* category ) {
            tOfs << category << "\t[" << getTimeStr() << "]" << "("<< priority << ")\t" << message << std::endl;
            tOfs.flush();
        }
        void LogEnter2File() {
            tOfs << std::endl;
            tOfs.flush();
        }
        void Log2Console(  const int sdlCategory, const char* message, const char* category ) {
            std::cout << category  << "\t[" << getTimeStr() << "]" << "("<< sdlCategory << ")\t" << message << std::endl;
        }
        void FailedSDL(const int sdlCategory, const char* strFuncName ) {
            Log2File( sdlCategory, getErrorDesc( strFuncName ), "FAILED SDL" );
            Log2Console( sdlCategory, getErrorDesc( strFuncName ), "FAILED SDL" );
        }
        void Failed( const int sdlCategory, const char* message ) {
            Log2File( sdlCategory, message, "FAILED" );
            Log2Console( sdlCategory, message, "FAILED" );
        }
        void Fault( const int sdlCategory, const char* message ) {
            Log2File( sdlCategory, message, "FAULT" );
            Log2Console( sdlCategory, message, "FAULT" );
        }
        void Success( const int sdlCategory, const char* message ) {
            Log2File( sdlCategory, message, "SUCCESS" );
            Log2Console( sdlCategory, message, "SUCCESS" );
        }
        void Info( const int sdlCategory, const char* message ) {
            Log2File( sdlCategory, message, "INFO" );
            Log2Console( sdlCategory, message, "INFO" );
        }
        void Warning( const int sdlCategory, const char* message ) {
            Log2File( sdlCategory, message, "WARNING" );
            Log2Console( sdlCategory, message, "WARNING" );
        }
        void FlushLogFile() { tOfs.flush(); }
    private:
        std::ofstream tOfs;
        char *strDate;
        char *strTime;
        tm *ptm;
    };
    static auto* Log = new HGLog();
}

/// \brief check if SDL object return with a non-nullptr value and log verbose
/// \param P SDL handle pointer
/// \param SDL_LOG_CATEGORY SDL_LOG_CATEGORY_XXX
/// \param FUNC_NAME e.g. "foo::foo"
#define HG_LOG_CHECK_SDL_HANDLE_IS_NULL( P, SDL_LOG_CATEGORY, FUNC_NAME ) \
if ( P == nullptr){ \
    HG_LOG_SDL_ERROR( SDL_LOG_CATEGORY, FUNC_NAME ); \
    return; \
} else { \
    HGCore::Log->Success( SDL_LOG_CATEGORY, FUNC_NAME ); \
}

/// \brief log the SDL error directly
/// \sa HG_LOG_CHECK_SDL_HANDLE_IS_NULL
#define HG_LOG_SDL_ERROR( SDL_LOG_CATEGORY, FUNC_NAME ) \
HGCore::Log->FailedSDL( SDL_LOG_CATEGORY, FUNC_NAME );
