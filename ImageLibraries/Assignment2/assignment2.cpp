#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "debug.hpp"
#include "mask.h"
#include "imageGrid.hpp"
#include "pixelRGB.h"

using namespace std;
using namespace cv;

namespace assignment2 {

	typedef RGBPix::pixelRGB RGB_P;

	const mask& UnsharpMask() {
		// static PrecisionType arr[] = {
		// 	-1, -2, -1,
		// 	-2, 13, -2,
		// 	-1, -2, -1
		// };
		// static mask um(3, arr);
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
		FUNC_LOG;
		//ensure correct args
		ASSERT(2 == argc);
		const char* test_file = argv[1];

		//setup all displayable images
		Mat originalImage = imread(test_file, CV_LOAD_IMAGE_COLOR);
		Mat UnsharpImage  = imread(test_file, CV_LOAD_IMAGE_COLOR);
		Mat SobelImage    = imread(test_file, CV_LOAD_IMAGE_COLOR);
		Mat Log7Image     = imread(test_file, CV_LOAD_IMAGE_COLOR);
		Mat Log11Image    = imread(test_file, CV_LOAD_IMAGE_COLOR);

		//ensure all images are valid
		ASSERT(nullptr != originalImage.data);
		ASSERT(nullptr != UnsharpImage.data);
		ASSERT(nullptr != SobelImage.data);
		ASSERT(nullptr != Log7Image.data);
		ASSERT(nullptr != Log11Image.data);

		//Windowing text
		string windowText = string("Window from ") + __FILE__ + ":" + __FUNCTION__;

		//constuct mask
		mask m7 = mask::makeLOG(7, 1.4);
		mask m11 = mask::makeLOG(11, 5.0);
		std::cout << m7.toString() << std::endl;
		std::cout << m11.toString() << std::endl;

		//construct imageGrids
		imageGrid<RGB_P> originalGrid(originalImage.rows, originalImage.step / 3, (RGB_P*)&originalImage.data[0]);
		imageGrid<RGB_P> UnsharpGrid = originalGrid.multiplyByMask(UnsharpMask());
		imageGrid<RGB_P> SobelGrid = originalGrid.sobel();
		imageGrid<RGB_P> Log7Grid = originalGrid.multiplyByMask(m7);
		imageGrid<RGB_P> Log11Grid = originalGrid.multiplyByMask(m11);

		//commit image changes
		originalGrid.commitImageGrid((RGB_P*)&originalImage.data[0]);
		UnsharpGrid.commitImageGrid((RGB_P*)&UnsharpImage.data[0]);
		SobelGrid.commitImageGrid((RGB_P*)&SobelImage.data[0]);
		Log7Grid.commitImageGrid((RGB_P*)&Log7Image.data[0]);
		Log11Grid.commitImageGrid((RGB_P*)&Log11Image.data[0]);

		//display results
		imshow("Original Image", originalImage);
		imshow("Unsharp Image", UnsharpImage);
		imshow("Sobel Image", SobelImage);
		imshow("LoG(  7x7, Sigma = 1.4) Image", Log7Image);
		imshow("LoG(11x11, Sigma = 5.0) Image", Log11Image);

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
	return assignment2::driver(argc, argv);
}