#include "debug.hpp"
#include "constants.hpp"
#include "constants.hpp"
#include "imageGrid.hpp"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>

//TODO: refactor this class to use pixel operations rather than PrecisionType operations
template <typename PixType> class histogramProcessor {
	PixType dummy;
	#define ArithPix decltype(dummy.Arithmetical())

		//members match arithmetical implementation of pixel type
		ArithPix percent1;
		ArithPix percent99;
		ArithPix median;
		ArithPix average;
		ArithPix pdf[NUM_INTENSITIES];
		const imageGrid<PixType>& image;
		const PrecisionType lPercentile;
		const PrecisionType hPercentile;

		public:
			histogramProcessor(const imageGrid<PixType>& input, PrecisionType lowPercentile = 1.0, PrecisionType highPercentile = 99.0) : 
				image(input),
				lPercentile(lowPercentile/100.0),
				hPercentile(highPercentile/100.0) {

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

						percentile = lPercentile;
						if (acc_c1 >= size*percentile && p01_c1 < 0) {
							p01_c1 = i;
						}
						if (acc_c2 >= size*percentile && p01_c2 < 0) {
							p01_c2 = i;
						}
						if (acc_c3 >= size*percentile && p01_c3 < 0) {
							p01_c3 = i;
						}

						percentile = hPercentile;
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

			ArithPix lowPercentilePixel() const {
				return percent1;
			}

			ArithPix highPercentilePixel() const {
				return percent99;
			}

			std::string toString() const {
				std::stringstream ss;

				PrecisionType sum = 0;
				ss << "\n\nComponent 1 Histogram (round off error present):\n";
				for (unsigned int i = 0; i < NUM_INTENSITIES; i++) {
					ss << std::setfill ('0') << std::setw(3) << i << " : ";
					sum += pdf[i].dataComponent1();
					ss << std::setfill ('0') << std::setw(5) << static_cast<int>(pdf[i].dataComponent1() * 255.0);
					ss << "\n";
				}

				ss << "\n\nComponent 2 Histogram (round off error present):\n";
				for (unsigned int i = 0; i < NUM_INTENSITIES; i++) {
					ss << std::setfill ('0') << std::setw(3) << i << " : ";
					ss << std::setfill ('0') << std::setw(5) << static_cast<int>(pdf[i].dataComponent2() * 255.0);
					ss << "\n";
				}

				ss << "\n\nComponent 3 Histogram (round off error present):\n";
				for (unsigned int i = 0; i < NUM_INTENSITIES; i++) {
					ss << std::setfill ('0') << std::setw(3) << i << " : ";
					ss << std::setfill ('0') << std::setw(5) << static_cast<int>(pdf[i].dataComponent3() * 255.0);
					ss << "\n";
				}
				return ss.str();
			}
	#undef ArithPix
};

