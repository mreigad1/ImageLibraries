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
		Mat N1_Frame = imread(frame_1, CV_LOAD_IMAGE_GRAYSCALE);
		Mat N2_Frame = imread(frame_1, CV_LOAD_IMAGE_GRAYSCALE);
		Mat N3_Frame = imread(frame_1, CV_LOAD_IMAGE_GRAYSCALE);
		Mat N4_Frame = imread(frame_1, CV_LOAD_IMAGE_GRAYSCALE);
		Mat T1_Frame = imread(frame_1, CV_LOAD_IMAGE_GRAYSCALE);
		Mat K1_Frame = imread(frame_1, CV_LOAD_IMAGE_GRAYSCALE);

		//ensure all images are valid
		ASSERT(nullptr != colorFrame1.data);

		//Windowing text
		string windowText = string("Window from ") + __FILE__ + ":" + __FUNCTION__;

		//construct imageGrids
		imageGrid<Grey_P> colorGrid1(colorFrame1.rows, colorFrame1.step / 1, (Grey_P*)(&colorFrame1.data[0]));

		//colorGrid1 = histogramProcessor<Grey_P>(colorGrid1).histogramCorrection();

		const int learningVectorSize = 4;

		//get array of 4x4 subimages
		auto subImages = colorGrid1.subImages(learningVectorSize);
		//auto classEngine = NearestNeighbor(subImages);

		auto processedM1 = NearestNeighbor(subImages).getM1();
		auto processedN1 = NearestNeighbor(subImages).getN1();
		auto processedN2 = NearestNeighbor(subImages).getN2();
		auto processedN3 = NearestNeighbor(subImages).getN3();
		auto processedN4 = NearestNeighbor(subImages).getN4();
		auto processedT1 = NearestNeighbor(subImages).getT1();
		auto processedK1 =          kMeans(subImages).getM1();

		imageGrid<Grey_P> M1 = imageGrid<Grey_P>::fromSubImageGrid(processedM1);
		imageGrid<Grey_P> N1 = imageGrid<Grey_P>::fromSubImageGrid(processedN1);
		imageGrid<Grey_P> N2 = imageGrid<Grey_P>::fromSubImageGrid(processedN2);
		imageGrid<Grey_P> N3 = imageGrid<Grey_P>::fromSubImageGrid(processedN3);
		imageGrid<Grey_P> N4 = imageGrid<Grey_P>::fromSubImageGrid(processedN4);
		imageGrid<Grey_P> T1 = imageGrid<Grey_P>::fromSubImageGrid(processedT1);
		imageGrid<Grey_P> K1 = imageGrid<Grey_P>::fromSubImageGrid(processedK1);

		M1.commitImageGrid ((Grey_P*)(&M1_Frame.data[0]));
		N1.commitImageGrid ((Grey_P*)(&N1_Frame.data[0]));
		N2.commitImageGrid ((Grey_P*)(&N2_Frame.data[0]));
		N3.commitImageGrid ((Grey_P*)(&N3_Frame.data[0]));
		N4.commitImageGrid ((Grey_P*)(&N4_Frame.data[0]));
		T1.commitImageGrid ((Grey_P*)(&T1_Frame.data[0]));
		K1.commitImageGrid ((Grey_P*)(&K1_Frame.data[0]));

		const PrecisionType numDiffSubImages = T1.getNumDiffPixels(N1) / (T1.Height() * T1.Width());
		std::cout << "Between Trained T1 and Tested N1, error rate E = " << numDiffSubImages * 200 << "%\n"; //this is 200 because its percentage and only half of image is trained

		//display results
		imshow("Original Image", colorFrame1);
		imshow("Trained Image (M1)", M1_Frame);
		imshow("Testing Image (N1)", N1_Frame);
		imshow("Testing Image (N2)", N2_Frame);
		imshow("Testing Image (N3)", N3_Frame);
		imshow("Testing Image (N4)", N4_Frame);
		imshow("Testing Image (T1)", T1_Frame);
		imshow("K-Means Image (K1)", K1_Frame);

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