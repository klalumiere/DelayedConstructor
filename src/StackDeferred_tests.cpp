#include "gtest/gtest.h"
#include "StackDeferred.h"

class StackDeferredTests : public ::testing::Test {
public:
    struct Destructed {
        Destructed(int &callCount) : callCount(callCount) {}
        ~Destructed() { ++callCount; }
        int &callCount;
    };

    const int intValue = 3;
    const int tempObjectDestroyed = 1;
    const int stackDeferredDestroyed = 1;
};


TEST_F(StackDeferredTests, emptyConstructor) {
	const StackDeferred<const int> s{};
}
TEST_F(StackDeferredTests, get) {
	const StackDeferred<const int> s(intValue);
	EXPECT_EQ(intValue,s.get());
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
    const int newValue = 42;
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
