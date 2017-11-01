#include "DCT.h"
#include "constants.hpp"
#include <math.h>

const unsigned Block::blockSize = 8;

Block::Block() :
	Array2D<PrecisionType>(BlockSize(), BlockSize()),
	isDCT(false) {
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

const PrecisionType& Block::cos8(unsigned int x, unsigned int u) const {
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

const PrecisionType& Block::alpha8(unsigned int i) const {
	const PrecisionType l_w = 8;
	static PrecisionType Root = sqrt(l_w);
	static PrecisionType TwoRoot = 2.0 * Root;
	if (0 == i) {
		return Root;
	}
	return TwoRoot;
}

Block Block::ComputeDct() const {
	Block retVal = (*this);

	//It's not a DCT, calculate the DCT
	if (false == retVal.isDCT) { 
		retVal.isDCT = true;

		for (unsigned u = 0; u < BlockSize(); u++) {
			for (unsigned v = 0; v < BlockSize(); v++) {
				PrecisionType& s = retVal[u][v];
				s = 0;

				for (unsigned x = 0; x < BlockSize(); x++) {
					for (unsigned y = 0; y < BlockSize(); y++) {

						//s += array[u][v] * cos((2 * u + 1) * i * PI / 16) * cos((2 * v + 1) * j * PI / 16) * ((i == 0) ? 1 / sqrt(2) : 1) * ((j == 0) ? 1 / sqrt(2) : 1);
						s += (((*this)[x][y]) * cos8(x, u) * cos8(y, v));
					}
				}
				s = s * alpha8(u) * alpha8(v);
			}
		}
	}
	return retVal;
}

Block Block::ComputeInverseDct() const {
	Block retVal = (*this);

	//it is a DCT, calculate the inverse
	if (true == retVal.isDCT) { 
		retVal.isDCT = false;

		for (unsigned x = 0; x < BlockSize(); x++) {
			for (unsigned y = 0; y < BlockSize(); y++) {
				PrecisionType& s = retVal[x][y];
				s = 0;

				for (unsigned u = 0; u < BlockSize(); u++) {
					for (unsigned v = 0; v < BlockSize(); v++) {
						//s += array[u][v] * cos((2 * u + 1) * i * PI / 16) * cos((2 * v + 1) * j * PI / 16) * ((i == 0) ? 1 / sqrt(2) : 1) * ((j == 0) ? 1 / sqrt(2) : 1);
						s += (alpha8(u) * alpha8(v) * ((*this)[u][v]) * cos8(x, u) * cos8(y, v));
					}
				}
			}
		}
	}
	return retVal;
}

const unsigned& Block::BlockSize() {
	return blockSize;
}

const unsigned& DCTImage::BlockSize() {
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
	Array2D<Block>(),
	source_h(grid.Height()),
	source_w(grid.Width()) {
		LINE_LOG;
		unsigned int h_t = grid.Height();
		unsigned int w_t = grid.Width();
		while (h_t % BlockSize()) h_t++;
		while (w_t % BlockSize()) w_t++;
		h_t /= BlockSize();
		w_t /= BlockSize();
		const unsigned int h = h_t;
		const unsigned int w = w_t;

		LINE_LOG;
		//resize structure
		(*this) = DCTImage(h, w, h*BlockSize(), w*BlockSize());

		{
			Block tmp;
			LINE_LOG;
			//for each 8x8 block in image grid
			for (unsigned int i = 0; i < h; i++) {
				for (unsigned int j = 0; j < w; j++) {

					//create Array2D for block
					((*this)[i][j]) = tmp;
					LINE_LOG;
					//Copy intensities to 8x8 grid
					for (unsigned int y = 0; y < BlockSize(); y++) {
						for (unsigned int x = 0; x < BlockSize(); x++) {
							Block& b = ((*this)[i][j]);
							LINE_LOG;
							b[y][x] = grid.getPixel(i * BlockSize() + x, j * BlockSize() + y).I();
							LINE_LOG;
						}
					}
				}
			}
		}
}

DCTImage DCTImage::toDct() const {
	DCTImage retVal = (*this);
	for (unsigned int i = 0; i < h; i++) {
		for (unsigned int j = 0; j < w; j++) {
			((*this)[i][j]) = ((*this)[i][j]).ComputeDct();
		}
	}
	return retVal;
}

DCTImage DCTImage::toInverseDct() const {
	DCTImage retVal = (*this);
	for (unsigned int i = 0; i < h; i++) {
		for (unsigned int j = 0; j < w; j++) {
			((*this)[i][j]) = ((*this)[i][j]).ComputeInverseDct();
		}
	}
	return retVal;
}

imageGrid<GreyscalePix::pixelGreyscale> DCTImage::ComputeDct() const {
	imageGrid<GreyscalePix::pixelGreyscale> retVal(source_h, source_w);
	{
		DCTImage result = toDct();
		for (unsigned int i = 0; i < result.Height(); i++) {
			for (unsigned int j = 0; j < result.Width(); j++) {
				for (unsigned int x = 0; x < BlockSize(); x++) {
					for (unsigned int y = 0; y < BlockSize(); y++) {
						retVal.getPixel(i * BlockSize() + x, j * BlockSize() + y) = GreyscalePix::pixelGreyscale((result[i][j])[x][y]);
					}
				}
			}
		}
	}
	return retVal;
}

imageGrid<GreyscalePix::pixelGreyscale> DCTImage::ComputeInverseDct() const {
	imageGrid<GreyscalePix::pixelGreyscale> retVal(source_h, source_w);
	{
		DCTImage result = toInverseDct();
		for (unsigned int i = 0; i < result.Height(); i++) {
			for (unsigned int j = 0; j < result.Width(); j++) {
				for (unsigned int x = 0; x < BlockSize(); x++) {
					for (unsigned int y = 0; y < BlockSize(); y++) {
						retVal.getPixel(i * BlockSize() + x, j * BlockSize() + y) = GreyscalePix::pixelGreyscale((result[i][j])[x][y]);
					}
				}
			}
		}
	}
	return retVal;
}