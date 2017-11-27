#include "Array2D.hpp"
#include "colorModelEnums.hpp"
#include "pixelGreyscale.h"
#include "imageGrid.hpp"
#include <vector>
#include <functional>

class Block : public Array2D<PrecisionType> {
	private:
		static const unsigned blockSize;
		const PrecisionType cos8Cached(unsigned int i, unsigned int j) const;
		bool isDCT;
	public:
		static const unsigned& BlockSize();
		Block();
		Block(const Block& other);
		Block(const PrecisionType* data);
		Block& operator=(const Block& other);

		Block ComputeDct(std::function<Block(const Block&)>& func) const;
		Block ComputeInverseDct() const;
		Block ComputeDctThenIdct(std::function<Block(const Block&)>& func) const;

		Block topLeftPercentage(PrecisionType p) const;
		std::string toString() const;

		static inline const PrecisionType alpha(unsigned int i) {
			//const PrecisionType l_w = 8;
			static PrecisionType Root = (1 / sqrt(2.0));//sqrt(l_w);
			static PrecisionType TwoRoot = 1;//2.0 * Root;
			if (0 == i) {
				return Root;
			}
			return TwoRoot;
		}
		
		static inline const PrecisionType cosN(unsigned int x, unsigned int u, unsigned int b) {
			const PrecisionType l_w = b;
			PrecisionType y = x;
			PrecisionType v = u;
			return cos((2.0 * y + 1) * (PI * v) / (2.0 * l_w));
		}
};