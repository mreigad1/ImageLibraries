#include "debug.hpp"
#include "Array2D.hpp"

namespace Array2DTest {
	struct userType {
		int num;
		std::string word;

		userType() {
			num = 0;
			word = "";
		}

		userType& operator=(const userType& other) {
			num =  other.num;
			word = other.word;
			return *this;
		}

		userType& operator=(userType& other) {
			num =  other.num;
			word = other.word;
			return *this;
		}
	};

	void test(const unsigned int h1, const unsigned int w1) {

		char buffer[] = { 1, 2, 3, 4, 5, 6, 7, 8 };
		Array2D<int>		item1(h1, w1);
		Array2D<userType>	item2(h1, w1);
		Array2D<char>		item3(2, 4, buffer);

		for (unsigned int i = 0; i < w1; i++) {
			for (unsigned int j = 0; j < h1; j++) {
				item1[j][i] = j * w1 + i;
			}
		}

		for (unsigned int i = 0; i < w1; i++) {
			for (unsigned int j = 0; j < h1; j++) {
				item2[i][j].num = item1[j][i];
				item2[i][j].word = "foo";
			}
		}

		Array2D<userType>	item4(item2);
		Array2D<userType>	item5(item2);

		ASSERT(h1 == item4.Height());
		ASSERT(w1 == item4.Width());
		ASSERT(item4.Height() == item5.Height());
		ASSERT(item4.Width() == item5.Width());
		
		for (unsigned int i = 0; i < h1; i++) {
			for (unsigned int j = 0; j < w1; j++) {
				ASSERT(item4[i][j].num  == item5[i][j].num );
				ASSERT(item4[i][j].word == item5[i][j].word);
				item4[i][j].num += item5[i][j].num;
				item4[i][j].word = "bar";
			}
		}

		item2 = item4;

		for (unsigned int i = 0; i < h1; i++) {
			for (unsigned int j = 0; j < w1; j++) {
				ASSERT(item2[i][j].num  == item4[i][j].num );
				ASSERT(item2[i][j].word == "bar");
			}
		}
		
	}
};

int main() {
	int h[] = { 100, 791, 10104 };
	int w[] = { 20, 87, 3 };
	int th = (sizeof(h) / sizeof(int));
	int tw = (sizeof(w) / sizeof(int));

	std::cout << "Array2DTest[";
	for (int i = 0; i < th; i++) {
		for (int j = 0; j < tw; j++) {
			std::string s = ((i == (th-1) && j == (tw-1)) ? "" : "|");
			std::cout << h[i] << "," << w[j] << s;
			Array2DTest::test(h[i], w[j]);
		}
	}
	std::cout << "]\n";
}