/// \brief hg memory management
/// \file Memory.h
#pragma once

#include <vector>
#include <stack>
#include <mutex>

namespace HG {
namespace Memory {

/// \brief Non-copyable base class
class NonCopyable {
protected:
    NonCopyable() {}
    virtual ~NonCopyable() {}

private:
    NonCopyable(const NonCopyable&);
    NonCopyable& operator=(const NonCopyable&);
};

/// \brief Safe delete pointer
template<typename T>
inline void SafeDelete(T*& p)
{
    if (p != nullptr)
    {
        delete p;
        p = nullptr;
    }
}

/// \brief Safe delete array
template<typename T>
inline void SafeDeleteArray(T*& p)
{
    if (p != nullptr)
    {
        delete[] p;
        p = nullptr;
    }
}

/// \brief Zero memory
inline void ZeroMemory(void* p, int len)
{
    memset(p, 0, len);
}

} // namespace Memory
} // namespace HG

// C-style macros for compatibility
#define HG_SAFE_DEL(P) HG::Memory::SafeDelete(P)
#define HG_SAFE_DEL_ARR(P) HG::Memory::SafeDeleteArray(P)
#define HG_ZERO_MEMORY(p, len) memset(p, 0, len)
#define HG_EMPTY_STRING ""
#define HG_EMPTY_STRINGW _T("")

/// \brief Prevent binary copy (use with public:)
#define HG_MEM_NO_BINARY_COPY(CLASSNAME) \
    private: \
    CLASSNAME(const CLASSNAME&); \
    CLASSNAME& operator=(const CLASSNAME&);
