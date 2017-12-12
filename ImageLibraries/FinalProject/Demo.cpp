#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include "debug.hpp"
#include "diffCalculator.h"

using namespace std;
using namespace cv;

namespace finalProject {
	int driver(int argc, char **argv) {
		//ensure correct args
		ASSERT(3 == argc);
		// const int image_height = stoi(string(argv[1]));
		// const int image_width  = stoi(string(argv[2]));
		const string input1 = argv[1]; //filenames of binary image files
		const string input2 = argv[2];

		//setup all displayable images
		Mat inputMat0 = imread(input1.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
		Mat inputMat1 = imread(input2.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
		Mat colorFrame1 = imread(input2.c_str(), CV_LOAD_IMAGE_GRAYSCALE);

		//ensure all images are valid
		ASSERT(nullptr != colorFrame1.data);

		//Windowing text
		string windowText = string("Window from ") + __FILE__ + ":" + __FUNCTION__;

		diffCalculator diff = diffCalculator::makeCalculator(colorFrame1.rows, colorFrame1.step, input1, input2, (byte*)(&inputMat0.data[0]), (byte*)(&inputMat1.data[0]));
		byte* const commitLocation = (byte*)(&colorFrame1.data[0]);
		double difference = diff.exportDifference(commitLocation);
		diff.exportImage((byte*)(&inputMat0.data[0]), 0);
		diff.exportImage((byte*)(&inputMat1.data[0]), 1);

		//cout << "The total image difference is " << difference * 100 << "%\n";
		cout << difference * 100;

		//display results
		imshow("Diff Image", colorFrame1);
		imshow("Original Image 1", inputMat0);
		imshow("Original Image 2", inputMat1);

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
	test |= finalProject::driver(argc, argv);
	return test;
}