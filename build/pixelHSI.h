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
	//HSI innately uses float types
	struct HSI {
		AI_h m_h;
		AI_s m_s;
		AI_i m_i;
	};

	class pixelHSI : public pixel<pixelHSI> {
		private:
			HSI hsi;
		public:
			pixelHSI();															//empty constructor
			pixelHSI(PrecisionType h, PrecisionType s, PrecisionType i);									//explicit constructor
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
			bool operator==(const pixelHSI& m) const;							//equal comparison for pixel primitives
			bool operator<=(const pixelHSI& m) const;							//less than equal comparison for pixel primitives
			bool operator<(const pixelHSI& m) const;							//strict less than comparison for pixel primitives
			bool operator>=(const pixelHSI& m) const;							//greater than equal for pixel primitives
			bool operator>(const pixelHSI& m) const;							//strict greater than for pixel primitives
			pixelHSI toGrey() const;											//get pixel as greyscale
			pixelHSI coordHash(unsigned int x, unsigned int y) const;			//pixel color based on position
			pixelHSI histogramShift(unsigned int average) const;				//greyscale histogram shift
			pixelHSI toNegative() const;										//greyscale pixel transformation
			PrecisionType getAvgIntensity() const;								//get average brightness
			pixelHSI toBinary(pixelHSI thresh) const;							//get binary thresholded pixel
			PrecisionType H() const;
			PrecisionType S() const;
			PrecisionType I() const;

			//cast operators
			arithmeticalHSI Arithmetical() const;
			operator arithmeticalHSI() const;									//allow implicit conversion to arithmeticalHSI
			explicit operator RGBPix::arithmeticalRGB() const;					//only allow explicit conversion to other pixel types
			explicit operator GreyscalePix::arithmeticalGreyscale() const;
	};
};