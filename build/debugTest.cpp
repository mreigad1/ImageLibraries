#include "debug.hpp"
namespace debugTest {
	void test_STR() {
		std::cout << STR(this is a string\n);
	}

	void test_ASSERT(int num = 19256184) {
		ASSERT(true);
		ASSERT(1 == 1);
		ASSERT(2 + 1 - 10 * 11 / 32 == num);
	}

	void test_TBI() {
		TBI;
	}
};

int main(int argc, char *argv[]) {
	ASSERT(2 == argc);
	ASSERT('A' == argv[1][0] || 'T' == argv[1][0]);
	debugTest::test_STR();
	if ('A' == argv[1][0]) {
		debugTest::test_ASSERT(-1);
	} else if ('T' == argv[1][0]) {
		debugTest::test_TBI();
	} else {
		ASSERT(0 == STR(Line should be unreachable));
	}
}