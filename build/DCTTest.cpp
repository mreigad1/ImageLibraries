#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "debug.hpp"
#include "pixelGreyscale.h"
#include "pixelHSI.h"
#include "pixelRGB.h"
#include "imageGrid.hpp"
#include "DCT.h"
#include "histogram.h"
#include <vector>
#include <iomanip>

using namespace std;
using namespace cv;

namespace assignment3 {

	int BlockTest() {

		const PrecisionType testData[] = {
			16,  11,  10,  16,  24,  40,  51,  61,
			12,  12,  14,  19,  26,  58,  60,  55,
			14,  13,  16,  24,  40,  57,  69,  56,
			14,  17,  22,  29,  51,  87,  80,  62,
			18,  22,  37,  56,  68, 109, 103,  77,
			24,  35,  55,  64,  81, 104, 113,  92,
			49,  64,  78,  87, 103, 121, 120, 101,
			72,  92,  95,  98, 112, 100, 103,  99,
		};

		std::function<Block(const Block&)> func;
		Block testBlock(reinterpret_cast<const PrecisionType*>(testData));
		Block testDCT = testBlock.ComputeDct(func);
		Block testInverse = testDCT.ComputeInverseDct();
		Block testInverse2 = testBlock.ComputeDct(func).ComputeInverseDct();
		Block testInverse3 = testBlock.ComputeDctThenIdct(func);

		Block* arr[] = { &testBlock, &testDCT, &testInverse, &testInverse2, &testInverse3 };
		for (unsigned int i = 0; i < 5; i++) {
			std::cout << "Test [" << i << "] = " << arr[i]->toString();
		}

		return 0;
	}

	int partB1(int argc, char **argv) {

		PrecisionType inputs[2][8] = 
		{
			{  10,  11,  12,  11,  12,  13,  12,  11 },
			{  10, -10,   8,  -7,   8,  -8,   7,  -7 }
		};

		std::cout << "Matt Reigada\nCS 555\nAssignment 3\n\n";

		std::vector<PrecisionType> v1 (inputs[0], inputs[0] + sizeof(inputs[0]) / sizeof(decltype(inputs[0][0])));
		decltype(v1)               v2 (inputs[1], inputs[1] + sizeof(inputs[1]) / sizeof(decltype(inputs[0][0])));
		decltype(v1)               vB (v1);
		vB.insert(vB.end(), v2.begin(), v2.end());

		//part A
		{
			decltype(v1) a1 = DCTImage::vectorDCT(v1);
			decltype(v1) a2 = DCTImage::vectorDCT(v2);
			decltype(v1) i1 = DCTImage::vectorIDCT(a1);
			decltype(v1) i2 = DCTImage::vectorIDCT(a2);
			decltype(v1)* aPtr[] = { &v1,  &v2,  &a1,  &a2, &i1, &i2  };
			std::string names[]  = {
				"      first_row",
				"     second_row",
				"   DCT_Result_1",
				"   DCT_Result_2",
				"reversed_DCT_R1",
				"reversed_DCT_R2"
			};

			std::cout << "\nB1 Part a\n";
			for (byte i = 0; i < sizeof(aPtr) / sizeof(decltype(&v1)); i++) {
				std::cout << "\t" << std::string(names[i]) << "[] = { ";
				for (byte vp = 0; vp < v1.size(); vp++) {
					std::cout << setprecision(3) << setw(3) << setfill(' ') << (*(aPtr[i]))[vp] << ((vp < (v1.size() - 1)) ? ", " : "");
				}
				std::cout << "  };\n";
			}
		}

		//part B
		{
			decltype(v1) bb = DCTImage::vectorDCT(vB);
			decltype(v1) ib = DCTImage::vectorIDCT(bb);
			decltype(v1)* aPtr[] = { &vB,  &bb, &ib  };
			std::string  names[] = { 
				"  array_size_16",
				"    DCT_Size_16",
				"   reversed_DCT"
			};

			std::cout << "\n\nB1 Part b\n";
			for (byte i = 0; i < sizeof(aPtr) / sizeof(decltype(&v1)); i++) {
				std::cout << "\t" << std::string(names[i]) << "[] = { ";
				for (byte vp = 0; vp < vB.size(); vp++) {
					std::cout  << setprecision(3) << setw(3) << setfill(' ')  << (*(aPtr[i]))[vp] << ((vp < (vB.size() - 1)) ? ", " : "");
				}
				std::cout << "  };\n";
			}
		}

		//TODO: part C
		{
			std::cout << "\n\nB1 Part c\n";
			std::string q[] = {
				#include "../ImageLibraries/Assignment3/part3C.txt"
			};

			for (byte i = 0; i < sizeof(q) / sizeof(decltype(q[0])); i++) {
				std::cout << q[i];
			}
		}

		std::cout << std::flush;

		return 0;
	}

	int partB2(int argc, char **argv) {
		//ensure correct args
		ASSERT(2 == argc);
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
		imageGrid<RGBPix::pixelRGB> colorGrid(colorImage.rows, colorImage.step / 3, (RGBPix::pixelRGB*)&colorImage.data[0]);
		imageGrid<HSIPix::pixelHSI> hsiGrid(colorGrid);
		imageGrid<GreyscalePix::pixelGreyscale> originalGrid(hsiGrid);

		//construct DCT's
		DCTImage dctProcessor(originalGrid);
		imageGrid<GreyscalePix::pixelGreyscale>   lowDCT_G = dctProcessor.ComputeDct(LOW);
		imageGrid<GreyscalePix::pixelGreyscale>   medDCT_G = dctProcessor.ComputeDct(MED);
		imageGrid<GreyscalePix::pixelGreyscale>  fullDCT_G = dctProcessor.ComputeDct(FULL);
		imageGrid<GreyscalePix::pixelGreyscale>  lowIDCT_G = dctProcessor.ComputeInverseDct(LOW);
		imageGrid<GreyscalePix::pixelGreyscale>  medIDCT_G = dctProcessor.ComputeInverseDct(MED);
		imageGrid<GreyscalePix::pixelGreyscale> fullIDCT_G = dctProcessor.ComputeInverseDct(FULL);

		//commit image changes
		originalGrid.commitImageGrid((GreyscalePix::pixelGreyscale*)&originalImage.data[0]);
		fullDCT_G.commitImageGrid((GreyscalePix::pixelGreyscale*)&fullDctProcessedImage.data[0]);
		fullIDCT_G.commitImageGrid((GreyscalePix::pixelGreyscale*)&fullIdctProcessedImage.data[0]);
		lowDCT_G.commitImageGrid((GreyscalePix::pixelGreyscale*)&lowDctProcessedImage.data[0]);
		lowIDCT_G.commitImageGrid((GreyscalePix::pixelGreyscale*)&lowIdctProcessedImage.data[0]);
		medDCT_G.commitImageGrid((GreyscalePix::pixelGreyscale*)&medDctProcessedImage.data[0]);
		medIDCT_G.commitImageGrid((GreyscalePix::pixelGreyscale*)&medIdctProcessedImage.data[0]);

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

	int ROI_Driver(int argc, char **argv) {
		ASSERT(2 == argc);
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
		imageGrid<RGB_P> stashed_grid(test_grid);
		test_grid.toGrey();
		//test_grid.darkMedianFilter();
		//imageGrid<RGB_P> filter_grid = test_grid;

		std::cout << "\nHistogram of Image"; 
		histogram disp_histogram(test_grid);
		disp_histogram.display();
		const byte thresh = disp_histogram.getMinima();
		
		test_grid.toBinary(RGB_P(thresh, thresh, thresh));
		test_grid.commitImageGrid((RGB_P*)&image_part2.data[0]);

		auto clusters = test_grid.clusterImage(RGB_P(thresh,thresh,thresh));
		test_grid.commitImageGrid((RGB_P*)&clustered_image.data[0]);

		test_grid.assignment3Coloring(clusters, stashed_grid);
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
};

int main (int argc, char **argv) {
	bool test = 0;
	//test |= assignment3::BlockTest();
	test |= assignment3::partB1(argc, argv);
	//test |= assignment3::partB2(argc, argv);
	test |= assignment3::ROI_Driver(argc, argv);
	return test;
}