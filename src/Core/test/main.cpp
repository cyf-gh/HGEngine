#include <Core/Test.hpp>

// Example test cases
void TestMathVec2() {
    HG::Math::HGVec2<float> v1(1.0f, 2.0f);
    HG::Math::HGVec2<float> v2(3.0f, 4.0f);
    
    auto sum = HG::Math::HGVec2<float>::Add(v1, v2);
    HG_TEST_ASSERT_EQ(4.0f, sum.X, "Vec2 Add X");
    HG_TEST_ASSERT_EQ(6.0f, sum.Y, "Vec2 Add Y");
}

void TestMathRect() {
    HG::Math::HGRect rect(10, 20, 100, 50);
    
    HG_TEST_ASSERT_EQ(10, rect.Left(), "Rect Left");
    HG_TEST_ASSERT_EQ(20, rect.Top(), "Rect Top");
    HG_TEST_ASSERT_EQ(110, rect.Right(), "Rect Right");
    HG_TEST_ASSERT_EQ(70, rect.Bottom(), "Rect Bottom");
}

void TestMathOverlap() {
    HG::Math::HGRect rect1(0, 0, 100, 100);
    HG::Math::HGRect rect2(50, 50, 100, 100);
    
    HG_TEST_ASSERT_TRUE(rect1.IsOverlap(rect2), "Rect overlap test");
}

void TestMemoryPool() {
    HG::Memory::ObjectPool<int> pool;
    pool.PreAllocates(10);
    
    HG_TEST_ASSERT_EQ(10u, pool.Size(), "Pool initial size");
    
    int* p1 = pool.Acquire();
    HG_TEST_ASSERT_NOT_NULL(p1, "Pool acquire");
    HG_TEST_ASSERT_EQ(9u, pool.Size(), "Pool size after acquire");
    
    pool.Release(p1);
    HG_TEST_ASSERT_EQ(10u, pool.Size(), "Pool size after release");
}

int main() {
    // Register tests
    HG_TEST_REGISTER("Math::HGVec2 Add", TestMathVec2);
    HG_TEST_REGISTER("Math::HGRect Bounds", TestMathRect);
    HG_TEST_REGISTER("Math::HGRect Overlap", TestMathOverlap);
    HG_TEST_REGISTER("Memory::ObjectPool", TestMemoryPool);
    
    // Run all tests
    HG_TEST_RUN_ALL();
    
    // Return exit code
    return HG_TEST_GET_RESULTS().IsAllPassed() ? 0 : 1;
}
