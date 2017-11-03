#include "histogram.hpp"
#include "pixelRGB.h"

#ifdef TESTING
	#define DRIVER main
#else
	#define DRIVER histogram_driver
#endif

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace std;
using namespace cv;

typedef RGBPix::pixelRGB RGB_P;
typedef histogramProcessor<RGB_P> histogram;

int clusteringDriver1(int argc, char **argv) {
	ASSERT(3 == argc);
	const char* test_file = argv[1];
	Mat original_image  = imread(test_file, CV_LOAD_IMAGE_COLOR);
	Mat clustered_image = imread(test_file, CV_LOAD_IMAGE_COLOR);
	Mat buffer_img      = imread(test_file, CV_LOAD_IMAGE_COLOR);
	Mat image_part2     = imread(test_file, CV_LOAD_IMAGE_COLOR);
	Mat image_part3     = imread(test_file, CV_LOAD_IMAGE_COLOR);
	ASSERT(nullptr != buffer_img.data);

	//Create the display window
	string windowText = string("Window from ") + __FILE__ + ":" + __FUNCTION__;
	//namedWindow(windowText.c_str());

	imageGrid<RGB_P> test_grid(buffer_img.rows, buffer_img.step / 3, (RGB_P*)&buffer_img.data[0]);
	test_grid.toGrey();
	//test_grid.darkMedianFilter();
	//imageGrid<RGB_P> filter_grid = test_grid;

	std::cout << "\nHistogram of Image"; 
	histogram disp_histogram(test_grid);
	disp_histogram.display();
	const byte thresh = 35;
	std::cout << "\n(PART 1) Optimal threshold of ~" << (int)thresh << " hardcoded, though determined from Histogram\n";
	
	test_grid.toBinary(RGB_P(thresh, thresh, thresh));
	test_grid.commitImageGrid((RGB_P*)&image_part2.data[0]);

	auto clusters = test_grid.clusterImage(RGB_P(thresh,thresh,thresh));
	test_grid.commitImageGrid((RGB_P*)&clustered_image.data[0]);

	test_grid.assignment1Coloring(clusters);
	test_grid.commitImageGrid((RGB_P*)&image_part3.data[0]);

	//Display loop
	bool loop = true;
	while(loop) {
		imshow("Original image", original_image);
		imshow("PART 2 Image", image_part2);
		imshow("Clustered Groups Image", clustered_image);
		imshow("PART 3 Image", image_part3);

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

int clusteringDriver2(int argc, char **argv) {
	ASSERT(3 == argc);
	const char* test_file = argv[2];
	Mat original_image  = imread(test_file, CV_LOAD_IMAGE_COLOR);
	Mat clustered_image = imread(test_file, CV_LOAD_IMAGE_COLOR);
	Mat buffer_img      = imread(test_file, CV_LOAD_IMAGE_COLOR);
	Mat image_part2     = imread(test_file, CV_LOAD_IMAGE_COLOR);
	Mat image_part3     = imread(test_file, CV_LOAD_IMAGE_COLOR);
	ASSERT(nullptr != buffer_img.data);

	//Create the display window
	string windowText = string("Window from ") + __FILE__ + ":" + __FUNCTION__;
	//namedWindow(windowText.c_str());

	imageGrid<RGB_P> test_grid(buffer_img.rows, buffer_img.step / 3, (RGB_P*)&buffer_img.data[0]);
	test_grid.toGrey();
	//test_grid.darkMedianFilter();
	//imageGrid<RGB_P> filter_grid = test_grid;

	std::cout << "\nHistogram of Image"; 
	histogram disp_histogram(test_grid);
	disp_histogram.display();
	const byte thresh = 50;
	std::cout << "\n(PART 1) Optimal threshold of ~" << (int)thresh << " hardcoded, though determined from Histogram\n";
	
	test_grid.toBinary(RGB_P(thresh, thresh, thresh));
	test_grid.commitImageGrid((RGB_P*)&image_part2.data[0]);

	auto clusters = test_grid.clusterImage(RGB_P(thresh,thresh,thresh));
	test_grid.commitImageGrid((RGB_P*)&clustered_image.data[0]);

	test_grid.assignment1Coloring(clusters);
	test_grid.commitImageGrid((RGB_P*)&image_part3.data[0]);

	//Display loop
	bool loop = true;
	while(loop) {
		imshow("Original image", original_image);
		imshow("PART 2 Image", image_part2);
		imshow("Clustered Groups Image", clustered_image);
		imshow("PART 3 Image", image_part3);

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
	return (clusteringDriver1(argc, argv) | clusteringDriver2(argc, argv));
}