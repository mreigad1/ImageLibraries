#include "debug.hpp"
#include "constants.hpp"
#include "constants.hpp"
#include "imageGrid.hpp"
#include <iomanip> 
#include <algorithm>
#include <functional>

#define ArithPix decltype(dummy.Arithmetical())

template <typename PixType> class histogramProcessor {
	PixType dummy;
	//members match arithmetical implementation of pixel type
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
						auto c1 = static_cast<byte>(intensity.dataComponent1() * 255);
						auto c2 = static_cast<byte>(intensity.dataComponent2() * 255);
						auto c3 = static_cast<byte>(intensity.dataComponent3() * 255);

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

				//get median components
				for (unsigned int i = 0; i < NUM_INTENSITIES; i++) {
					acc = acc + pdf[i];
					const int size = (image.Height() * image.Width()) / 2;

					//these data components will be normalized medians
					if (acc.dataComponent1() >= size && median_c1 < 0) {
						median_c1 = i / 255.0;	
					}
					if (acc.dataComponent2() >= size && median_c2 < 0) {
						median_c2 = i / 255.0;
					}
					if (acc.dataComponent3() >= size && median_c3 < 0) {
						median_c3 = i / 255.0;
					}
				}

				//set median pixel
				median = PixType(median_c1, median_c2, median_c3);
		}

		PixType histogramShift(const PixType& original) const {
			PixType rv = original;

			//these are in range [0, 1]
			const byte numComps = 3;
			const PrecisionType c1 = (original.dataComponent1());
			const PrecisionType c2 = (original.dataComponent2());
			const PrecisionType c3 = (original.dataComponent3());
			const PrecisionType m1 = static_cast<byte>(median.dataComponent1());
			const PrecisionType m2 = static_cast<byte>(median.dataComponent2());
			const PrecisionType m3 = static_cast<byte>(median.dataComponent3());
			const PrecisionType components[numComps] = { c1, c2, c3 };
			const PrecisionType medians[numComps] = { m1, m2, m3 };
			      PrecisionType results[numComps] = {  0,  0,  0 };

			for (byte i = 0; i < numComps; i++) {

				const PrecisionType contrast = medians[i];
				PrecisionType F = (1.016 * (contrast + 1.0)) / (1.0 * (1.016 - contrast));
				results[i] = F * ((components[i]) - 0.5) + 0.5;
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
			image = tmp_image;
		}

		void display() const {
			std::cout << "\n\nComponent 1 Histogram:\n";
			for (unsigned int i = 0; i < NUM_INTENSITIES; i++) {
				std::cout << std::setfill ('0') << std::setw(3) << i << " : ";
				std::cout << std::setfill ('0') << std::setw(5) << static_cast<int>(pdf[i].dataComponent1());
				std::cout << "\n";
			}

			std::cout << "\n\nComponent 2 Histogram:\n";
			for (unsigned int i = 0; i < NUM_INTENSITIES; i++) {
				std::cout << std::setfill ('0') << std::setw(3) << i << " : ";
				std::cout << std::setfill ('0') << std::setw(5) << static_cast<int>(pdf[i].dataComponent2());
				std::cout << "\n";
			}

			std::cout << "\n\nComponent 3 Histogram:\n";
			for (unsigned int i = 0; i < NUM_INTENSITIES; i++) {
				std::cout << std::setfill ('0') << std::setw(3) << i << " : ";
				std::cout << std::setfill ('0') << std::setw(5) << static_cast<int>(pdf[i].dataComponent3());
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

