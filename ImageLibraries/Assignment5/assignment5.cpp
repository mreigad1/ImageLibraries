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
			1, 1, 1, 1, 1,
			1, 1, 1, 1, 1,
			1, 1, 1, 1, 1,
			1, 1, 1, 1, 1,
			1, 1, 1, 1, 1,
		};
		static mask um(5, arr);
		return um;
	}

	int driver(int argc, char **argv) {
		//ensure correct args
		ASSERT(3 == argc);
		const char* frame_1 = argv[1];
		const char* frame_2 = argv[2];

		std::cout << "\n\nBeginning assignment 4\n";
		std::cout << std::flush;

		//setup all displayable images
		Mat colorFrame1 = imread(frame_1,  CV_LOAD_IMAGE_COLOR);
		Mat colorFrame2 = imread(frame_2,  CV_LOAD_IMAGE_COLOR);
		Mat motionFrame = imread(frame_2,  CV_LOAD_IMAGE_COLOR);

		//ensure all images are valid
		ASSERT(nullptr != colorFrame1.data);
		ASSERT(nullptr != colorFrame2.data);
		ASSERT(nullptr != motionFrame.data);

		//Windowing text
		string windowText = string("Window from ") + __FILE__ + ":" + __FUNCTION__;

		//construct imageGrids
		imageGrid<RGB_P> colorGrid1(colorFrame1.rows, colorFrame1.step / 3, (RGBPix::pixelRGB*)&colorFrame1.data[0]);
		imageGrid<RGB_P> colorGrid2(colorFrame2.rows, colorFrame2.step / 3, (RGBPix::pixelRGB*)&colorFrame2.data[0]);

		bool attempt = false;
		const int sliceSize = 8;
		auto subImages1 = colorGrid1.subImages(sliceSize);
		auto subImages2 = colorGrid2.subImages(sliceSize);

		for (unsigned int i = 0; i < subImages1.Height(); i++) {
			for (unsigned int j = 0; j < subImages1.Width(); j++) {
				coordinate c = motionVectorMap<RGB_P>::process(subImages1[i][j], subImages2[i][j], structuringElement());
				std::cout << "(" << c.y << ", " << c.x << ")\n";
				subImages1[i][j] = subImages1[i][j].toMotionDisplay(c);
			}
		}

		colorGrid1 = imageGrid<RGB_P>::fromSubImageGrid(subImages1, &attempt);

		std::cout << "Was a " << (attempt ? std::string("success") : std::string("failure")) << std::endl;

		colorGrid1.commitImageGrid ((RGB_P*)&motionFrame.data[0]);

		//display results
		imshow("Frame 1 Image", colorFrame1);
		imshow("Frame 2 Image", colorFrame2);
		imshow( "Motion Image", motionFrame);

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