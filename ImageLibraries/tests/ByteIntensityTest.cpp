#include "debug.hpp"
#include "colorModelEnums.hpp"
#include "ByteIntensity.hpp"

namespace ByteIntensityTest {

	void driver() {
		ByteIntensity<ind_r> item1;
		ByteIntensity<ind_g> item2(0x1F);
		ByteIntensity<ind_g> item3(item2);

		item2 = item3 - 300;
		item3 = item2 + 1000;
		ASSERT(0x00 == item2.value());
		ASSERT(0xFF == item3.value());

		item2 = 1000 * (item2 + 1);
		ASSERT(0xFF == item2.value());

		item2 = ((item2 + 100) * (item3 + 500)) / (item3 * item3);
		std::cout << "[" << static_cast<int>(item2.value()) << "]";
		ASSERT(0 != item2.value());
	}
};

int main() {
	std::cout << "ByteIntensityTest Start|";
	ByteIntensityTest::driver();
	std::cout << "ByteIntensityTest Finish.";
}