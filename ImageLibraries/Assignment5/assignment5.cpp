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
			-1,  0,  2,  0, -1,
			 0,  4,  8,  4,  0,
			 2,  8,  2,  8,  2,
			 0,  4,  8,  4,  0,
			-1,  0,  2,  0, -1,
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

		//ensure all images are valid
		ASSERT(nullptr != colorImage.data);
		ASSERT(nullptr != greyImage.data);

		//Windowing text
		string windowText = string("Window from ") + __FILE__ + ":" + __FUNCTION__;

		//construct imageGrids
		imageGrid<RGB_P> colorGrid(colorImage.rows, colorImage.step / 3, (RGBPix::pixelRGB*)&colorImage.data[0]);
		imageGrid<GREY_P> greyGrid(colorGrid);

		//convert back to RGB
		colorGrid = greyGrid;


		colorGrid = histogramProcessor<RGB_P>(colorGrid).histogramCorrection();
		colorGrid = colorGrid.multiplyByMask(unsharpMask());
		//colorGrid = colorGrid - colorGrid.sobel();
		colorGrid = histogramProcessor<RGB_P>(colorGrid).histogramCorrection();

		auto smoothed      = colorGrid.erode(structuringElement()).dilate(structuringElement()) + colorGrid.dilate(structuringElement()).erode(structuringElement());
		auto morphGradient = colorGrid.dilate(structuringElement()) - colorGrid.erode(structuringElement());
		auto topHat        = colorGrid - colorGrid.erode(structuringElement()).dilate(structuringElement());
		auto textSeg       = smoothed.toBinary(histogramProcessor<RGB_P>(smoothed).binaryThreshold());
		auto myAlg = colorGrid - (textSeg + smoothed).toNegative() + (smoothed - (morphGradient + topHat) - (morphGradient + topHat));//.erode(structuringElement()).dilate(structuringElement());
		//auto myAlg2 = (((colorGrid - myAlg) + (smoothed - myAlg)) - morphGradient); 
		colorGrid = myAlg;


		//colorGrid = colorGrid.toBinary(histogramProcessor<RGB_P>(colorGrid).binaryThreshold());

		//commit image changes
		colorGrid.commitImageGrid ((RGB_P*)&greyImage.data[0]);

		//display results
		imshow(        "Color Image", colorImage);
		imshow("Intensity Image (I)", greyImage);

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