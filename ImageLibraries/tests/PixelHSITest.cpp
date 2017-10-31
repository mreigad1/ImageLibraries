#include "debug.hpp"
#include "pixelHSI.h"
namespace PixelHSITest {
	int driver() {
		HSIPix::pixelHSI pix1;
		HSIPix::pixelHSI pix2(0x10, 0xFF, 0x00);
		HSIPix::pixelHSI pix3(pix2);
		HSIPix::pixelHSI pix4(0x20, 0x20, 0x20);
		pix1 = pix3;

		ASSERT(pix2 <= pix2);
		ASSERT(pix2 >= pix2);
		ASSERT(false == (pix2 < pix2));
		ASSERT(false == (pix2 > pix2));

		ASSERT(pix3 <= pix2 && pix3 >= pix2);
		pix3 + pix3;
		pix3 = pix3;
		pix3 = pix3 + pix3;
		pix2 = pix2 + pix1;

		ASSERT(pix3 <= pix2 && pix3 >= pix2);
		pix3 = pix3 - 0x20;
		pix2 = pix2 - pix4;

		ASSERT(pix3 <= pix2 && pix3 >= pix2);
		pix2 = pix2 / 0x02;
		pix4 = pix4 / 0x10;
		pix3 = pix3 / pix4.getAvgIntensity();

		ASSERT(pix3 <= pix2);
		ASSERT(pix3 >= pix2);
		ASSERT(pix3 == pix2);
		pix2 = pix2 * (0x02 * 0x03);

		pix3 = pix3 * (pix4 * 0x03);
		ASSERT(pix3 <= pix2 && pix3 >= pix2);
		return 0;
	}
};
int main() {
	int retVal = 0;
	std::cout << "Starting PixelHSITest|";
	retVal = PixelHSITest::driver();
	std::cout << "Finished PixelHSITest\n";
	return retVal;
}