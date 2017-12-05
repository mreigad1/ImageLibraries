#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "debug.hpp"
#include "pixelRGB.h"
#include "imageGrid.hpp"
#include "motionVectorMap.hpp"
#include "block.h"
#include "histogram.hpp"
#include <vector>
#include <iomanip>

using namespace std;
using namespace cv;

typedef GreyscalePix::pixelGreyscale Grey_P;

namespace assignment5 {

	const mask& structuringElement() {
		static PrecisionType arr[] = {
			0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,
			0,	0,	0,	0,	1,	1,	1,	1,	1,	1,	1,	0,	0,	0,	0,
			0,	0,	0,	1,	1,	1,	1,	1,	1,	1,	1,	1,	0,	0,	0,
			0,	0,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	0,	0,
			0,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	0,
			0,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	0,
			0,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	0,
			1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,
			0,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	0,
			0,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	0,
			0,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	0,
			0,	0,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	0,	0,
			0,	0,	0,	1,	1,	1,	1,	1,	1,	1,	1,	1,	0,	0,	0,
			0,	0,	0,	0,	1,	1,	1,	1,	1,	1,	1,	0,	0,	0,	0,
			0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,
		};
		static mask um(15, arr);
		return um;
	}

	int driver(int argc, char **argv) {
		//ensure correct args
		ASSERT(2 == argc);
		const char* frame_1 = argv[1];

		std::cout << "\n\nBeginning assignment 5 Part 1\n";
		std::cout << std::flush;

		//setup all displayable images
		Mat colorFrame1 = imread(frame_1, CV_LOAD_IMAGE_GRAYSCALE);

		//ensure all images are valid
		ASSERT(nullptr != colorFrame1.data);

		//Windowing text
		string windowText = string("Window from ") + __FILE__ + ":" + __FUNCTION__;

		//construct imageGrids
		imageGrid<Grey_P> colorGrid1(colorFrame1.rows, colorFrame1.step / 1, (Grey_P*)(&colorFrame1.data[0]));

		colorGrid1 = histogramProcessor<Grey_P>(colorGrid1).histogramCorrection();

		//get array of 4x4 subimages
		auto subImages = colorGrid1.subImages(4);
		auto processedSubs = subImages;
		for (unsigned  i = 0; i < subImages.Height()/2; i++) {
			for (unsigned j = 0; j < subImages.Width(); j++) {
				processedSubs[i][j] = subImages[i][j].trainingClassifier();
			}
		}

		colorGrid1 = imageGrid<Grey_P>::fromSubImageGrid(processedSubs);

		colorGrid1.commitImageGrid ((Grey_P*)(&colorFrame1.data[0]));

		//display results
		imshow("Frame 1 Image", colorFrame1);

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