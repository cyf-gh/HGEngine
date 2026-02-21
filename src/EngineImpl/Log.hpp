#pragma once

#include <string>
#include <fstream>
#include <ctime>
#include <iostream>
#include <sstream>
#include <Memory.h>
#include <Error.h>
#include <Directory.hpp>
#include <mutex>

namespace HG {

/// \brief Log level enum
enum class LogLevel {
    Debug = 0,
    Info = 1,
    Warning = 2,
    Error = 3,
    Fatal = 4,
    None = 5
};

/// \brief Get log level string
inline const char* LogLevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::Debug:   return "DEBUG";
        case LogLevel::Info:    return "INFO ";
        case LogLevel::Warning: return "WARN ";
        case LogLevel::Error:   return "ERROR";
        case LogLevel::Fatal:   return "FATAL";
        default:                return "UNKNOWN";
    }
}

/// \brief Logger class with level-based logging
class HGLog {
private:
    static HGLog* s_Instance;
    static std::mutex s_Mutex;

    LogLevel m_MinLevel;
    std::ofstream m_File;
    bool m_OutputToConsole;
    bool m_OutputToFile;
    std::string m_LogBuffer;

    char m_DateStr[20];
    char m_TimeStr[10];
    tm* m_pTm;

    void updateTime() {
        time_t now = time(nullptr);
        m_pTm = localtime(&now);
        sprintf_s(m_DateStr, sizeof(m_DateStr), "%04d-%02d-%02d", 
            1900 + m_pTm->tm_year, 1 + m_pTm->tm_mon, m_pTm->tm_mday);
        sprintf_s(m_TimeStr, sizeof(m_TimeStr), "%02d:%02d:%02d", 
            m_pTm->tm_hour, m_pTm->tm_min, m_pTm->tm_sec);
    }

    void logMessage(LogLevel level, const char* message, const char* category = nullptr) {
        if (level < m_MinLevel) return;

        std::lock_guard<std::mutex> lock(s_Mutex);
        updateTime();

        std::ostringstream oss;
        if (category) {
            oss << category << "\t";
        }
        oss << "[" << m_TimeStr << "] " 
            << LogLevelToString(level) << ": " 
            << message;

        std::string logStr = oss.str();

        if (m_OutputToConsole) {
            std::cout << logStr << std::endl;
        }

        if (m_OutputToFile && m_File.is_open()) {
            m_File << m_DateStr << "\t" << logStr << std::endl;
            m_File.flush();
        }

        m_LogBuffer += logStr;
        m_LogBuffer += "\n";
    }

public:
    /// \brief Get singleton instance
    static HGLog* GetInstance() {
        if (s_Instance == nullptr) {
            s_Instance = new HGLog();
        }
        return s_Instance;
    }

    /// \brief Initialize logger
    /// \param logPath Path to log directory (e.g., "./Log/")
    /// \param minLevel Minimum log level to output
    /// \param outputToConsole Enable console output
    /// \param outputToFile Enable file output
    static void Initialize(const char* logPath = "./Log/", 
                          LogLevel minLevel = LogLevel::Debug,
                          bool outputToConsole = true,
                          bool outputToFile = true) {
        std::lock_guard<std::mutex> lock(s_Mutex);
        if (s_Instance != nullptr) {
            delete s_Instance;
        }
        s_Instance = new HGLog(logPath, minLevel, outputToConsole, outputToFile);
    }

    /// \brief Shutdown logger
    static void Shutdown() {
        std::lock_guard<std::mutex> lock(s_Mutex);
        if (s_Instance != nullptr) {
            delete s_Instance;
            s_Instance = nullptr;
        }
    }

    /// \brief Constructor
    explicit HGLog(const char* logPath = "./Log/",
                  LogLevel minLevel = LogLevel::Debug,
                  bool outputToConsole = true,
                  bool outputToFile = true)
        : m_MinLevel(minLevel), m_OutputToConsole(outputToConsole), 
          m_OutputToFile(outputToFile), m_pTm(nullptr) {
        if (m_OutputToFile) {
            HGDirectory::CreateDirectoryIfDoesNotExsit(logPath);
            std::string logFile(logPath);
            updateTime();
            logFile += m_DateStr;
            logFile += ".log";
            m_File.open(logFile.c_str(), std::ofstream::out | std::ofstream::app);
            if (!m_File.is_open()) {
                std::cerr << "Failed to open log file: " << logFile << std::endl;
                m_OutputToFile = false;
            }
        }
    }

    /// \brief Destructor
    ~HGLog() {
        if (m_File.is_open()) {
            m_File.close();
        }
    }

    /// \brief Set minimum log level
    void SetMinLevel(LogLevel level) { m_MinLevel = level; }

    /// \brief Get minimum log level
    LogLevel GetMinLevel() const { return m_MinLevel; }

    /// \brief Enable/disable console output
    void SetConsoleOutput(bool enable) { m_OutputToConsole = enable; }

    /// \brief Enable/disable file output
    void SetFileOutput(bool enable) { m_OutputToFile = enable; }

    /// \brief Log debug message
    void Debug(const char* message, const char* category = nullptr) {
        logMessage(LogLevel::Debug, message, category);
    }

    /// \brief Log info message
    void Info(const char* message, const char* category = nullptr) {
        logMessage(LogLevel::Info, message, category);
    }

    /// \brief Log warning message
    void Warning(const char* message, const char* category = nullptr) {
        logMessage(LogLevel::Warning, message, category);
    }

    /// \brief Log error message
    void Error(const char* message, const char* category = nullptr) {
        logMessage(LogLevel::Error, message, category);
    }

    /// \brief Log fatal message
    void Fatal(const char* message, const char* category = nullptr) {
        logMessage(LogLevel::Fatal, message, category);
    }

    /// \brief Get log buffer content
    std::string GetBuffer() const { return m_LogBuffer; }

    /// \brief Clear log buffer
    void ClearBuffer() { m_LogBuffer.clear(); }

    /// \brief Flush log file
    void Flush() {
        if (m_File.is_open()) {
            m_File.flush();
        }
    }

    // Legacy compatibility methods
    void Log2File(int priority, const char* message, const char* category) {
        LogLevel level = static_cast<LogLevel>(priority);
        logMessage(level, message, category);
    }

    void Log2Console(int priority, const char* message, const char* category) {
        LogLevel level = static_cast<LogLevel>(priority);
        logMessage(level, message, category);
    }

    void FailedSDL(int category, const char* strFuncName) {
        Error(strFuncName, "SDL_ERROR");
    }

    void Failed(int category, const char* message) {
        Error(message);
    }

    void AssertFailed(int category, const char* message) {
        Error(message, "ASSERT_FAILED");
    }

    void AssertSuccess(int category, const char* message) {
        Debug(message, "ASSERT_SUCCESS");
    }

    void Fault(int category, const char* message) {
        Fatal(message);
    }

    void Success(int category, const char* message) {
        Info(message);
    }

    void Info(int category, const char* message) {
        Info(message);
    }

    void Warning(int category, const char* message) {
        Warning(message);
    }

    void Debug(int category, const char* message) {
        Debug(message);
    }
};

/// \brief Global log instance (legacy compatibility)
#define HG_LOG HG::HGLog::GetInstance()

// Initialization macros
#define HG_LOG_INIT(PATH, LEVEL) HG::HGLog::Initialize(PATH, LEVEL)
#define HG_LOG_SHUTDOWN() HG::HGLog::Shutdown()

}

/// \brief check if SDL object return with a non-nullptr value and log verbose
/// \param P SDL handle pointer
/// \param SDL_LOG_CATEGORY SDL_LOG_CATEGORY_XXX
/// \param FUNC_NAME e.g. "foo::foo"
#define HG_LOG_CHECK_SDL_HANDLE_IS_NULL( P, SDL_LOG_CATEGORY, FUNC_NAME ) \
if ( P == nullptr){ \
    HG_LOG->FailedSDL( SDL_LOG_CATEGORY, FUNC_NAME ); \
    return; \
} else { \
    HG_LOG->Success( SDL_LOG_CATEGORY, FUNC_NAME ); \
}

/// \brief log the SDL error directly
/// \sa HG_LOG_CHECK_SDL_HANDLE_IS_NULL
#define HG_LOG_SDL_ERROR( SDL_LOG_CATEGORY, FUNC_NAME ) \
HG::HGLog::GetInstance()->FailedSDL( SDL_LOG_CATEGORY, FUNC_NAME );

/// \brief log info
#define HG_LOG_INFO( info ) \
HG::HGLog::GetInstance()->Info( info )

#define HG_LOG_INFOF( FMT, ARGS ) \
HG::HGLog::GetInstance()->Info( std::format( FMT, ARGS ).c_str() )

/// \brief log failed info
#define HG_LOG_FAILED( info ) \
HG::HGLog::GetInstance()->Failed( 3, info )

/// \brief log failed info
#define HG_LOG_FAILEDF( FMT, ARGS ) \
HG::HGLog::GetInstance()->Failed( 3, std::format( FMT, ARGS ).c_str() )

/// \brief log failed info
#define HG_LOG_WARNNING( info ) \
HG::HGLog::GetInstance()->Warning( 100, info )

/// \brief log failed info
#define HG_LOG_WARNNINGF( FMT, ARGS ) \
HG::HGLog::GetInstance()->Warning( std::format( FMT, ARGS ).c_str() )

#define HG_LOG_SUCCESS( info ) \
HG::HGLog::GetInstance()->Success( 3, info )

#ifdef HG_RELEASE
#   define HG_LOG_DEBUGF( FMT, ARGS )
#else
#   define HG_LOG_DEBUGF( FMT, ARGS ) \
HG::HGLog::GetInstance()->Debug( std::format( FMT, ARGS ).c_str() )
#endif

/// \brief log failed info
#define HG_LOG_TEST_ASSERT_SUCCESS( info ) \
HG::HGLog::GetInstance()->AssertSuccess( 3, info )

/// \brief log failed info
#define HG_LOG_TEST_ASSERT_FAILED( info ) \
HG::HGLog::GetInstance()->AssertFailed( 3, info )
