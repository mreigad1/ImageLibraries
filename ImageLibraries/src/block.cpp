#include "block.h"
#include "constants.hpp"
#include <math.h>
#include <sstream>
#include <algorithm>
#include <functional>
#include <map>

const unsigned Block::blockSize = 8;

Block::Block() :
	Array2D<PrecisionType>(BlockSize(), BlockSize()),
	isDCT(false) {
		for (unsigned int i = 0; i < Height(); i++) {
			for (unsigned int j = 0; j < Width(); j++) {
				array[i * Width() + j] = 0;
			}
		}
}

Block::Block(const Block& other) :
	Array2D<PrecisionType>(other),
	isDCT(other.isDCT) {
		for (unsigned int i = 0; i < Height(); i++) {
			for (unsigned int j = 0; j < Width(); j++) {
				array[i * Width() + j] = other.array[i * Width() + j];
			}
		}
}

Block::Block(const PrecisionType* data) :
	Array2D<PrecisionType> (blockSize, blockSize, data) {
}

std::string Block::toString() const {
	std::stringstream ss;
	ss << "{";
	for (unsigned int i = 0; i < BlockSize(); i++) {
		ss << "\n\t";
		for (unsigned int j = 0; j < BlockSize(); j++) {
			ss << ((*this)[i][j]) << ", ";
		}
	}
	ss << "\n};\n";
	return ss.str();
}

Block& Block::operator=(const Block& other) {
	if (NULL != array) {
		delete[] array;
		array = NULL;
	}
	h = other.Height();
	w = other.Width();
	isDCT = other.isDCT;
	array = new PrecisionType[Height() * Width()];
	for (unsigned int i = 0; i < Height(); i++) {
		for (unsigned int j = 0; j < Width(); j++) {
			array[i * Width() + j] = other.array[i * Width() + j];
		}
	}
	return *this;
}

Block Block::topLeftPercentage(PrecisionType p) const {
	Block rv;
	static const unsigned int GridSize = BlockSize() * BlockSize();
	const unsigned int targetNum = floor(p * GridSize);
	const unsigned int subWidth = ceil(sqrt(p * GridSize));
	unsigned int accum = 0;

	for (unsigned int i = 0; i < subWidth; i++) {
		for (unsigned int j = 0; j < subWidth; j++) {
			rv[i][j] = ((*this)[i][j]);
			accum++;
			if (accum >= targetNum) {
				i = j = subWidth;
				break;
			}
		}
	}
	return rv;
}

inline const PrecisionType Block::cos8Cached(unsigned int x, unsigned int u) const {
	const unsigned int l_w = 8;
	static PrecisionType lookUp[l_w][l_w];
	static bool initialized = false;
	if (!initialized) {
		for (unsigned int y = 0; y < l_w; y++) {
			for (unsigned int v = 0; v < l_w; v++) {
				lookUp[y][v] = cos((2.0 * y + 1) * (PI * v) / (2.0 * l_w));
			}
		}
		initialized = true;
	}
	return lookUp[x][u];
}

Block Block::ComputeDct(std::function<Block(const Block&)>& func) const {
	Block retVal = (*this);

	//It's not a DCT, calculate the DCT
	//if (false == retVal.isDCT) { 
	//	retVal.isDCT = true;

		for (unsigned u = 0; u < BlockSize(); u++) {
			for (unsigned v = 0; v < BlockSize(); v++) {
				PrecisionType& F_uv = retVal[v][u];
				PrecisionType sum = 0;

				for (unsigned x = 0; x < BlockSize(); x++) {
					for (unsigned y = 0; y < BlockSize(); y++) {
						//s += array[u][v] * cos((2 * u + 1) * i * PI / 16) * cos((2 * v + 1) * j * PI / 16) * ((i == 0) ? 1 / sqrt(2) : 1) * ((j == 0) ? 1 / sqrt(2) : 1);

						PrecisionType& f_xy = (*this)[y][x];
						sum += (f_xy * cos8Cached(x, u) * cos8Cached(y, v));
					}
				}
				F_uv = round( (0.25) * alpha(u) * alpha(v) * sum );
			}
		}
	//}
	if (func) {
		retVal = func(retVal);
	}
	return retVal;
}

Block Block::ComputeInverseDct() const {
	Block retVal = (*this);

	//it is a DCT, calculate the inverse
	//if (true == retVal.isDCT) { 
	//	retVal.isDCT = false;

		for (unsigned x = 0; x < BlockSize(); x++) {
			for (unsigned y = 0; y < BlockSize(); y++) {
				PrecisionType& f_xy = retVal[y][x];
				PrecisionType sum = 0;

				for (unsigned u = 0; u < BlockSize(); u++) {
					for (unsigned v = 0; v < BlockSize(); v++) {
						//s += array[u][v] * cos((2 * u + 1) * i * PI / 16) * cos((2 * v + 1) * j * PI / 16) * ((i == 0) ? 1 / sqrt(2) : 1) * ((j == 0) ? 1 / sqrt(2) : 1);

						PrecisionType& F_uv = (*this)[v][u];
						sum += (alpha(u) * alpha(v) * F_uv * cos8Cached(x, u) * cos8Cached(y, v));
					}
				}

				f_xy = round( (0.25) * sum );
			}
		}
	//}
	return retVal;
}

Block Block::ComputeDctThenIdct(std::function<Block(const Block&)>& func) const {
	Block rv = ComputeDct(func).ComputeInverseDct();
	return rv;
}

inline const unsigned& Block::BlockSize() {
	return blockSize;
}