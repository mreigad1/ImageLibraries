#pragma once
#include "debug.hpp"
#include "colorModelEnums.hpp"
#include "colorModelTypes.hpp"
#include "ByteIntensity.hpp"
#include "pixel.hpp"

namespace RGBPix {

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
			arithmeticalRGB(const arithmeticalRGB& other);						//const constructor
			arithmeticalRGB& operator=(const arithmeticalRGB& data);			//assignment operator from rgb pixel
			std::string toString() const;										//return stringified object
			arithmeticalRGB operator+(const arithmeticalRGB& neighbor)   const;	//addition operator for pixel primitive
			arithmeticalRGB operator+(const PrecisionType  & brightness) const;	//addition operator for PrecisionType format brightness
			arithmeticalRGB operator-(const arithmeticalRGB& neighbor)   const;	//subtraction operator for pixel primitive
			arithmeticalRGB operator-(const PrecisionType  & brightness) const;	//subtraction operator for PrecisionType primitive
			arithmeticalRGB operator/(const arithmeticalRGB& other)      const;	//division operator for integer type
			arithmeticalRGB operator/(const PrecisionType  & denom)      const;	//division operator for PrecisionType type
			arithmeticalRGB operator*(const arithmeticalRGB& other)      const;	//multiplication for pixel primitive
			arithmeticalRGB operator*(const PrecisionType  & scalar)     const;	//multiplication for PrecisionType scalar
			arithmeticalRGB root()const;										//return root of this pixel
			PrecisionType R() const;
			PrecisionType G() const;
			PrecisionType B() const;

			PrecisionType dataComponent1() const;								//static polymorphic alias for normalized [0,1] R component
			PrecisionType dataComponent2() const;								//static polymorphic alias for normalized [0,1] G component
			PrecisionType dataComponent3() const;								//static polymorphic alias for normalized [0,1] B component
			static arithmeticalRGB denormalize(PrecisionType c1, PrecisionType c2, PrecisionType c3);
	};
};