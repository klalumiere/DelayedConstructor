#include <atomic>
#include <gtest/gtest.h>
#include <DelayedConstructor/DelayedConstructor.h>

class DelayedConstructorTests : public ::testing::Test {
public:
    struct DestructorSpy {
        DestructorSpy(int &callCount) : callCount(callCount) {}
        ~DestructorSpy() { ++callCount; }
        int &callCount;
    };

    const int arbitraryInt = 42;
    const int rvalueDestroyed = 1;
    const int delayedConstructorDestroyed = 1;
    const DelayedConstructor<const int> dc{arbitraryInt};
};


TEST_F(DelayedConstructorTests, emptyConstructor) {
    const DelayedConstructor<const int> s{};
}
TEST_F(DelayedConstructorTests, get) {
    EXPECT_EQ(arbitraryInt,dc.get());
}
TEST_F(DelayedConstructorTests, construct) {
    DelayedConstructor<const int> s;
    s.construct(arbitraryInt);
    EXPECT_EQ(arbitraryInt,s.get());
}
TEST_F(DelayedConstructorTests, destruct) {
    int callCount = 0;
    DelayedConstructor<DestructorSpy> s;
    s.construct(DestructorSpy{callCount});
    s.destruct();
    EXPECT_EQ(rvalueDestroyed + delayedConstructorDestroyed,callCount);
}
TEST_F(DelayedConstructorTests, constructTwice) {
    DelayedConstructor<const int> s;
    s.construct(arbitraryInt);
    s.destruct();
    const int newValue = arbitraryInt + 1;
    s.construct(newValue);
    EXPECT_EQ(newValue,s.get());
}
TEST_F(DelayedConstructorTests, destructOutOfScope) {
    int callCount = 0;
    {
        DelayedConstructor<DestructorSpy> s;
        s.construct(DestructorSpy{callCount});
    }
    EXPECT_EQ(rvalueDestroyed + delayedConstructorDestroyed,callCount);
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
    const DelayedConstructor<const int> s1(dc);
    EXPECT_EQ(arbitraryInt,s1.get());
}
TEST_F(DelayedConstructorTests, moveConstructor) {
    DelayedConstructor<const int> sToMove{arbitraryInt};
    const DelayedConstructor<const int> s1(std::move(sToMove));
    EXPECT_EQ(arbitraryInt,s1.get());
}
TEST_F(DelayedConstructorTests, assignment) {
    DelayedConstructor<const int> s1(arbitraryInt+1);
    s1 = dc;
    EXPECT_EQ(arbitraryInt,s1.get());
}
TEST_F(DelayedConstructorTests, selfAssignment) {
    DelayedConstructor<const int> s1(dc);
    s1 = s1;
    EXPECT_EQ(arbitraryInt,s1.get());
}
TEST_F(DelayedConstructorTests, moveAssignment) {
    DelayedConstructor<const int> s1(arbitraryInt+1);
    s1 = std::move(dc);
    EXPECT_EQ(arbitraryInt,s1.get());
}
TEST_F(DelayedConstructorTests, selfMoveAssignment) {
    DelayedConstructor<const int> s1(dc);
    s1 = std::move(s1);
    EXPECT_EQ(arbitraryInt,s1.get());
}
TEST_F(DelayedConstructorTests, isConstructed) {
    const DelayedConstructor<const int> s{};
    EXPECT_FALSE(s.isConstructed());
    EXPECT_TRUE(dc.isConstructed());
}
TEST_F(DelayedConstructorTests, createDelayedConstructor) {
    const auto s = createDelayedConstructor<const int>(arbitraryInt);
    EXPECT_EQ(arbitraryInt,s.get());
}
TEST_F(DelayedConstructorTests, canHostAtomicConstruct) {
    DelayedConstructor<std::atomic<int>> s;
    s.construct(arbitraryInt);
    EXPECT_EQ(arbitraryInt,s.get());
}
TEST_F(DelayedConstructorTests, canHostAtomicConstructor) {
    DelayedConstructor<std::atomic<int>> s{arbitraryInt};
    EXPECT_EQ(arbitraryInt,s.get());
}
