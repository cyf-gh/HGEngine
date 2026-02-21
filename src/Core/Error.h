/// error code
#pragma once

#include <exception>
#include <string>
#include <stdexcept>
#include "Type.h"

#ifdef _WIN32
#include <windows.h>
#define HG_ERR_SYS_ERROR()  GetLastError()
#else
#include <errno.h>
#define HG_ERR_SYS_ERROR()  errno
#endif

#define HG_RESULT_RES HGResult res;
#define HG_CHECK_ERR( RES_FUNC ) res = RES_FUNC; if ( res != HG_ERR_OK ) { return res; }
#define HG_CHECK_ERR_RET( RES_FUNC, RET ) res = RES_FUNC; if ( res != HG_ERR_OK ) { return RET; }
#define HG_THROW_IF_NULL( PTR, MSG ) if( PTR == nullptr ) { throw HG::Exception( MSG ); }


namespace HG {
/// \brief as SDL errors have been logged by SDL log functions
/// HG_ERR_XXX only do process that the behavior of function has done
/// successfully or not
/// \sa HG_CHECK_ERR
enum {
    HG_ERR_OK,
    HG_ERR_NULL_OUTPUT,
    HG_ERR_THREAD,
    HG_ERR_OUT_OF_MEMORY,
    HG_ERR_INVALID_PARAM,
    HG_ERR_NOT_FOUND,
    HG_ERR_ALREADY_EXISTS,
    HG_ERR_PARSE_FAILED,
    HG_ERR_UNKNOWN,
};

/// \brief HGEngine exception class
/// \details Provides detailed error information including error code and message
class Exception : public std::exception {
private:
    n32 m_ErrorCode;
    std::string m_Message;
    std::string m_What;

public:
    /// \brief Constructor with error code and message
    /// \param errorCode HG_ERR_XXX error code
    /// \param message Detailed error message
    Exception(n32 errorCode, const char* message)
        : m_ErrorCode(errorCode), m_Message(message) {
        m_What = std::to_string(errorCode) + ": " + message;
    }

    /// \brief Constructor with message only (defaults to HG_ERR_UNKNOWN)
    /// \param message Detailed error message
    Exception(const char* message)
        : m_ErrorCode(HG_ERR_UNKNOWN), m_Message(message) {
        m_What = std::to_string(HG_ERR_UNKNOWN) + ": " + message;
    }

    /// \brief Get error code
    /// \return HG_ERR_XXX code
    n32 GetErrorCode() const { return m_ErrorCode; }

    /// \brief Get error message
    /// \return Error message string
    const char* GetMessage() const { return m_Message.c_str(); }

    /// \brief Override what() for std::exception compatibility
    /// \return C-string with error information
    const char* what() const noexcept override { return m_What.c_str(); }

    /// \brief Destructor
    ~Exception() override = default;
};

/// \brief Result class for error handling
/// \details Provides a more convenient way to handle error codes with messages
class Result {
private:
    n32 m_Code;
    std::string m_Message;

public:
    /// \brief Default constructor (success)
    Result() : m_Code(HG_ERR_OK), m_Message("OK") {}

    /// \brief Constructor with error code
    /// \param code HG_ERR_XXX code
    Result(n32 code) : m_Code(code) {
        m_Message = GetErrorString(code);
    }

    /// \brief Constructor with error code and message
    /// \param code HG_ERR_XXX code
    /// \param message Detailed message
    Result(n32 code, const char* message) : m_Code(code), m_Message(message) {}

    /// \brief Check if result is success
    /// \return true if HG_ERR_OK
    bool IsOK() const { return m_Code == HG_ERR_OK; }

    /// \brief Get error code
    /// \return HG_ERR_XXX code
    n32 GetCode() const { return m_Code; }

    /// \brief Get error message
    /// \return Message string
    const std::string& GetMessage() const { return m_Message; }

    /// \brief Convert to bool (true if success)
    explicit operator bool() const { return IsOK(); }

    /// \brief Get error string from code
    /// \param code HG_ERR_XXX code
    /// \return Error string
    static const char* GetErrorString(n32 code) {
        switch (code) {
            case HG_ERR_OK: return "OK";
            case HG_ERR_NULL_OUTPUT: return "Null output pointer";
            case HG_ERR_THREAD: return "Thread error";
            case HG_ERR_OUT_OF_MEMORY: return "Out of memory";
            case HG_ERR_INVALID_PARAM: return "Invalid parameter";
            case HG_ERR_NOT_FOUND: return "Not found";
            case HG_ERR_ALREADY_EXISTS: return "Already exists";
            case HG_ERR_PARSE_FAILED: return "Parse failed";
            default: return "Unknown error";
        }
    }
};

/// \brief Macro to throw exception
/// \param CODE HG_ERR_XXX code
/// \param MSG Error message
#define HG_THROW( CODE, MSG ) throw HG::Exception( CODE, MSG )

/// \brief Macro to throw exception if condition is true
/// \param COND Condition to check
/// \param CODE HG_ERR_XXX code
/// \param MSG Error message
#define HG_THROW_IF( COND, CODE, MSG ) if( COND ) { throw HG::Exception( CODE, MSG ); }
}
