#pragma once
#include "debug.hpp"
#include "arithmeticalRGB.h"
#include "ByteIntensity.hpp"
#include "pixel.hpp"
#include "colorModelEnums.hpp"
#include "colorModelTypes.hpp"
#include <cstring>

namespace RGBPix {

	class arithmeticalRGB;

	//array indices
	const int numColors = 3;

	//array type
	struct RGB {
		BI_b m_b;
		BI_g m_g;
		BI_r m_r;
	};

	class pixelRGB : public pixel<pixelRGB> {
		private:
			RGB rgb;
		public:
			pixelRGB();															//empty constructor
			pixelRGB(byte r, byte g, byte b);									//explicit constructor
			pixelRGB(const pixelRGB& other);									//const constructor
			pixelRGB(const arithmeticalRGB& other);								//downcast constructor
			std::string toString() const;										//toString
			pixelRGB& operator=(const pixelRGB& data);							//assignment operator from rgb pixel
			pixelRGB& operator=(const arithmeticalRGB& other);					//downcast assignment
			arithmeticalRGB operator+(const pixelRGB& neighbor) const;			//addition operator for pixel primitive
			arithmeticalRGB operator-(const pixelRGB& neighbor) const;			//subtraction operator for pixel primitive
			arithmeticalRGB operator/(const pixelRGB& other) const;				//division operator for integer type
			arithmeticalRGB operator*(const pixelRGB& other) const;				//multiplication for pixel primitive
			arithmeticalRGB operator+(PrecisionType brightness) const;			//addition operator for integer format brightness
			arithmeticalRGB operator-(PrecisionType brightness) const;			//subtraction operator for pixel primitive
			arithmeticalRGB operator*(PrecisionType scalar) const;				//multiplication for integer scalar
			arithmeticalRGB operator/(PrecisionType denom) const;				//division operator for integer type
			arithmeticalRGB operator+(const arithmeticalRGB& brightness) const;	//addition operator for integer format brightness
			arithmeticalRGB operator-(const arithmeticalRGB& brightness) const;	//subtraction operator for pixel primitive
			arithmeticalRGB operator*(const arithmeticalRGB& scalar) const;		//multiplication for integer scalar
			arithmeticalRGB operator/(const arithmeticalRGB& denom) const;		//division operator for integer type
			arithmeticalRGB lux() const;										//luxing algorithm
			arithmeticalRGB unlux() const;										//unluxing algorithm
			arithmeticalRGB continuousLux(double p) const;						//continuous luxing algorithm
			arithmeticalRGB root() const;										//square root of pixel primitive
			bool operator==(const pixelRGB& m) const;							//equal comparison for pixel primitives
			bool operator<=(const pixelRGB& m) const;							//less than equal comparison for pixel primitives
			bool operator<(const pixelRGB& m) const;							//strict less than comparison for pixel primitives
			bool operator>=(const pixelRGB& m) const;							//greater than equal for pixel primitives
			bool operator>(const pixelRGB& m) const;							//strict greater than for pixel primitives
			pixelRGB toGrey() const;											//get pixel as greyscale
			pixelRGB coordHash(unsigned int x, unsigned int y) const;			//pixel color based on position
			pixelRGB histogramShift(unsigned int average) const;				//greyscale histogram shift
			pixelRGB toNegative() const;										//greyscale pixel transformation
			byte getAvgIntensity() const;										//get average brightness
			pixelRGB toBinary(pixelRGB thresh) const;							//get binary thresholded pixel
			byte R() const;
			byte G() const;
			byte B() const;

			//cast operators
			operator arithmeticalRGB() const;									//allow implicit conversion to arithmeticalRGB
			explicit operator HSIPix::arithmeticalHSI() const;					//only allow explicit conversion to other pixel types
			explicit operator GreyscalePix::arithmeticalGreyscale() const;
	};
};