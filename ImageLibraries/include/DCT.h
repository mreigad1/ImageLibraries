#include "Array2D.hpp"
#include "colorModelEnums.hpp"
#include "pixelGreyscale.h"
#include "imageGrid.hpp"
#include <vector>
#include <functional>

class Block;

class DCTImage : public Array2D<Block> {
	public:
		static const unsigned& BlockSize();
		DCTImage(const DCTImage& other);
		DCTImage(unsigned int h, unsigned int w, unsigned int s_h, unsigned int s_w);
		DCTImage(const imageGrid<GreyscalePix::pixelGreyscale>& grid);
		DCTImage& operator=(const DCTImage& other);

		imageGrid<GreyscalePix::pixelGreyscale> ComputeDct(const ImageQuality quality = FULL) const;
		imageGrid<GreyscalePix::pixelGreyscale> ComputeInverseDct(const ImageQuality quality = FULL) const;

		static std::vector<PrecisionType>  vectorDCT(const std::vector<PrecisionType>& input);
		static std::vector<PrecisionType> vectorIDCT(const std::vector<PrecisionType>& input);

	private:
		unsigned int source_h;
		unsigned int source_w;

		DCTImage toDct(const ImageQuality quality = FULL) const;
		DCTImage toInverseDct(const ImageQuality quality = FULL) const;
		DCTImage processOverImage(std::function<Block(const Block&)>& func) const;

		imageGrid<GreyscalePix::pixelGreyscale> toImgGrid(const DCTImage& dctImg) const;
		PrecisionType toPercentage(const ImageQuality quality) const;
};