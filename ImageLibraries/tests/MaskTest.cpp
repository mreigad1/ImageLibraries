#include "debug.hpp"
#include "mask.h"

namespace MaskTest {
	int driver(int width, PrecisionType sigma) {
		PrecisionType* test_arr = new PrecisionType[width * width];
		for (auto i = 0; i < width; i++) {
			for (auto j = 0; j < width; j++) {
				test_arr[i * width + j] = (i * i + j * j) / ((i + 1) * (i * j + 1) * (j + 1));
			}
		}
		mask m1;
		mask m2 = mask::makeLOG(width, sigma);
		mask m3(width, test_arr);
		delete [] test_arr;
		test_arr = NULL;
		m1 = mask::makeLOG(width, sigma);
		for (auto i = 0; i < width; i++) {
			for (auto j = 0; j < width; j++) {
				ASSERT(m1.getMaskData()[i][j] == m2.getMaskData()[i][j]);
			}
		}

		PrecisionType acc = 0;
		for (auto i = 0; i < width; i++) {
			for (auto j = 0; j < width; j++) {
				acc += m1.getMaskData()[i][j];
			}
		}
		
		return 0;
	}
};

int main() {
	int retVal = 0;
	std::cout << "Beginning MaskTest|";
	for (auto width = 1; width < 11; width += 2) {
		for (PrecisionType sigma = 1; sigma <= 1.5; sigma += 0.05) {
			retVal |= MaskTest::driver(width, sigma);
		}
	}
	std::cout << "Ending MaskTest\n";
	return retVal;
}