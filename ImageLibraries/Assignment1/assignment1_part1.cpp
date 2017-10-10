#include "histogram.h"

#ifdef TESTING
	#define DRIVER main
#else
	#define DRIVER histogram_driver
#endif

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace std;
using namespace cv;

int colorInversion(int argc, char **argv) {
	ASSERT(3 == argc);
	const char* test_file = argv[1];
	Mat test_image     = imread(test_file, CV_LOAD_IMAGE_COLOR);
	Mat original_image = imread(test_file, CV_LOAD_IMAGE_COLOR);
	Mat *image = &test_image;
	ASSERT(nullptr != test_image.data);

	//Create the display window
	string windowText = string("Window from ") + __FILE__ + ":" + __FUNCTION__;
	//namedWindow(windowText.c_str());

	imageGrid<RGB_P> test_grid(test_image.rows, test_image.step / 3, (RGB_P*)&test_image.data[0]);
	test_grid.toGrey();
	//test_grid.darkMedianFilter();
	//imageGrid<RGB_P> filter_grid = test_grid;

	std::cout << "\nHistogram of Original"; 
	histogram disp_histogram(test_grid);
	disp_histogram.display();

	test_grid.toNegative();

	std::cout << "\nHistogram after taking negative";
	histogram disp2_histogram(test_grid);
	disp2_histogram.display();

	test_grid.commitImageGrid((RGB_P*)&test_image.data[0]);
	//filter_grid.commitImageGrid((RGB_P*)&original_image.data[0]);

	//Display loop
	imshow("Original Image", original_image);
	imshow("Negative Image", *image);
	return 0;
}

//This driver will test and implement assignment 1 histogram feature
int histogramCorrection (int argc, char **argv) {
	ASSERT(3 == argc);
	const char* test_file = argv[2];
	Mat test_image     = imread(test_file, CV_LOAD_IMAGE_COLOR);
	Mat original_image = imread(test_file, CV_LOAD_IMAGE_COLOR);
	Mat *image = &test_image;
	ASSERT(nullptr != test_image.data);

	//Create the display window
	string windowText = string("Window from ") + __FILE__ + ":" + __FUNCTION__;
	//namedWindow(windowText.c_str());

	imageGrid<RGB_P> test_grid(test_image.rows, test_image.step / 3, (RGB_P*)&test_image.data[0]);
	test_grid.toGrey();
	//test_grid.darkMedianFilter();
	//imageGrid<RGB_P> filter_grid = test_grid;

	std::cout << "\nHistogram of Original"; 
	histogram disp_histogram(test_grid);
	disp_histogram.display();

	for (int i = 0; i++ < 2;) {
		histogram processing_histogram(test_grid);
		processing_histogram.apply();
		test_grid.darkMedianFilter();
	}

	std::cout << "\nHistogram after balancing";
	histogram disp2_histogram(test_grid);
	disp2_histogram.display();

	test_grid.commitImageGrid((RGB_P*)&test_image.data[0]);
	//filter_grid.commitImageGrid((RGB_P*)&original_image.data[0]);

	//Display loop
	bool loop = true;
	while(loop) {
		imshow("Histogram Corrected Image", *image);
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

int main(int argc, char **argv) {
	return (colorInversion(argc, argv) | histogramCorrection(argc, argv));
}