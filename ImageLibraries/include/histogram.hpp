#include "debug.hpp"
#include "constants.hpp"
#include "constants.hpp"
#include "imageGrid.hpp"
#include <iomanip> 
#include <algorithm>
#include <cmath>

#define ArithPix decltype(dummy.Arithmetical())

template <typename PixType> class histogramProcessor {
	PixType dummy;
	//members match arithmetical implementation of pixel type
	ArithPix percent1;
	ArithPix percent99;
	ArithPix median;
	ArithPix average;
	ArithPix pdf[NUM_INTENSITIES];
	const imageGrid<PixType>& image;

	public:

		histogramProcessor(const imageGrid<PixType>& input) : 
			image(input) {

				//initialize pdf
				for (unsigned int i = 0; i < NUM_INTENSITIES; i++) {
					pdf[i] = ArithPix();
				}

				//accumulator to calculate averages
				ArithPix acc;

				//for each pixel in image
				for (unsigned int i = 0; i < image.Height(); i++) {
					for (unsigned int j = 0; j < image.Width(); j++) {

						//get pixel from image
						auto intensity = image.getPixel(i, j);

						//add pixel contents to accumulator
						acc = acc + intensity;

						//get pixel's data for mapping indices
						auto c1 = static_cast<byte>(std::max(std::min(intensity.dataComponent1() * 255.0, 255.0), 0.0));
						auto c2 = static_cast<byte>(std::max(std::min(intensity.dataComponent2() * 255.0, 255.0), 0.0));
						auto c3 = static_cast<byte>(std::max(std::min(intensity.dataComponent3() * 255.0, 255.0), 0.0));

						//retrieve pixels to increment
						ArithPix& p1 = pdf[c1];
						ArithPix& p2 = pdf[c2];
						ArithPix& p3 = pdf[c3];

						//increment the respective pixel components
						p1 = p1 + PixType( 1, 0, 0 );
						p2 = p2 + PixType( 0, 1, 0 );
						p3 = p3 + PixType( 0, 0, 1 );
					}
				}

				//take average
				acc = acc / (image.Height() * image.Width());
				average = acc;

				//clear accumulator
				acc = ArithPix();

				//median components
				PrecisionType median_c1 = -1;
				PrecisionType median_c2 = -1;
				PrecisionType median_c3 = -1;
				PrecisionType p01_c1 = -1;
				PrecisionType p01_c2 = -1;
				PrecisionType p01_c3 = -1;
				PrecisionType p99_c1 = -1;
				PrecisionType p99_c2 = -1;
				PrecisionType p99_c3 = -1;
				PrecisionType acc_c1 = 0;
				PrecisionType acc_c2 = 0;
				PrecisionType acc_c3 = 0;

				//get median components
				for (unsigned int i = 0; i < NUM_INTENSITIES; i++) {
					acc_c1 += pdf[i].dataComponent1();
					acc_c2 += pdf[i].dataComponent2();
					acc_c3 += pdf[i].dataComponent3();
					const PrecisionType size = (image.Height() * image.Width()) / (255.0);
					PrecisionType percentile = 0.5;

					//these data components will be normalized medians
					if (acc_c1 >= size*percentile && median_c1 < 0) {
						median_c1 = i;
					}
					if (acc_c2 >= size*percentile && median_c2 < 0) {
						median_c2 = i;
					}
					if (acc_c3 >= size*percentile && median_c3 < 0) {
						median_c3 = i;
					}

					percentile = 0.01;
					if (acc_c1 >= size*percentile && p01_c1 < 0) {
						p01_c1 = i;
					}
					if (acc_c2 >= size*percentile && p01_c2 < 0) {
						p01_c2 = i;
					}
					if (acc_c3 >= size*percentile && p01_c3 < 0) {
						p01_c3 = i;
					}

					percentile = 0.99;
					if (acc_c1 >= size*percentile && p99_c1 < 0) {
						p99_c1 = i;
					}
					if (acc_c2 >= size*percentile && p99_c2 < 0) {
						p99_c2 = i;
					}
					if (acc_c3 >= size*percentile && p99_c3 < 0) {
						p99_c3 = i;
					}
				}

				//set median pixel
				percent1 = PixType(p01_c1, p01_c2, p01_c3);
				median = PixType(median_c1, median_c2, median_c3);
				percent99 = PixType(p99_c1, p99_c2, p99_c3);
				std::cout << std::endl << __LINE__ << percent1.toString() << std::flush;
				std::cout << std::endl << __LINE__ << median.toString() << std::flush;
				std::cout << std::endl << __LINE__ << percent99.toString() << std::flush;
		}

		PixType histogramShift(const PixType& original) const {
			PixType rv = original;

			//these are in range [0, 1]
			const          byte             numComps = 3;
			const PrecisionType                   c1 = original.dataComponent1();
			const PrecisionType                   c2 = original.dataComponent2();
			const PrecisionType                   c3 = original.dataComponent3();
			const PrecisionType               p99_c1 = percent99.dataComponent1();
			const PrecisionType               p99_c2 = percent99.dataComponent2();
			const PrecisionType               p99_c3 = percent99.dataComponent3();
			const PrecisionType               p01_c1 = percent1.dataComponent1();
			const PrecisionType               p01_c2 = percent1.dataComponent2();
			const PrecisionType               p01_c3 = percent1.dataComponent3();
			const PrecisionType components[numComps] = { c1, c2, c3 };
			const PrecisionType       lows[numComps] = { p01_c1, p01_c2, p01_c3 };
			const PrecisionType      highs[numComps] = { p99_c1, p99_c2, p99_c3 };
			      PrecisionType    results[numComps] = {  0,  0,  0 };

			for (byte i = 0; i < numComps; i++) {
				results[i] = (components[i] - lows[i]) / (highs[i] - lows[i]);
				results[i] = std::min(std::max(results[i] * 1.0, 0.0), 1.0);
			}

			rv = PixType::denormalize(results[0], results[1], results[2]);
			return PixType(rv);
		}

		imageGrid<PixType> histogramCorrection() const {
			imageGrid<PixType> tmp_image = image;
			for (unsigned int i = 0; i < image.Height(); i++) {
				for (unsigned int j = 0; j < image.Width(); j++) {
					tmp_image.getPixel(i, j) = histogramShift(image.getPixel(i, j));
				}
			}
			return tmp_image;
		}

		void display() const {
			PrecisionType sum = 0;
			std::cout << "\n\nComponent 1 Histogram (round off error present):\n";
			for (unsigned int i = 0; i < NUM_INTENSITIES; i++) {
				std::cout << std::setfill ('0') << std::setw(3) << i << " : ";
				sum += pdf[i].dataComponent1();
				std::cout << std::setfill ('0') << std::setw(5) << static_cast<int>(pdf[i].dataComponent1() * 255.0);
				std::cout << "\n";
			}

			std::cout << "\n\nComponent 2 Histogram (round off error present):\n";
			for (unsigned int i = 0; i < NUM_INTENSITIES; i++) {
				std::cout << std::setfill ('0') << std::setw(3) << i << " : ";
				std::cout << std::setfill ('0') << std::setw(5) << static_cast<int>(pdf[i].dataComponent2() * 255.0);
				std::cout << "\n";
			}

			std::cout << "\n\nComponent 3 Histogram (round off error present):\n";
			for (unsigned int i = 0; i < NUM_INTENSITIES; i++) {
				std::cout << std::setfill ('0') << std::setw(3) << i << " : ";
				std::cout << std::setfill ('0') << std::setw(5) << static_cast<int>(pdf[i].dataComponent3() * 255.0);
				std::cout << "\n";
			}
		}

		/*PixType getMinima() const {
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
		}*/
};

