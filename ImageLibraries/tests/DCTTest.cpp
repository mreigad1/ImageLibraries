#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "debug.hpp"
#include "pixelGreyscale.h"
#include "imageGrid.hpp"
#include "DCT.h"

using namespace std;
using namespace cv;

namespace assignment2 {

	int driver(int argc, char **argv) {
		FUNC_LOG;
		//ensure correct args
		ASSERT(2 == argc);
		const char* test_file = argv[1];

		//setup all displayable images
		Mat originalImage = imread(test_file, CV_LOAD_IMAGE_GRAYSCALE);
		Mat dctProcessedImage = imread(test_file, CV_LOAD_IMAGE_GRAYSCALE);
		Mat idctProcessedImage = imread(test_file, CV_LOAD_IMAGE_GRAYSCALE);
		LINE_LOG;
		//ensure all images are valid
		ASSERT(nullptr != originalImage.data);
		LINE_LOG;
		//Windowing text
		string windowText = string("Window from ") + __FILE__ + ":" + __FUNCTION__;

		//construct imageGrids
		imageGrid<GreyscalePix::pixelGreyscale> originalGrid(originalImage.rows, originalImage.step / 1, (GreyscalePix::pixelGreyscale*)&originalImage.data[0]);
		LINE_LOG;
		//construct DCT's
		DCTImage dctProcessor(originalGrid);
		LINE_LOG;
		imageGrid<GreyscalePix::pixelGreyscale> DCT_G = dctProcessor.ComputeDct();
		imageGrid<GreyscalePix::pixelGreyscale> IDCT_G = dctProcessor.ComputeDct();
		LINE_LOG;
		//commit image changes
		originalGrid.commitImageGrid((GreyscalePix::pixelGreyscale*)&originalImage.data[0]);
		DCT_G.commitImageGrid((GreyscalePix::pixelGreyscale*)&dctProcessedImage.data[0]);
		IDCT_G.commitImageGrid((GreyscalePix::pixelGreyscale*)&idctProcessedImage.data[0]);
		LINE_LOG;
		//display results
		imshow("Original Image", originalImage);
		imshow("DCT Image", dctProcessedImage);
		imshow("IDCT Image", idctProcessedImage);

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