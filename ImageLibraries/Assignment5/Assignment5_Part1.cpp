#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "debug.hpp"
#include "pixelRGB.h"
#include "imageGrid.hpp"
#include "motionVectorMap.hpp"
#include "block.h"
#include "histogram.hpp"
#include "GreyscaleClassification.h"
#include <vector>
#include <iomanip>

using namespace std;
using namespace cv;

typedef GreyscalePix::pixelGreyscale Grey_P;

namespace assignment5 {
	int driver(int argc, char **argv) {
		//ensure correct args
		ASSERT(2 == argc);
		const char* frame_1 = argv[1];

		std::cout << "\n\nBeginning assignment 5 Part 1\n";
		std::cout << std::flush;

		//setup all displayable images
		Mat colorFrame1 = imread(frame_1, CV_LOAD_IMAGE_GRAYSCALE);
		Mat M1_Frame = imread(frame_1, CV_LOAD_IMAGE_GRAYSCALE);


		//ensure all images are valid
		ASSERT(nullptr != colorFrame1.data);

		//Windowing text
		string windowText = string("Window from ") + __FILE__ + ":" + __FUNCTION__;

		//construct imageGrids
		imageGrid<Grey_P> colorGrid1(colorFrame1.rows, colorFrame1.step / 1, (Grey_P*)(&colorFrame1.data[0]));

		colorGrid1 = histogramProcessor<Grey_P>(colorGrid1).histogramCorrection();

		//get array of 4x4 subimages
		auto subImages = colorGrid1.subImages(4);
		auto classEngine = NearestNeighbor(subImages);

		auto processedSubs = classEngine.getN1();

		imageGrid<Grey_P> M1 = imageGrid<Grey_P>::fromSubImageGrid(processedSubs);

		M1.commitImageGrid ((Grey_P*)(&M1_Frame.data[0]));

		//display results
		imshow("Original Image", colorFrame1);
		imshow("Trained Image", M1_Frame);

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
	test |= assignment5::driver(argc, argv);
	return test;
}