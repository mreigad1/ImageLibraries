#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "debug.hpp"
#include "pixelRGB.h"
#include "imageGrid.hpp"
#include "motionVectorMap.hpp"
#include "block.h"
#include <vector>
#include <iomanip>

using namespace std;
using namespace cv;

typedef RGBPix::pixelRGB RGB_P;

namespace assignment5 {

	const mask& structuringElement() {
		static PrecisionType arr[] = {
			// 0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,
			// 0,	0,	0,	1,	1,	1,	1,	1,	1,	1,	0,	0,	0,
			// 0,	0,	1,	1,	1,	1,	1,	1,	1,	1,	1,	0,	0,
			// 0,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	0,
			// 0,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	0,
			// 0,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	0,
			// 1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,
			// 0,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	0,
			// 0,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	0,
			// 0,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	0,
			// 0,	0,	1,	1,	1,	1,	1,	1,	1,	1,	1,	0,	0,
			// 0,	0,	0,	1,	1,	1,	1,	1,	1,	1,	0,	0,	0,
			// 0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,
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
		ASSERT(3 == argc);
		const char* frame_1 = argv[1];
		const char* frame_2 = argv[2];

		std::cout << "\n\nBeginning assignment 5\n";
		std::cout << std::flush;

		//setup all displayable images
		Mat colorFrame1 = imread(frame_1,  CV_LOAD_IMAGE_COLOR);
		Mat colorFrame2 = imread(frame_2,  CV_LOAD_IMAGE_COLOR);
		Mat motionFrame = imread(frame_2,  CV_LOAD_IMAGE_COLOR);
		Mat diffFrame = imread(frame_2,  CV_LOAD_IMAGE_COLOR);
		Mat reconstructedFrame = imread(frame_2,  CV_LOAD_IMAGE_COLOR);

		//ensure all images are valid
		ASSERT(nullptr != colorFrame1.data);
		ASSERT(nullptr != colorFrame2.data);
		ASSERT(nullptr != motionFrame.data);

		//Windowing text
		string windowText = string("Window from ") + __FILE__ + ":" + __FUNCTION__;

		//construct imageGrids
		imageGrid<RGB_P> colorGrid1(colorFrame1.rows, colorFrame1.step / 3, (RGBPix::pixelRGB*)&colorFrame1.data[0]);
		imageGrid<RGB_P> colorGrid2(colorFrame2.rows, colorFrame2.step / 3, (RGBPix::pixelRGB*)&colorFrame2.data[0]);

		Array2D<coordinate> movementVectors = motionVectorMap<RGBPix::arithmeticalRGB>::motionEstimation(colorGrid1, colorGrid2, structuringElement(), 1);
		imageGrid<RGB_P> motionComp = colorGrid2.motionCompensated(colorGrid1, movementVectors);
		imageGrid<RGB_P> imageDiff = colorGrid2 - colorGrid1;
		imageGrid<RGB_P> reconstructed = imageGrid<RGB_P>::reassembleMotion(motionComp, colorGrid1, movementVectors);

		//std::cout << "Was a " << (attempt ? std::string("success") : std::string("failure")) << std::endl;

		motionComp.commitImageGrid ((RGB_P*)&motionFrame.data[0]);
		imageDiff.commitImageGrid ((RGB_P*)&diffFrame.data[0]);
		reconstructed.commitImageGrid ((RGB_P*)&reconstructedFrame.data[0]);

		//display results
		imshow("Frame 1 Image", colorFrame1);
		imshow("Frame 2 Image", colorFrame2);
		imshow( "Motion Image", motionFrame);
		imshow( "Difference Image", diffFrame);
		imshow( "Reconstructed Image", reconstructedFrame);

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