#include "DCT.h"
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

inline const PrecisionType Block::alpha(unsigned int i) {
	//const PrecisionType l_w = 8;
	static PrecisionType Root = (1 / sqrt(2.0));//sqrt(l_w);
	static PrecisionType TwoRoot = 1;//2.0 * Root;
	if (0 == i) {
		return Root;
	}
	return TwoRoot;
}

const PrecisionType Block::cosN(unsigned int x, unsigned int u, unsigned int b) {
	const PrecisionType l_w = b;
	PrecisionType y = x;
	PrecisionType v = u;
	return cos((2.0 * y + 1) * (PI * v) / (2.0 * l_w));
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

inline const unsigned& DCTImage::BlockSize() {
	return Block::BlockSize();
}

DCTImage::DCTImage(const DCTImage& other) :
	Array2D<Block>(other.Height(), other.Width()),
	source_h(other.source_h),
	source_w(other.source_w) {
		for (unsigned int i = 0; i < other.Height(); i++) {
			for (unsigned int j = 0; j < other.Width(); j++) {
				((*this)[i][j]) = other[i][j];
			}
		}
}

DCTImage& DCTImage::operator=(const DCTImage& other) {
	source_h = other.source_h;
	source_w = other.source_w;
	if (NULL != array) {
		delete[] array;
		array = NULL;
	}
	h = other.Height();
	w = other.Width();
	array = new Block[Height() * Width()];
	for (unsigned int i = 0; i < Height(); i++) {
		for (unsigned int j = 0; j < Width(); j++) {
			array[i * Width() + j] = other.array[i * Width() + j];
		}
	}
	return *this;

}

DCTImage::DCTImage(unsigned int h, unsigned int w, unsigned int s_h, unsigned int s_w) :
	Array2D<Block>(h, w),
	source_h(s_h),
	source_w(s_w) {
}

DCTImage::DCTImage(const imageGrid<GreyscalePix::pixelGreyscale>& grid) :
	Array2D<Block>(ceil(grid.Height() / BlockSize()), ceil(grid.Width() / BlockSize())),
	source_h(grid.Height()),
	source_w(grid.Width()) {

		{
			Block tmp;
			//for each 8x8 block in image grid
			for (unsigned int i = 0; i < Height(); i++) {
				for (unsigned int j = 0; j < Width(); j++) {

					//create Array2D for block
					((*this)[i][j]) = tmp;
					Block& b = ((*this)[i][j]);
					//Copy intensities to 8x8 grid
					for (unsigned int y = 0; y < BlockSize(); y++) {
						for (unsigned int x = 0; x < BlockSize(); x++) {
							PrecisionType& entry = b[y][x];
							auto pixel = grid.getPixel(i * BlockSize() + x, j * BlockSize() + y).I();
							entry = pixel;
						}
					}
				}
			}
		}
}

DCTImage DCTImage::processOverImage(std::function<Block(const Block&)>& func) const {
	DCTImage retVal = (*this);
	for (unsigned int i = 0; i < Height(); i++) {
		for (unsigned int j = 0; j < Width(); j++) {
			retVal[i][j] = func(retVal[i][j]);
		}
	}
	return retVal;
}

inline PrecisionType DCTImage::toPercentage(const ImageQuality quality) const {
	static std::map<ImageQuality, PrecisionType> kvStore;
	static bool initialized = false;
	if (!initialized) {
		initialized = true;
		kvStore[LOW ] = ( 1.0 / (BlockSize() * BlockSize()));
		kvStore[MED ] = ( 9.0 / (BlockSize() * BlockSize()));
		kvStore[HIGH] = (16.0 / (BlockSize() * BlockSize()));
		kvStore[FULL] = 1.0;
	};
	return kvStore[quality];
}

DCTImage DCTImage::toDct(const ImageQuality quality) const {
	std::function<Block(const Block&)> selectedFilter = std::bind(&Block::topLeftPercentage, std::placeholders::_1, toPercentage(quality));
	std::function<Block(const Block&)> func = std::bind(&Block::ComputeDct, std::placeholders::_1, selectedFilter);
	return processOverImage(func);
}

DCTImage DCTImage::toInverseDct(const ImageQuality quality) const {
	std::function<Block(const Block&)> selectedFilter = std::bind(&Block::topLeftPercentage, std::placeholders::_1, toPercentage(quality));
	std::function<Block(const Block&)> func = std::bind(&Block::ComputeDctThenIdct, std::placeholders::_1, selectedFilter);
	return processOverImage(func);
}

imageGrid<GreyscalePix::pixelGreyscale> DCTImage::toImgGrid(const DCTImage& dctImg) const {
	imageGrid<GreyscalePix::pixelGreyscale> retVal(source_h, source_w);
	for (unsigned int i = 0; i < dctImg.Height(); i++) {
		for (unsigned int j = 0; j < dctImg.Width(); j++) {
			for (unsigned int y = 0; y < BlockSize(); y++) {
				for (unsigned int x = 0; x < BlockSize(); x++) {
					Block& currBlock = dctImg[i][j];
					PrecisionType i_value = currBlock[x][y];
					PrecisionType intensity = std::max<PrecisionType>(std::min<PrecisionType>(round(i_value), 255.0), 0.0);
					GreyscalePix::pixelGreyscale p_value = GreyscalePix::pixelGreyscale(static_cast<byte>(intensity));
					retVal.getPixel(i * BlockSize() + y, j * BlockSize() + x) = p_value;
				}
			}
		}
	}
	return retVal;
}

imageGrid<GreyscalePix::pixelGreyscale> DCTImage::ComputeDct(const ImageQuality quality) const {
	DCTImage result = toDct(quality);
	return toImgGrid(result);
}

imageGrid<GreyscalePix::pixelGreyscale> DCTImage::ComputeInverseDct(const ImageQuality quality) const {
	DCTImage result = toInverseDct(quality);
	return toImgGrid(result);
}

std::vector<PrecisionType> DCTImage::vectorDCT(const std::vector<PrecisionType>& input) {
	std::vector<PrecisionType> retVal = input;
	PrecisionType rat = sqrt(static_cast<PrecisionType>(BlockSize()) / input.size());
	for (byte u = 0; u < retVal.size(); u++) {
		retVal[u] = 0;
		for (byte x = 0; x < input.size(); x++) {
			retVal[u] += ((u > (input.size()*1.0)) ? 0 : (input[x] * Block::cosN(x, u, input.size())));
		}
		retVal[u] *= (0.5 * Block::alpha(u) * rat);
	}
	return retVal;
}

std::vector<PrecisionType> DCTImage::vectorIDCT(const std::vector<PrecisionType>& input) {
	std::vector<PrecisionType> retVal = input;
	PrecisionType rat = sqrt(static_cast<PrecisionType>(BlockSize()) / input.size());
	for (byte x = 0; x < retVal.size(); x++) {
		retVal[x] = 0;
		for (byte u = 0; u < input.size(); u++) {
			retVal[x] += (input[u] * Block::alpha(u) * Block::cosN(x, u, input.size()));
		}
		retVal[x] *= (0.5 * rat);
	}
	return retVal;
}