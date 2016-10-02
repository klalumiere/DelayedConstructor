#include "gtest/gtest.h"
#include "StackDeferred.h"

class StackDeferredTests : public ::testing::Test {
public:
    struct Destructed {
        Destructed(int &callCount) : callCount(callCount) {}
        ~Destructed() { ++callCount; }
        int &callCount;
    };

    const int intValue = 42;
    const int tempObjectDestroyed = 1;
    const int stackDeferredDestroyed = 1;
    const StackDeferred<const int> s0{intValue};
};


TEST_F(StackDeferredTests, emptyConstructor) {
    const StackDeferred<const int> s{};
}
TEST_F(StackDeferredTests, get) {
    EXPECT_EQ(intValue,s0.get());
}
TEST_F(StackDeferredTests, construct) {
    StackDeferred<const int> s;
    s.construct(intValue);
    EXPECT_EQ(intValue,s.get());
}
TEST_F(StackDeferredTests, destruct) {
    int callCount = 0;
    StackDeferred<Destructed> s;
    s.construct(Destructed{callCount});
    s.destruct();
    EXPECT_EQ(tempObjectDestroyed + stackDeferredDestroyed,callCount);
}
TEST_F(StackDeferredTests, constructTwice) {
    StackDeferred<const int> s;
    s.construct(intValue);
    s.destruct();
    const int newValue = intValue + 1;
    s.construct(newValue);
    EXPECT_EQ(newValue,s.get());
}
TEST_F(StackDeferredTests, destructOutOfScope) {
    int callCount = 0;
    {
        StackDeferred<Destructed> s;
        s.construct(Destructed{callCount});
    }
    EXPECT_EQ(tempObjectDestroyed + stackDeferredDestroyed,callCount);
}
TEST_F(StackDeferredTests, classWithInheritance) {
    struct Base {
        virtual ~Base() {}
        int baseVal = 3;
    };
    struct Derived : Base {
        double derivedVal = 2.3;
    };

    const Derived expected{};
    const StackDeferred<Derived> s(Derived{});
    EXPECT_EQ(expected.baseVal,s.get().baseVal);
    EXPECT_EQ(expected.derivedVal,s.get().derivedVal);
}
TEST_F(StackDeferredTests, copyConstructor) {
    const StackDeferred<const int> s1(s0);
    EXPECT_EQ(intValue,s1.get());
}
TEST_F(StackDeferredTests, moveConstructor) {
    const StackDeferred<const int> s1(std::move(s0));
    EXPECT_EQ(intValue,s1.get());
}
TEST_F(StackDeferredTests, assignment) {
    StackDeferred<const int> s1(intValue+1);
    s1 = s0;
    EXPECT_EQ(intValue,s1.get());
}
TEST_F(StackDeferredTests, selfAssignment) {
    StackDeferred<const int> s1(s0);
    s1 = s1;
    EXPECT_EQ(intValue,s1.get());
}
TEST_F(StackDeferredTests, moveAssignment) {
    StackDeferred<const int> s1(intValue+1);
    s1 = std::move(s0);
    EXPECT_EQ(intValue,s1.get());
}
TEST_F(StackDeferredTests, selfMoveAssignment) {
    StackDeferred<const int> s1(s0);
    s1 = std::move(s1);
    EXPECT_EQ(intValue,s1.get());
}
TEST_F(StackDeferredTests, isConstructed) {
    const StackDeferred<const int> s{};
    EXPECT_FALSE(s.isConstructed());
    EXPECT_TRUE(s0.isConstructed());
}
TEST_F(StackDeferredTests, make_StackDeferred) {
    const auto s = make_StackDeferred(intValue);
    EXPECT_EQ(intValue,s.get());
}
