#include <atomic>
#include "gtest/gtest.h"
#include "DelayedConstructor.h"

class DelayedConstructorTests : public ::testing::Test {
public:
    struct Destructed {
        Destructed(int &callCount) : callCount(callCount) {}
        ~Destructed() { ++callCount; }
        int &callCount;
    };

    const int intValue = 42;
    const int tempObjectDestroyed = 1;
    const int DelayedConstructorDestroyed = 1;
    const DelayedConstructor<const int> s0{intValue};
};


TEST_F(DelayedConstructorTests, emptyConstructor) {
    const DelayedConstructor<const int> s{};
}
TEST_F(DelayedConstructorTests, get) {
    EXPECT_EQ(intValue,s0.get());
}
TEST_F(DelayedConstructorTests, construct) {
    DelayedConstructor<const int> s;
    s.construct(intValue);
    EXPECT_EQ(intValue,s.get());
}
TEST_F(DelayedConstructorTests, destruct) {
    int callCount = 0;
    DelayedConstructor<Destructed> s;
    s.construct(Destructed{callCount});
    s.destruct();
    EXPECT_EQ(tempObjectDestroyed + DelayedConstructorDestroyed,callCount);
}
TEST_F(DelayedConstructorTests, constructTwice) {
    DelayedConstructor<const int> s;
    s.construct(intValue);
    s.destruct();
    const int newValue = intValue + 1;
    s.construct(newValue);
    EXPECT_EQ(newValue,s.get());
}
TEST_F(DelayedConstructorTests, destructOutOfScope) {
    int callCount = 0;
    {
        DelayedConstructor<Destructed> s;
        s.construct(Destructed{callCount});
    }
    EXPECT_EQ(tempObjectDestroyed + DelayedConstructorDestroyed,callCount);
}
TEST_F(DelayedConstructorTests, classWithInheritance) {
    struct Base {
        virtual ~Base() {}
        int baseVal = 3;
    };
    struct Derived : Base {
        double derivedVal = 2.3;
    };

    const Derived expected{};
    const DelayedConstructor<Derived> s(Derived{});
    EXPECT_EQ(expected.baseVal,s.get().baseVal);
    EXPECT_EQ(expected.derivedVal,s.get().derivedVal);
}
TEST_F(DelayedConstructorTests, copyConstructor) {
    const DelayedConstructor<const int> s1(s0);
    EXPECT_EQ(intValue,s1.get());
}
TEST_F(DelayedConstructorTests, moveConstructor) {
    DelayedConstructor<const int> sToMove{intValue};
    const DelayedConstructor<const int> s1(std::move(sToMove));
    EXPECT_EQ(intValue,s1.get());
}
TEST_F(DelayedConstructorTests, assignment) {
    DelayedConstructor<const int> s1(intValue+1);
    s1 = s0;
    EXPECT_EQ(intValue,s1.get());
}
TEST_F(DelayedConstructorTests, selfAssignment) {
    DelayedConstructor<const int> s1(s0);
    s1 = s1;
    EXPECT_EQ(intValue,s1.get());
}
TEST_F(DelayedConstructorTests, moveAssignment) {
    DelayedConstructor<const int> s1(intValue+1);
    s1 = std::move(s0);
    EXPECT_EQ(intValue,s1.get());
}
TEST_F(DelayedConstructorTests, selfMoveAssignment) {
    DelayedConstructor<const int> s1(s0);
    s1 = std::move(s1);
    EXPECT_EQ(intValue,s1.get());
}
TEST_F(DelayedConstructorTests, isConstructed) {
    const DelayedConstructor<const int> s{};
    EXPECT_FALSE(s.isConstructed());
    EXPECT_TRUE(s0.isConstructed());
}
TEST_F(DelayedConstructorTests, makeDelayedConstructor) {
    const auto s = makeDelayedConstructor<const int>(intValue);
    EXPECT_EQ(intValue,s.get());
}
TEST_F(DelayedConstructorTests, canHostAtomicConstruct) {
    DelayedConstructor<std::atomic<int>> s;
    s.construct(intValue);
    EXPECT_EQ(intValue,s.get());
}
TEST_F(DelayedConstructorTests, canHostAtomicConstructor) {
    DelayedConstructor<std::atomic<int>> s{intValue};
    EXPECT_EQ(intValue,s.get());
}
