#pragma once
#include "debug.hpp"
#include "colorModelEnums.hpp"
#include "colorModelTypes.hpp"
#include "pixelRGB.h"
#include "ByteIntensity.hpp"
#include "pixel.hpp"

namespace RGBPix {

	class pixelRGB;

	struct RGBPrecise {
		AI_b m_b;
		AI_g m_g;
		AI_r m_r;
	};

	class arithmeticalRGB : pixel<arithmeticalRGB> {
		private:
			RGBPrecise rgb;
		public:
			arithmeticalRGB();													//empty constructor
			arithmeticalRGB(byte r, byte g, byte b);							//explicit constructor
			arithmeticalRGB(const pixelRGB& other);								//constructor
			arithmeticalRGB(const arithmeticalRGB& other);						//const constructor
			arithmeticalRGB& operator=(const pixelRGB& data);					//assignment operator from pixelRGB
			arithmeticalRGB& operator=(const arithmeticalRGB& data);			//assignment operator from rgb pixel
			pixelRGB toPixelRGB() const;										//operator to cast to pixelRGB
			std::string toString() const;										//return stringified object
			arithmeticalRGB operator+(const pixelRGB       & neighbor)   const;	//addition operator for pixel primitive
			arithmeticalRGB operator+(const arithmeticalRGB& neighbor)   const;	//addition operator for pixel primitive
			arithmeticalRGB operator+(const PrecisionType  & brightness) const;	//addition operator for PrecisionType format brightness
			arithmeticalRGB operator-(const pixelRGB       & neighbor)   const;	//addition operator for pixel primitive
			arithmeticalRGB operator-(const arithmeticalRGB& neighbor)   const;	//subtraction operator for pixel primitive
			arithmeticalRGB operator-(const PrecisionType  & brightness) const;	//subtraction operator for PrecisionType primitive
			arithmeticalRGB operator/(const pixelRGB       & neighbor)   const;	//addition operator for pixel primitive
			arithmeticalRGB operator/(const arithmeticalRGB& other)      const;	//division operator for integer type
			arithmeticalRGB operator/(const PrecisionType  & denom)      const;	//division operator for PrecisionType type
			arithmeticalRGB operator*(const pixelRGB       & neighbor)   const;	//addition operator for pixel primitive
			arithmeticalRGB operator*(const arithmeticalRGB& other)      const;	//multiplication for pixel primitive
			arithmeticalRGB operator*(const PrecisionType  & scalar)     const;	//multiplication for PrecisionType scalar
			arithmeticalRGB root()const;										//return root of this pixel
			PrecisionType R() const;
			PrecisionType G() const;
			PrecisionType B() const;
	};
};