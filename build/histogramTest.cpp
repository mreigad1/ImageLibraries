#include "imageGrid.hpp"
#include "histogram.hpp"
#include "pixelRGB.h"

typedef RGBPix::pixelRGB RGB_P;
typedef histogramProcessor<RGB_P> histogram;

namespace histogramTest {
	imageGrid<RGB_P> getGrid() {
		const int height = 8;
		const int width = 6;
		RGB_P* data = new RGB_P[height * width];
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				data[i * width + j] = RGB_P(i * i & MAX_BYTE, j * i & MAX_BYTE, j * j & MAX_BYTE);
			}
		}
		imageGrid<RGB_P> smallStart(height, width, data);
		delete[] data;
		data = NULL;
		return smallStart;
	}

	int histogram_driver(int argc, char **argv) {

		imageGrid<RGB_P> testImg = getGrid();

		histogram h(testImg);

		//byte thresh = getMinima()

		return 0;
	}
};

int main (int argc, char **argv) {
	return histogramTest::histogram_driver(argc, argv);
}