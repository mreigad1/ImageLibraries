#include "debug.hpp"
#include "constants.hpp"
#include "pixelRGB.h"
#include "imageGrid.hpp"

typedef RGBPix::pixelRGB RGB_P;

class histogram {
	unsigned int median;
	unsigned int average;
	unsigned int pdf[NUM_INTENSITIES];
	imageGrid<RGB_P>& image;

	public:
		histogram(imageGrid<RGB_P>& input);
		void apply();
		void display();
		byte getMinima();
};