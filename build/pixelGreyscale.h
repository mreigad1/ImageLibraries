#pragma once
#include "debug.hpp"
#include "arithmeticalGreyscale.h"
#include "ByteIntensity.hpp"
#include "pixel.hpp"
#include "colorModelEnums.hpp"
#include "colorModelTypes.hpp"
#include <cstring>

namespace GreyscalePix {

	class arithmeticalGreyscale;

	class pixelGreyscale : public pixel<pixelGreyscale> {
		private:
			BI_grey m_i;
		public:
			pixelGreyscale();																//empty constructor
			pixelGreyscale(byte intensity);													//explicit constructor
			pixelGreyscale(byte r, byte g, byte b);											//explicit constructor for average intensity
			pixelGreyscale(const pixelGreyscale& other);									//const constructor
			pixelGreyscale(const arithmeticalGreyscale& other);								//downcast constructor
			std::string toString() const;													//toString
			pixelGreyscale& operator=(const pixelGreyscale& data);							//assignment operator from rgb pixel
			pixelGreyscale& operator=(const arithmeticalGreyscale& other);					//downcast assignment
			arithmeticalGreyscale operator+(const pixelGreyscale& neighbor) const;			//addition operator for pixel primitive
			arithmeticalGreyscale operator-(const pixelGreyscale& neighbor) const;			//subtraction operator for pixel primitive
			arithmeticalGreyscale operator/(const pixelGreyscale& other) const;				//division operator for integer type
			arithmeticalGreyscale operator*(const pixelGreyscale& other) const;				//multiplication for pixel primitive
			arithmeticalGreyscale operator+(PrecisionType brightness) const;				//addition operator for integer format brightness
			arithmeticalGreyscale operator-(PrecisionType brightness) const;				//subtraction operator for pixel primitive
			arithmeticalGreyscale operator*(PrecisionType scalar) const;					//multiplication for integer scalar
			arithmeticalGreyscale operator/(PrecisionType denom) const;						//division operator for integer type
			arithmeticalGreyscale operator+(const arithmeticalGreyscale& brightness) const;	//addition operator for integer format brightness
			arithmeticalGreyscale operator-(const arithmeticalGreyscale& brightness) const;	//subtraction operator for pixel primitive
			arithmeticalGreyscale operator*(const arithmeticalGreyscale& scalar) const;		//multiplication for integer scalar
			arithmeticalGreyscale operator/(const arithmeticalGreyscale& denom) const;		//division operator for integer type
			arithmeticalGreyscale lux() const;												//luxing algorithm
			arithmeticalGreyscale unlux() const;											//unluxing algorithm
			arithmeticalGreyscale continuousLux(double p) const;							//continuous luxing algorithm
			arithmeticalGreyscale root() const;												//square root of pixel primitive
			bool operator==(const pixelGreyscale& m) const;									//equal comparison for pixel primitives
			bool operator<=(const pixelGreyscale& m) const;									//less than equal comparison for pixel primitives
			bool operator<(const pixelGreyscale& m) const;									//strict less than comparison for pixel primitives
			bool operator>=(const pixelGreyscale& m) const;									//greater than equal for pixel primitives
			bool operator>(const pixelGreyscale& m) const;									//strict greater than for pixel primitives
			pixelGreyscale toGrey() const;													//get pixel as greyscale
			pixelGreyscale coordHash(unsigned int x, unsigned int y) const;					//pixel color based on position
			pixelGreyscale histogramShift(unsigned int average) const;						//greyscale histogram shift
			pixelGreyscale toNegative() const;												//greyscale pixel transformation
			byte getAvgIntensity() const;													//get average brightness
			pixelGreyscale toBinary(pixelGreyscale thresh) const;							//get binary thresholded pixel
			byte I() const;																	//get intensity

			PrecisionType dataComponent1() const;											//static polymorphic alias for I [0.0, 1.0]
			PrecisionType dataComponent2() const;											//static polymorphic alias for I [0.0, 1.0]
			PrecisionType dataComponent3() const;											//static polymorphic alias for I [0.0, 1.0]
			static pixelGreyscale denormalize(PrecisionType c1, PrecisionType c2, PrecisionType c3);

			//cast operators
			arithmeticalGreyscale Arithmetical() const;
			operator arithmeticalGreyscale() const;											//allow implicit conversion to arithmeticalGreyscale
			explicit operator RGBPix::arithmeticalRGB() const;								//only allow explicit conversion to other pixel types
			explicit operator HSIPix::arithmeticalHSI() const;
	};
};