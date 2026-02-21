#pragma once

#include <string>
#include <functional>
#include <vector>
#include <iostream>
#include <cassert>

namespace HG {
namespace Test {

/// \brief Test result enum
enum class TestResult {
    Passed,
    Failed,
    Skipped
};

/// \brief Test case information
struct TestCase {
    std::string Name;
    std::function<void()> Func;
    TestResult Result;
    std::string Message;
};

/// \brief Test suite collection
class TestSuite {
private:
    static TestSuite* s_Instance;
    std::vector<TestCase> m_Tests;
    int m_Passed;
    int m_Failed;
    int m_Skipped;

public:
    static TestSuite& GetInstance() {
        if (s_Instance == nullptr) {
            s_Instance = new TestSuite();
        }
        return *s_Instance;
    }

    TestSuite() : m_Passed(0), m_Failed(0), m_Skipped(0) {}

    void RegisterTest(const char* name, std::function<void()> func) {
        TestCase tc;
        tc.Name = name;
        tc.Func = func;
        tc.Result = TestResult::Skipped;
        m_Tests.push_back(tc);
    }

    void RunAll() {
        m_Passed = 0;
        m_Failed = 0;
        m_Skipped = 0;

        std::cout << "========================================" << std::endl;
        std::cout << "         Running HGEngine Tests        " << std::endl;
        std::cout << "========================================" << std::endl;

        for (auto& test : m_Tests) {
            std::cout << "Running: " << test.Name << " ... ";
            try {
                test.Func();
                test.Result = TestResult::Passed;
                m_Passed++;
                std::cout << "PASSED" << std::endl;
            } catch (const std::exception& e) {
                test.Result = TestResult::Failed;
                test.Message = e.what();
                m_Failed++;
                std::cout << "FAILED (" << e.what() << ")" << std::endl;
            } catch (...) {
                test.Result = TestResult::Failed;
                m_Failed++;
                std::cout << "FAILED (unknown exception)" << std::endl;
            }
        }

        std::cout << "========================================" << std::endl;
        std::cout << "Results: " << m_Passed << " passed, " 
                  << m_Failed << " failed, " 
                  << m_Skipped << " skipped" << std::endl;
        std::cout << "========================================" << std::endl;
    }

    int GetPassedCount() const { return m_Passed; }
    int GetFailedCount() const { return m_Failed; }
    int GetSkippedCount() const { return m_Skipped; }
    int GetTotalCount() const { return static_cast<int>(m_Tests.size()); }

    bool IsAllPassed() const { return m_Failed == 0; }
};

/// \brief Register a test case
#define HG_TEST_REGISTER(NAME, FUNC) \
    HG::Test::TestSuite::GetInstance().RegisterTest(NAME, FUNC)

/// \brief Run all registered tests
#define HG_TEST_RUN_ALL() \
    HG::Test::TestSuite::GetInstance().RunAll()

/// \brief Get test results
#define HG_TEST_GET_RESULTS() \
    HG::Test::TestSuite::GetInstance()

/// \brief Simple assertion macro
#define HG_TEST_ASSERT(EXPRESSION, MSG) \
    if (!(EXPRESSION)) { \
        throw std::runtime_error(std::string(MSG) + ": assertion failed: " + #EXPRESSION); \
    }

/// \brief Assert equals
#define HG_TEST_ASSERT_EQ(EXPECTED, ACTUAL, MSG) \
    if ((EXPECTED) != (ACTUAL)) { \
        throw std::runtime_error(std::string(MSG) + ": expected " + std::to_string(EXPECTED) + " but got " + std::to_string(ACTUAL)); \
    }

/// \brief Assert not equals
#define HG_TEST_ASSERT_NE(NOT_EXPECTED, ACTUAL, MSG) \
    if ((NOT_EXPECTED) == (ACTUAL)) { \
        throw std::runtime_error(std::string(MSG) + ": expected not " + std::to_string(NOT_EXPECTED) + " but got " + std::to_string(ACTUAL)); \
    }

/// \brief Assert true
#define HG_TEST_ASSERT_TRUE(EXPR, MSG) \
    HG_TEST_ASSERT(EXPR, MSG)

/// \brief Assert false
#define HG_TEST_ASSERT_FALSE(EXPR, MSG) \
    HG_TEST_ASSERT(!(EXPR), MSG)

/// \brief Assert null
#define HG_TEST_ASSERT_NULL(PTR, MSG) \
    if ((PTR) != nullptr) { \
        throw std::runtime_error(std::string(MSG) + ": expected null but got non-null"); \
    }

/// \brief Assert not null
#define HG_TEST_ASSERT_NOT_NULL(PTR, MSG) \
    if ((PTR) == nullptr) { \
        throw std::runtime_error(std::string(MSG) + ": expected non-null but got null"); \
    }

/// \brief Assert string equals
#define HG_TEST_ASSERT_STREQ(EXPECTED, ACTUAL, MSG) \
    if (std::string(EXPECTED) != std::string(ACTUAL)) { \
        throw std::runtime_error(std::string(MSG) + ": expected \"" + EXPECTED + "\" but got \"" + ACTUAL + "\""); \
    }

/// \brief Legacy macro for backward compatibility
#define HG_TEST_START( TEST_NAME )

}
}