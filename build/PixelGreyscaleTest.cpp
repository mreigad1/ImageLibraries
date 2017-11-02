#include "debug.hpp"
#include "pixelGreyscale.h"
namespace PixelGreyscaleTest {
	int driver() {
		GreyscalePix::pixelGreyscale pix1(1);
		GreyscalePix::pixelGreyscale pix2(0);
		GreyscalePix::pixelGreyscale pix3(10);
		GreyscalePix::pixelGreyscale pix4(10);
		pix2 = pix2 + pix2;
		pix1 = pix1 + pix2;
		pix4 = pix1 / pix1;
		ASSERT(pix4 == pix1);
		pix4 = pix2 + 1;
		ASSERT(pix4 == pix1);
		pix4 = pix1 * 10;
		ASSERT(pix4 == pix3);
		return 0;
	}
};
int main() {
	int retVal = 0;
	std::cout << "Starting PixelGreyscaleTest|";
	retVal = PixelGreyscaleTest::driver();
	std::cout << "Finished PixelGreyscaleTest\n";
	return retVal;
}