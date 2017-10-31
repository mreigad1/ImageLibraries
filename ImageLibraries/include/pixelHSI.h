#pragma once
#include "debug.hpp"
#include "arithmeticalHSI.h"
#include "ByteIntensity.hpp"
#include "pixel.hpp"
#include "colorModelEnums.hpp"
#include "colorModelTypes.hpp"
#include <cstring>

namespace HSIPix {

	//array indices
	const int numColors = 3;

	//array type
	struct HSI {
		BI_h m_h;
		BI_s m_s;
		BI_i m_i;
	};

	class pixelHSI : public pixel<pixelHSI> {
		private:
			HSI hsi;
		public:
			pixelHSI();															//empty constructor
			pixelHSI(byte h, byte s, byte i);									//explicit constructor
			pixelHSI(const pixelHSI& other);									//const constructor
			pixelHSI(const arithmeticalHSI& other);								//downcast constructor
			std::string toString() const;										//toString
			pixelHSI& operator=(const pixelHSI& data);							//assignment operator from hsi pixel
			pixelHSI& operator=(const arithmeticalHSI& other);					//downcast assignment
			arithmeticalHSI operator+(const pixelHSI& neighbor) const;			//addition operator for pixel primitive
			arithmeticalHSI operator-(const pixelHSI& neighbor) const;			//subtraction operator for pixel primitive
			arithmeticalHSI operator/(const pixelHSI& other) const;				//division operator for integer type
			arithmeticalHSI operator*(const pixelHSI& other) const;				//multiplication for pixel primitive
			arithmeticalHSI operator+(PrecisionType brightness) const;			//addition operator for integer format brightness
			arithmeticalHSI operator-(PrecisionType brightness) const;			//subtraction operator for pixel primitive
			arithmeticalHSI operator*(PrecisionType scalar) const;				//multiplication for integer scalar
			arithmeticalHSI operator/(PrecisionType denom) const;				//division operator for integer type
			arithmeticalHSI operator+(const arithmeticalHSI& brightness) const;	//addition operator for integer format brightness
			arithmeticalHSI operator-(const arithmeticalHSI& brightness) const;	//subtraction operator for pixel primitive
			arithmeticalHSI operator*(const arithmeticalHSI& scalar) const;		//multiplication for integer scalar
			arithmeticalHSI operator/(const arithmeticalHSI& denom) const;		//division operator for integer type
			arithmeticalHSI lux() const;										//luxing algorithm
			arithmeticalHSI unlux() const;										//unluxing algorithm
			arithmeticalHSI continuousLux(double p) const;						//continuous luxing algorithm
			arithmeticalHSI root() const;										//square root of pixel primitive
			bool operator<=(const pixelHSI& m) const;							//less than equal comparison for pixel primitives
			bool operator<(const pixelHSI& m) const;							//strict less than comparison for pixel primitives
			bool operator>=(const pixelHSI& m) const;							//greater than equal for pixel primitives
			bool operator>(const pixelHSI& m) const;							//strict greater than for pixel primitives
			pixelHSI toGrey() const;											//get pixel as greyscale
			pixelHSI coordHash(unsigned int x, unsigned int y) const;			//pixel color based on position
			pixelHSI histogramShift(unsigned int average) const;				//greyscale histogram shift
			pixelHSI toNegative() const;										//greyscale pixel transformation
			byte getAvgIntensity() const;										//get average brightness
			pixelHSI toBinary(pixelHSI thresh) const;							//get binary thresholded pixel
			byte H() const;
			byte S() const;
			byte I() const;
	};
};