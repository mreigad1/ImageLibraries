#include "Array2D.hpp"
#include "colorModelEnums.hpp"
#include "pixelGreyscale.h"
#include "imageGrid.hpp"

class Block : Array2D<PrecisionType> {
	private:
		static const unsigned blockSize = 8;
		const PrecisionType& cos8(unsigned int i, unsigned int j) const;
		const PrecisionType& alpha8(unsigned int i) const;
		bool isDCT;
	public:
		static const unsigned& BlockSize();
		Block();
		Block(const Block& other);
		Block& operator=(const Block& other);
		Block ComputeDct() const;
		Block ComputeInverseDct() const;
};

class DCTImage : Array2D<Block> {
	private:
		unsigned int source_h;
		unsigned int source_w;
	public:
		static const unsigned& BlockSize();
		DCTImage(const DCTImage& other);
		DCTImage(unsigned int h, unsigned int w, unsigned int s_h, unsigned int s_w);
		DCTImage(imageGrid<GreyscalePix::pixelGreyscale>& grid);
		DCTImage& operator=(const DCTImage& other);

		DCTImage toDct() const;
		DCTImage toInverseDct() const;

		imageGrid<GreyscalePix::pixelGreyscale> ComputeDct() const;
		imageGrid<GreyscalePix::pixelGreyscale> ComputeInverseDct() const;
};