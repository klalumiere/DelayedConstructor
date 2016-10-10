#include <atomic>
#include "gtest/gtest.h"
#include "StaticConstructor.h"

class StaticConstructorTests : public ::testing::Test {
public:
    struct Destructed {
        Destructed(int &callCount) : callCount(callCount) {}
        ~Destructed() { ++callCount; }
        int &callCount;
    };

    const int intValue = 42;
    const int tempObjectDestroyed = 1;
    const int StaticConstructorDestroyed = 1;
    const StaticConstructor<const int> s0{intValue};
};


TEST_F(StaticConstructorTests, emptyConstructor) {
    const StaticConstructor<const int> s{};
}
TEST_F(StaticConstructorTests, get) {
    EXPECT_EQ(intValue,s0.get());
}
TEST_F(StaticConstructorTests, construct) {
    StaticConstructor<const int> s;
    s.construct(intValue);
    EXPECT_EQ(intValue,s.get());
}
TEST_F(StaticConstructorTests, destruct) {
    int callCount = 0;
    StaticConstructor<Destructed> s;
    s.construct(Destructed{callCount});
    s.destruct();
    EXPECT_EQ(tempObjectDestroyed + StaticConstructorDestroyed,callCount);
}
TEST_F(StaticConstructorTests, constructTwice) {
    StaticConstructor<const int> s;
    s.construct(intValue);
    s.destruct();
    const int newValue = intValue + 1;
    s.construct(newValue);
    EXPECT_EQ(newValue,s.get());
}
TEST_F(StaticConstructorTests, destructOutOfScope) {
    int callCount = 0;
    {
        StaticConstructor<Destructed> s;
        s.construct(Destructed{callCount});
    }
    EXPECT_EQ(tempObjectDestroyed + StaticConstructorDestroyed,callCount);
}
TEST_F(StaticConstructorTests, classWithInheritance) {
    struct Base {
        virtual ~Base() {}
        int baseVal = 3;
    };
    struct Derived : Base {
        double derivedVal = 2.3;
    };

    const Derived expected{};
    const StaticConstructor<Derived> s(Derived{});
    EXPECT_EQ(expected.baseVal,s.get().baseVal);
    EXPECT_EQ(expected.derivedVal,s.get().derivedVal);
}
TEST_F(StaticConstructorTests, copyConstructor) {
    const StaticConstructor<const int> s1(s0);
    EXPECT_EQ(intValue,s1.get());
}
TEST_F(StaticConstructorTests, moveConstructor) {
    const StaticConstructor<const int> s1(std::move(s0));
    EXPECT_EQ(intValue,s1.get());
}
TEST_F(StaticConstructorTests, assignment) {
    StaticConstructor<const int> s1(intValue+1);
    s1 = s0;
    EXPECT_EQ(intValue,s1.get());
}
TEST_F(StaticConstructorTests, selfAssignment) {
    StaticConstructor<const int> s1(s0);
    s1 = s1;
    EXPECT_EQ(intValue,s1.get());
}
TEST_F(StaticConstructorTests, moveAssignment) {
    StaticConstructor<const int> s1(intValue+1);
    s1 = std::move(s0);
    EXPECT_EQ(intValue,s1.get());
}
TEST_F(StaticConstructorTests, selfMoveAssignment) {
    StaticConstructor<const int> s1(s0);
    s1 = std::move(s1);
    EXPECT_EQ(intValue,s1.get());
}
TEST_F(StaticConstructorTests, isConstructed) {
    const StaticConstructor<const int> s{};
    EXPECT_FALSE(s.isConstructed());
    EXPECT_TRUE(s0.isConstructed());
}
TEST_F(StaticConstructorTests, make_StaticConstructor) {
    const auto s = make_StaticConstructor(intValue);
    EXPECT_EQ(intValue,s.get());
}
TEST_F(StaticConstructorTests, canHostAtomicConstruct) {
    StaticConstructor<std::atomic<char>> s;
    s.construct('s');
}
