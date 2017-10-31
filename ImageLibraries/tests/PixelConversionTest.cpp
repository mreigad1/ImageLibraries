#include "debug.hpp"
#include "pixelRGB.h"
#include "pixelHSI.h"
#include "arithmeticalRGB.h"
#include "arithmeticalHSI.h"
#include <iostream>
namespace PixelRGBTest {
	template <typename T> T abs(T val) {
		return ((val < 0) ? (val * -1) : val);
	}

	int driver() {
		{//test RGB -> HSI -> RGB
			const int numTuples = 3;
			PrecisionType testNums[numTuples][3] = {
				{ 000, 000, 000 },
				{  10,  90, 200 },
				{ 255, 255, 255 },
			};

			for (int i = 0; i < numTuples; i++) {
				RGBPix::pixelRGB pix1(testNums[i][0], testNums[i][1], testNums[i][2]);
				HSIPix::pixelHSI pix2 = static_cast<HSIPix::arithmeticalHSI>(pix1);
				RGBPix::pixelRGB pix3 = static_cast<RGBPix::arithmeticalRGB>(pix2);
				RGBPix::pixelRGB pix4 = static_cast<RGBPix::arithmeticalRGB>(pix1);
				/*
				std::cout << "\tRGB->HSI #1 = " << pix1.toString() << std::endl;
				std::cout << "\tRGB->HSI #3 = " << pix3.toString() << std::endl;
				std::cout << "\tRGB->HSI #4 = " << pix4.toString() << std::endl;
				std::cout << std::endl;
				*/
				ASSERT(pix1 == pix3);
				ASSERT(pix1 == pix4);
				ASSERT(pix4 == pix3);
			}
		}

		{//test HSI -> RGB -> HSI
			const int numTuples = 3;
			PrecisionType testNums[numTuples][4] = {
				{     0.0,  0.0,      0.0, 0.00001 },
				{  3.7561,  0.9, 0.392157,    0.01 },
				{     0.0,  0.0,      1.0, 0.00001 },
			};

			for (int i = 0; i < numTuples; i++) {
				HSIPix::pixelHSI pix1(testNums[i][0], testNums[i][1], testNums[i][2]);
				RGBPix::pixelRGB pix2 = static_cast<RGBPix::arithmeticalRGB>(pix1);
				HSIPix::pixelHSI pix3 = static_cast<HSIPix::arithmeticalHSI>(pix2);
				HSIPix::pixelHSI pix4 = static_cast<HSIPix::arithmeticalHSI>(pix1);
				/*
				std::cout << "\tHSI->RGB #1 = " << pix1.toString() << std::endl;
				std::cout << "\tHSI->RGB #3 = " << pix3.toString() << std::endl;
				std::cout << "\tHSI->RGB #4 = " << pix4.toString() << std::endl;
				std::cout << std::endl;
				*/

				//ensure within acceptable round error of test row
				ASSERT(abs(pix1.H() - pix3.H()) < testNums[i][3]);
				ASSERT(abs(pix1.S() - pix3.S()) < testNums[i][3]);
				ASSERT(abs(pix1.I() - pix3.I()) < testNums[i][3]);
				ASSERT(abs(pix1.H() - pix4.H()) < testNums[i][3]);
				ASSERT(abs(pix1.S() - pix4.S()) < testNums[i][3]);
				ASSERT(abs(pix1.I() - pix4.I()) < testNums[i][3]);
				ASSERT(abs(pix3.H() - pix4.H()) < testNums[i][3]);
				ASSERT(abs(pix3.S() - pix4.S()) < testNums[i][3]);
				ASSERT(abs(pix3.I() - pix4.I()) < testNums[i][3]);
			}
		}

		return 0;
	}
};
int main() {
	int retVal = 0;
	std::cout << "Starting PixelConversionTest|\n";
	retVal = PixelRGBTest::driver();
	std::cout << "Finished PixelConversionTest\n";
	return retVal;
}