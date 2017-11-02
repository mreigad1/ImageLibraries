#include "debug.hpp"
#include "constants.hpp"
#include "histogram.h"
#include "imageGrid.hpp"
#include <iomanip> 
#include <algorithm>

histogram::histogram(imageGrid<RGB_P>& input) : image(input){

		for (unsigned int i = 0; i < NUM_INTENSITIES; i++) {
			pdf[i] = 0;
		}

		unsigned long long acc = 0;
		for (unsigned int i = 0; i < image.Height(); i++) {
			for (unsigned int j = 0; j < image.Width(); j++) {
				unsigned int intensity = image.getPixel(i, j).getAvgIntensity();
				pdf[intensity]++;
				acc += intensity;
			}
		}
		acc += (MAX_BYTE / 2);
		acc /= (image.Height() * image.Width());
		average = (unsigned int)acc;
		acc = 0;

		for (unsigned int i = 0; i < NUM_INTENSITIES; i++) {
			acc += pdf[i];
			if (acc >= ((image.Height() * image.Width()) / 2)) {
				median = i;
				break;
			}
		}
}

void histogram::apply() {
	imageGrid<RGB_P> tmp_image = image;
	for (unsigned int i = 0; i < image.Height(); i++) {
		for (unsigned int j = 0; j < image.Width(); j++) {
			tmp_image.getPixel(i, j) = image.getPixel(i, j).histogramShift(average);
		}
	}
	image = tmp_image;
}

void histogram::display() {
	const int divisor = 5;
	const int N_BUCKETS = (NUM_INTENSITIES / divisor) + 1;
	unsigned int buckets[N_BUCKETS];
	for (unsigned int i = 0; i < N_BUCKETS; i++) {
		buckets[i] = 0;
	}

	for (unsigned int i = 0; i < NUM_INTENSITIES; i++) {
		const int bucket = i / divisor;
		buckets[bucket] += pdf[i];
	}

	for (unsigned int i = 0; i < N_BUCKETS; i++) {
		std::cout << std::endl;
		std::cout << "Greyscale intensity level ";
		std::cout << std::setfill ('0') << std::setw(3) << (i*divisor) << "-";
		std::cout << std::setfill ('0') << std::setw(3) << (((i+1)*divisor)-1);
		std::cout << ":\t" << buckets[i];
	}
	std::cout << std::endl;
}

byte histogram::getMinima() {
	byte rv = std::min(static_cast<int>(average + median), 0xff);

	byte precedentWindow = 15;
	byte antecedentWindow = 15;
	byte requiredAntecedents = 13;
	byte requiredPrecedents = 13;

	int cdf_i = 1;

	for (byte i = (NUM_INTENSITIES - antecedentWindow); i > precedentWindow; i--) {
		int anteCount = 0;
		int preCount = 0;
		for (byte j = 1; j <= antecedentWindow; j++) {
			if (pdf[i] <= pdf[i + j]) {
				anteCount++;
			}
		}
		for (byte j = 1; j <= precedentWindow; j++) {
			if (pdf[i] >= pdf[i - j]) {
				preCount++;
			}
		}
		if ((anteCount >= requiredAntecedents && preCount >= requiredPrecedents) || ((image.Height() * image.Width() / cdf_i) <= 2)) {
			rv = i;
			break;
		}
		cdf_i += pdf[i];
	}

	std::cout << "threshold = " << static_cast<int>(rv) << std::endl << std::flush;
	return rv;
}

#ifdef TESTING
	#define DRIVER main
#else
	#define DRIVER histogram_driver
#endif

int DRIVER (int argc, char **argv) {
	return 0;
}
