#include "debug.hpp"
#include "constants.hpp"
#include "imageGrid.hpp"
#include "pixelRGB.h"
#include "mask.h"

namespace ImageGridTest {
	imageGrid<RGBPix::pixelRGB> getGrid() {
		const int height = 8;
		const int width = 6;
		RGBPix::pixelRGB* data = new RGBPix::pixelRGB[height * width];
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				data[i * width + j] = RGBPix::pixelRGB(i * i & MAX_BYTE, j * i & MAX_BYTE, j * j & MAX_BYTE);
			}
		}
		imageGrid<RGBPix::pixelRGB> smallStart(height, width, data)
;		delete[] data;
		data = NULL;
		return smallStart;
	}

	mask getMask() {
		return mask::makeLOG(3, 1.5);
	}

	int driver() {

		auto my_grid = getGrid();
		auto my_mask = getMask();

		my_grid.toNegative();

		//my_grid.multiply(my_mask);

		return 0;
	}
};

int main() {
	int retVal = 0;
	std::cout << "Starting ImageGridTest|";
	retVal = ImageGridTest::driver();
	std::cout << "ImageGridTest Finished.\n";
	return retVal;
}