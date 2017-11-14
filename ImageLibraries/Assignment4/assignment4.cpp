#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "debug.hpp"
#include "pixelGreyscale.h"
#include "pixelHSI.h"
#include "pixelRGB.h"
#include "imageGrid.hpp"
#include "DCT.h"
#include "histogram.hpp"
#include <vector>
#include <iomanip>

using namespace std;
using namespace cv;

typedef GreyscalePix::pixelGreyscale GREY_P;
typedef HSIPix::pixelHSI HSI_P;
typedef RGBPix::pixelRGB RGB_P;
typedef histogramProcessor<RGB_P> histogram;

namespace assignment4 {

	int part(int argc, char **argv) {
		//ensure correct args
		ASSERT(4 == argc);
		const char* test_file = argv[1];

		std::cout << "\n\nBeginning part B2\n";
		std::cout << std::flush;

		//setup all displayable images
		Mat colorImage = imread(test_file,  CV_LOAD_IMAGE_COLOR);
		Mat originalImage = imread(test_file, CV_LOAD_IMAGE_GRAYSCALE);
		Mat lowDctProcessedImage = imread(test_file, CV_LOAD_IMAGE_GRAYSCALE);
		Mat lowIdctProcessedImage = imread(test_file, CV_LOAD_IMAGE_GRAYSCALE);
		Mat medDctProcessedImage = imread(test_file, CV_LOAD_IMAGE_GRAYSCALE);
		Mat medIdctProcessedImage = imread(test_file, CV_LOAD_IMAGE_GRAYSCALE);
		Mat fullDctProcessedImage = imread(test_file, CV_LOAD_IMAGE_GRAYSCALE);
		Mat fullIdctProcessedImage = imread(test_file, CV_LOAD_IMAGE_GRAYSCALE);

		//ensure all images are valid
		ASSERT(nullptr != originalImage.data);

		//Windowing text
		string windowText = string("Window from ") + __FILE__ + ":" + __FUNCTION__;

		//construct imageGrids
		imageGrid<RGB_P> colorGrid(colorImage.rows, colorImage.step / 3, (RGBPix::pixelRGB*)&colorImage.data[0]);
		imageGrid<HSI_P> hsiGrid(colorGrid);
		imageGrid<GREY_P> originalGrid(hsiGrid);

		//construct DCT's
		DCTImage dctProcessor(originalGrid);
		imageGrid<GREY_P>   lowDCT_G = dctProcessor.ComputeDct(LOW);
		imageGrid<GREY_P>   medDCT_G = dctProcessor.ComputeDct(MED);
		imageGrid<GREY_P>  fullDCT_G = dctProcessor.ComputeDct(FULL);
		imageGrid<GREY_P>  lowIDCT_G = dctProcessor.ComputeInverseDct(LOW);
		imageGrid<GREY_P>  medIDCT_G = dctProcessor.ComputeInverseDct(MED);
		imageGrid<GREY_P> fullIDCT_G = dctProcessor.ComputeInverseDct(FULL);

		//commit image changes
		originalGrid.commitImageGrid ((GREY_P*)&originalImage.data[0]);
		fullDCT_G.commitImageGrid    ((GREY_P*)&fullDctProcessedImage.data[0]);
		fullIDCT_G.commitImageGrid   ((GREY_P*)&fullIdctProcessedImage.data[0]);
		lowDCT_G.commitImageGrid     ((GREY_P*)&lowDctProcessedImage.data[0]);
		lowIDCT_G.commitImageGrid    ((GREY_P*)&lowIdctProcessedImage.data[0]);
		medDCT_G.commitImageGrid     ((GREY_P*)&medDctProcessedImage.data[0]);
		medIDCT_G.commitImageGrid    ((GREY_P*)&medIdctProcessedImage.data[0]);

		//display results
		imshow(                         "Color Image", colorImage);
		imshow(                 "Intensity Image (I)", originalImage);
		imshow(                 "Frequency Image (F)", fullDctProcessedImage);
		imshow(                     "Decoded Image F", fullIdctProcessedImage);
		imshow(             "DC Component Image (D1)", lowDctProcessedImage);
		imshow(        "9 Low Frequencies Image (D2)", medDctProcessedImage );
		imshow(     "DC Component Decoded Image (R1)", lowIdctProcessedImage);
		imshow("9 Low Frequencies Decoded Image (R2)", medIdctProcessedImage);

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

	int ROI_Driver(int argnum, int argc, char **argv) {
		ASSERT(4 == argc);
		const char* test_file = argv[argnum];
		Mat original_image  = imread(test_file, CV_LOAD_IMAGE_COLOR);
		Mat clustered_image = imread(test_file, CV_LOAD_IMAGE_COLOR);
		Mat buffer_img      = imread(test_file, CV_LOAD_IMAGE_COLOR);
		Mat image_part2     = imread(test_file, CV_LOAD_IMAGE_COLOR);
		Mat image_part3     = imread(test_file, CV_LOAD_IMAGE_COLOR);
		ASSERT(nullptr != buffer_img.data);

		//Create the display window
		string windowText = string("Window from ") + __FILE__ + ":" + __FUNCTION__;
		imageGrid<RGB_P> base_grid(buffer_img.rows, buffer_img.step / 3, (RGB_P*)&buffer_img.data[0]);
		imageGrid<RGB_P> unmodified_grid(base_grid);
		auto sb = base_grid.sobel();
		for (int i = 0; i < 2; i++) {
			base_grid = base_grid.multiplyByMask(UnsharpMask()) - sb;
		}
		imageGrid<HSI_P> test_grid(base_grid);

		{
			PrecisionType low = 25;
			PrecisionType high = 90;
			auto thresh = histogram(test_grid, low, high).lowPercentilePixel();
			test_grid = histogram(test_grid, low, high).histogramCorrection();
			test_grid = histogram(test_grid).histogramCorrection().toBinary(thresh);
			auto clusters = histogram(test_grid, 0, 100).histogramCorrection().clusterImage(thresh);
			test_grid = histogram(test_grid, 0, 100).histogramCorrection().assignment3Coloring(clusters, unmodified_grid);
			imageGrid<RGB_P>(test_grid).commitImageGrid((RGB_P*)&image_part2.data[0]);
		}

		//Display loop
		bool loop = true;
		while(loop) {
			imshow("Original image", original_image);
			imshow("PART 2 Image", image_part2);
			//imshow("Part 3 Image", clustered_image);
			//imshow("PART 4 Image", image_part3);

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
	//test |= assignment3::BlockTest();
	return test;
}