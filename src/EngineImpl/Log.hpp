#pragma once

#include <string>
#include <fstream>
#include <ctime>
#include <iostream>
#include <Memory.h>
#include <Error.h>
#include <Directory.hpp>

namespace HG {
    class HGLog {
    private:
        HG_INLINE const char* timeSprintf( const int t ) {
            return t < 10 ? "0%d" : "%d";
        }
        const std::string getErrorDesc( const char * strFuncName, const char* strMsg = "" ) {
            std::string desc( strFuncName );
            desc += " failed: %s";
            desc += strMsg;
            return desc;
        }
        inline const char *getDateStr() {
            time_t now = time(nullptr );

            ptm = localtime(&now);
            std::string t = "%d-";
            t += timeSprintf( ptm->tm_mon );
            t += "-";
            t += timeSprintf( ptm->tm_mday );
            sprintf_s( strDate, 20, t.c_str(), 1900 + ptm->tm_year, 1 + ptm->tm_mon, ptm->tm_mday );
            return strDate;
        }
        inline const char *getTimeStr() {
            time_t now = time( nullptr );

            ptm = localtime(&now);
            std::string t;
            t += timeSprintf( ptm->tm_hour );
            t += ":";
            t += timeSprintf( ptm->tm_min );
            t += ":";
            t += timeSprintf( ptm->tm_sec );
            sprintf_s( strTime, 10, t.c_str(), ptm->tm_hour, ptm->tm_min, ptm->tm_sec );
            return strTime;
        }

    public:
        /// \param strLogFilePath log file path, like "./Log/"
        /// \note rather ./Log/ than ./Log
        explicit HGLog( const char* strLogFilePath = "./Log/")
            :ptm( nullptr ), strTime( new char[10] ), strDate( new char[20] ) {
            HGDirectory::CreateDirectoryIfDoesNotExsit( strLogFilePath );
            
            std::string strLogFile( strLogFilePath );
            strLogFile.append( getDateStr() ).append(".txt" );
            tOfs.open( strLogFile.c_str(), std::ofstream::out | std::ofstream::app );
            if ( !tOfs.is_open() ) {
                std::cerr << "failed to open file: " << HG_ERR_SYS_ERROR() << std::endl;
                std::cerr<<" log component may do not work";
                assert( false );
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
            HGLogStr += ( std::string( category )+ "\t" + '[' + std::string( getTimeStr() ) + "]" + message + "\n" );
        }
        void FailedSDL(const int sdlCategory, const char* strFuncName ) {
            Log2File( sdlCategory, getErrorDesc( strFuncName ).c_str(), "FAILED SDL" );
            Log2Console( sdlCategory, getErrorDesc( strFuncName ).c_str(), "FAILED SDL" );
        }
        void Failed( const int sdlCategory, const char* message ) {
            Log2File( sdlCategory, message, "FAILED" );
            Log2Console( sdlCategory, message, "FAILED" );
        }
        void AssertFailed( const int sdlCategory, const char* message ) {
            Log2File( sdlCategory, message, "ASSERT FAILED" );
            Log2Console( sdlCategory, message, "ASSERT FAILED" );
        }
        void AssertSuccess( const int sdlCategory, const char* message ) {
            Log2File( sdlCategory, message, "ASSERT SUCCESS" );
            Log2Console( sdlCategory, message, "ASSERT SUCCESS" );
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
        void Debug( const int sdlCategory, const char* message ) {
            Log2Console( sdlCategory, message, "DEBUG" );
        }
        void FlushLogFile() { tOfs.flush(); }

        std::string GetString() { std::stringstream ss; ss << std::cout.rdbuf(); return ss.str(); }
        static HGLog *Log;
        static std::string HGLogStr;
    private:
        std::ofstream tOfs;
        char *strDate;
        char *strTime;
        tm *ptm;
    };
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
    HG::HGLog::Log->Success( SDL_LOG_CATEGORY, FUNC_NAME ); \
}

/// \brief log the SDL error directly
/// \sa HG_LOG_CHECK_SDL_HANDLE_IS_NULL
#define HG_LOG_SDL_ERROR( SDL_LOG_CATEGORY, FUNC_NAME ) \
HG::HGLog::Log->FailedSDL( SDL_LOG_CATEGORY, FUNC_NAME );

/// \brief log info
#define HG_LOG_INFO( info ) \
HG::HGLog::Log->Info( 3, info )

#define HG_LOG_INFOF( FMT, ARGS ) \
HG::HGLog::Log->Info( 3, std::format( FMT, ARGS ).c_str() )

/// \brief log failed info
#define HG_LOG_FAILED( info ) \
HG::HGLog::Log->Failed( 3, info )

/// \brief log failed info
#define HG_LOG_FAILEDF( FMT, ARGS ) \
HG::HGLog::Log->Failed( 3, std::format( FMT, ARGS ).c_str() )

/// \brief log failed info
#define HG_LOG_WARNNING( info ) \
HG::HGLog::Log->Warning( 100, info )

/// \brief log failed info
#define HG_LOG_WARNNINGF( FMT, ARGS ) \
HG::HGLog::Log->Warning( 3, std::format( FMT, ARGS ).c_str() )

#define HG_LOG_SUCCESS( info ) \
HG::HGLog::Log->Success( 3, info )

#ifdef HG_RELEASE
#   define HG_LOG_DEBUGF( FMT, ARGS )
#else
#   define HG_LOG_DEBUGF( FMT, ARGS ) \
HG::HGLog::Log->Debug( 3, std::format( FMT, ARGS ).c_str() )
#endif

/// \brief log failed info
#define HG_LOG_TEST_ASSERT_SUCCESS( info ) \
HG::HGLog::Log->AssertSuccess( 3, info )

/// \brief log failed info
#define HG_LOG_TEST_ASSERT_FAILED( info ) \
HG::HGLog::Log->AssertFailed( 3, info )
