#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "debug.hpp"
#include "pixelGreyscale.h"
#include "pixelRGB.h"
#include "imageGrid.hpp"
#include "histogram.hpp"
#include <vector>
#include <iomanip>

using namespace std;
using namespace cv;

typedef GreyscalePix::pixelGreyscale GREY_P;
typedef RGBPix::pixelRGB RGB_P;
typedef histogramProcessor<RGB_P> histogram;

namespace assignment4 {

	const mask& structuringElement() {
		static PrecisionType arr[] = {
			-1,  0,  4,  0, -1,
			 0,  8,  6,  4,  0,
			 4,  6,  8,  6,  4,
			 0,  4,  6,  8,  0,
			-1,  0,  4,  0, -1,
		};
		static mask um(5, arr);
		return um;
	}

	const mask& unsharpMask() {
		static PrecisionType arr[] = {
			  -1,   -4,   -6,   -4,   -1,
			  -4,  -16,  -24,  -16,   -4,
			  -6,  -24,  476,  -24,   -6,
			  -4,  -16,  -24,  -16,   -4,
			  -1,   -4,   -6,   -4,   -1,
		};
		static mask um(5, arr, 1.0 / 256);
		return um;
	}

	int driver(int argc, char **argv) {
		//ensure correct args
		ASSERT(2 == argc);
		const char* test_file = argv[1];

		std::cout << "\n\nBeginning assignment 4\n";
		std::cout << std::flush;

		//setup all displayable images
		Mat colorImage = imread(test_file,  CV_LOAD_IMAGE_COLOR);
		Mat greyImage = imread(test_file, CV_LOAD_IMAGE_COLOR);
		Mat binImage = imread(test_file, CV_LOAD_IMAGE_COLOR);
		Mat clusteredImage = imread(test_file, CV_LOAD_IMAGE_COLOR);

		//ensure all images are valid
		ASSERT(nullptr != colorImage.data);
		ASSERT(nullptr != greyImage.data);

		//Windowing text
		string windowText = string("Window from ") + __FILE__ + ":" + __FUNCTION__;

		//construct imageGrids
		imageGrid<RGB_P> colorGrid(colorImage.rows, colorImage.step / 3, (RGBPix::pixelRGB*)&colorImage.data[0]);
		imageGrid<GREY_P> greyGrid(colorGrid);

		//convert back to RGB
		//colorGrid = greyGrid;

		//colorGrid = histogramProcessor<RGB_P>(colorGrid).histogramCorrection();
		//colorGrid = colorGrid.multiplyByMask(unsharpMask());
		//colorGrid = colorGrid - colorGrid.sobel();
		//colorGrid = histogramProcessor<RGB_P>(colorGrid).histogramCorrection();

		// auto binMask       = colorGrid.toBinary(histogramProcessor<RGB_P>(colorGrid).binaryThreshold()).toNegative();
		// auto eroded        = colorGrid.erode(structuringElement());
		// auto dilated       = colorGrid.dilate(structuringElement());

		// auto morphGradient = dilated - eroded;
		// auto topHat        = colorGrid - eroded;
		// auto opening       = eroded.dilate(structuringElement());
		// auto closing       = dilated.erode(structuringElement());
		// auto smoothed      = opening + closing;

		// auto morphMask     = morphGradient.toBinary(histogramProcessor<RGB_P>(morphGradient).binaryThreshold());
		// auto textSeg       = smoothed.toBinary(histogramProcessor<RGB_P>(smoothed).binaryThreshold());
		// auto myAlg         = smoothed - (morphMask + binMask);

		// colorGrid = topHat + topHat.dilate(structuringElement()) + morphGradient;

		for (int i = 0; i < 3; i++) {
			auto eroded        = colorGrid.erode(structuringElement());
			auto dilated       = colorGrid.dilate(structuringElement());
			auto morphGradient = dilated - eroded;
			auto topHat        = colorGrid - eroded;
			auto opening       = eroded.dilate(structuringElement());
			colorGrid = (opening - morphGradient);
		}

		auto thresh = histogramProcessor<RGB_P>(colorGrid).binaryThreshold();
		auto binGrid = colorGrid.toBinary(thresh);
		auto clusteredGrid = binGrid;
		auto clustCount = clusteredGrid.clusterImage(thresh).size();
		std::cout << "There are " << clustCount << " pig clusters in image.\n";

		//commit image changes
		colorGrid.commitImageGrid ((RGB_P*)&greyImage.data[0]);
		binGrid.commitImageGrid ((RGB_P*)&binImage.data[0]);
		clusteredGrid.commitImageGrid ((RGB_P*)&clusteredImage.data[0]);

		//display results
		imshow(       "Original Image", colorImage);
		imshow(        "Morphed Image", greyImage);
		imshow("Bin Thresholded Morph", binImage);
		imshow("Grouped Objects Image", clusteredImage);

		//Display loop
		bool loop = true;
		while(loop) {
			switch(cvWaitKey(15)) {
				case 27:  //Esc key case
					loop = false;
					//exit(0);
				break;
				case 32:  //Space bar case
				break;
				default:
				break;
			}
		}

		return 0;
	}
};

int main (int argc, char **argv) {
	bool test = 0;
	test |= assignment4::driver(argc, argv);
	return test;
}