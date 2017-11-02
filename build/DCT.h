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
		static inline const PrecisionType alpha(unsigned int i);
		static inline const PrecisionType cosN(unsigned int x, unsigned int u, unsigned int b);
};

class DCTImage : public Array2D<Block> {
	private:
		unsigned int source_h;
		unsigned int source_w;

		DCTImage toDct(const ImageQuality quality = FULL) const;
		DCTImage toInverseDct(const ImageQuality quality = FULL) const;
		DCTImage processOverImage(std::function<Block(const Block&)>& func) const;

		imageGrid<GreyscalePix::pixelGreyscale> toImgGrid(const DCTImage& dctImg) const;
		PrecisionType toPercentage(const ImageQuality quality) const;

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
};