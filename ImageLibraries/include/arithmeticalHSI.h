#pragma once
#include "debug.hpp"
#include "colorModelEnums.hpp"
#include "colorModelTypes.hpp"
#include "pixelHSI.h"
#include "ByteIntensity.hpp"
#include "pixel.hpp"

namespace HSIPix {

	class pixelHSI;

	struct HSIPrecise {
		AI_h m_h;
		AI_s m_s;
		AI_i m_i;
	};

	class arithmeticalHSI : pixel<arithmeticalHSI> {
		private:
			HSIPrecise hsi;
		public:
			arithmeticalHSI();													//empty constructor
			arithmeticalHSI(byte h, byte s, byte i);							//explicit constructor
			arithmeticalHSI(const pixelHSI& other);								//constructor
			arithmeticalHSI(const arithmeticalHSI& other);						//const constructor
			arithmeticalHSI& operator=(const pixelHSI& data);					//assignment operator from pixelHSI
			arithmeticalHSI& operator=(const arithmeticalHSI& data);			//assignment operator from hsi pixel
			pixelHSI toPixelHSI() const;										//operator to cast to pixelHSI
			std::string toString() const;										//return stringified object
			arithmeticalHSI operator+(const pixelHSI       & neighbor)   const;	//addition operator for pixel primitive
			arithmeticalHSI operator+(const arithmeticalHSI& neighbor)   const;	//addition operator for pixel primitive
			arithmeticalHSI operator+(const PrecisionType  & brightness) const;	//addition operator for PrecisionType format brightness
			arithmeticalHSI operator-(const pixelHSI       & neighbor)   const;	//addition operator for pixel primitive
			arithmeticalHSI operator-(const arithmeticalHSI& neighbor)   const;	//subtraction operator for pixel primitive
			arithmeticalHSI operator-(const PrecisionType  & brightness) const;	//subtraction operator for PrecisionType primitive
			arithmeticalHSI operator/(const pixelHSI       & neighbor)   const;	//addition operator for pixel primitive
			arithmeticalHSI operator/(const arithmeticalHSI& other)      const;	//division operator for integer type
			arithmeticalHSI operator/(const PrecisionType  & denom)      const;	//division operator for PrecisionType type
			arithmeticalHSI operator*(const pixelHSI       & neighbor)   const;	//addition operator for pixel primitive
			arithmeticalHSI operator*(const arithmeticalHSI& other)      const;	//multiplication for pixel primitive
			arithmeticalHSI operator*(const PrecisionType  & scalar)     const;	//multiplication for PrecisionType scalar
			arithmeticalHSI root() const;										//return root of this pixel
			PrecisionType H() const;
			PrecisionType S() const;
			PrecisionType I() const;
	};
};