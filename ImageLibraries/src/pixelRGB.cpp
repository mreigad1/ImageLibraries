#include "debug.hpp"
#include "constants.hpp"
#include "pixelRGB.h"

namespace RGBPix {
	//empty constructor
	pixelRGB::pixelRGB() :
		pixel<pixelRGB>() {
	}

	//explicit constructor
	pixelRGB::pixelRGB(byte r, byte g, byte b) :
		pixel<pixelRGB>() {
			rgb.m_r = r;
			rgb.m_g = g;
			rgb.m_b = b;
	}

	//const constructor
	pixelRGB::pixelRGB(const pixelRGB& other) :
		pixel<pixelRGB>() {
			rgb.m_r = other.rgb.m_r;
			rgb.m_g = other.rgb.m_g;
			rgb.m_b = other.rgb.m_b;
	}
	
	//downcast constructor
	pixelRGB::pixelRGB(const arithmeticalRGB& other) {
		(*this) = other.toPixelRGB();
	}

	//toString method
	std::string pixelRGB::toString() const {
		std::string retVal = "{"
		+ std::to_string((int)rgb.m_r.value()) + ", "
		+ std::to_string((int)rgb.m_g.value()) + ", "
		+ std::to_string((int)rgb.m_b.value()) + "}";
		return retVal;
	}

	//assignment operator from rgb pixel
	pixelRGB& pixelRGB::operator=(const pixelRGB& other) {
		rgb.m_r = other.rgb.m_r;
		rgb.m_g = other.rgb.m_g;
		rgb.m_b = other.rgb.m_b;
		return *this;
	}

	//downcast assignment
	pixelRGB& pixelRGB::operator=(const arithmeticalRGB& other) {
		(*this) = other.toPixelRGB();
		return *this;
	}

	//addition operator for pixel primitive
	arithmeticalRGB pixelRGB::operator+(const pixelRGB& neighbor) const {
		return arithmeticalRGB(*this) + arithmeticalRGB(neighbor);
	}
	
	//subtraction operator for pixel primitive
	arithmeticalRGB pixelRGB::operator-(const pixelRGB& neighbor) const {
		return arithmeticalRGB(*this) - arithmeticalRGB(neighbor);
	}

	//division operator for integer type
	arithmeticalRGB pixelRGB::operator/(const pixelRGB& other) const {
		ASSERT(0 != other.rgb.m_r.value());
		ASSERT(0 != other.rgb.m_g.value());
		ASSERT(0 != other.rgb.m_b.value());
		return arithmeticalRGB(*this) / arithmeticalRGB(other);
	}

	//multiplication for pixel primitive
	arithmeticalRGB pixelRGB::operator*(const pixelRGB& other) const {
		return arithmeticalRGB(*this) * arithmeticalRGB(other);
	}

	//addition operator for pixel primitive
	arithmeticalRGB pixelRGB::operator+(const arithmeticalRGB& neighbor) const {
		return arithmeticalRGB(*this) + neighbor;
	}
	
	//subtraction operator for pixel primitive
	arithmeticalRGB pixelRGB::operator-(const arithmeticalRGB& neighbor) const {
		return arithmeticalRGB(*this) - neighbor;
	}

	//division operator for integer type
	arithmeticalRGB pixelRGB::operator/(const arithmeticalRGB& other) const {
		ASSERT(0 != other.R());
		ASSERT(0 != other.G());
		ASSERT(0 != other.B());
		return arithmeticalRGB(*this) / other;
	}

	//multiplication for pixel primitive
	arithmeticalRGB pixelRGB::operator*(const arithmeticalRGB& other) const {
		return arithmeticalRGB(*this) * other;
	}

	//addition operator for integer format brightness
	arithmeticalRGB pixelRGB::operator+(PrecisionType brightness) const {
		return arithmeticalRGB(*this) + brightness;
	}

	//subtraction operator for pixel primitive
	arithmeticalRGB pixelRGB::operator-(PrecisionType brightness) const {
		return arithmeticalRGB(*this) - brightness;
	}

	//division operator for integer type
	arithmeticalRGB pixelRGB::operator/(PrecisionType denom) const {
		ASSERT(0 != denom);
		return arithmeticalRGB(*this) / denom;
	}

	//multiplication for integer scalar
	arithmeticalRGB pixelRGB::operator*(PrecisionType scalar) const {
		return arithmeticalRGB(*this) * scalar;
	}

	//less than equal comparison for pixel primitives
	bool pixelRGB::operator<=(const pixelRGB& m) const {
		return getAvgIntensity() <= m.getAvgIntensity();
	}

	//strict less than comparison for pixel primitives
	bool pixelRGB::operator<(const pixelRGB& m) const {
		return getAvgIntensity() < m.getAvgIntensity();
	}

	//greater than equal for pixel primitives
	bool pixelRGB::operator>=(const pixelRGB& m) const {
		return getAvgIntensity() >= m.getAvgIntensity();
	}

	//strict greater than for pixel primitives
	bool pixelRGB::operator>(const pixelRGB& m) const {
		return getAvgIntensity() > m.getAvgIntensity();
	}

	//luxing algorithm
	arithmeticalRGB pixelRGB::lux() const {
		TBI;
		arithmeticalRGB retVal;
		return retVal;
	}

	//unluxing algorithm
	arithmeticalRGB pixelRGB::unlux() const {
		TBI;
		arithmeticalRGB retVal;
		return retVal;
	}

	//continuous luxing algorithm
	arithmeticalRGB pixelRGB::continuousLux(double p) const {
		TBI;
		arithmeticalRGB retVal;
		return retVal;
	}

	//square root of pixel primitive
	arithmeticalRGB pixelRGB::root() const {
		arithmeticalRGB retVal(*this);
		return retVal.root();
	}

	//get pixel as greyscale
	pixelRGB pixelRGB::toGrey() const {
		byte greyLevel = getAvgIntensity();
		return pixelRGB(greyLevel, greyLevel, greyLevel);
	}

	//get average brightness
	byte pixelRGB::getAvgIntensity() const {
		int retVal = 0;
		retVal += rgb.m_r.value();
		retVal += rgb.m_g.value();
		retVal += rgb.m_b.value();
		retVal += 1; //this allows round up for /3
		retVal = retVal / 3;
		retVal = (retVal > 0xFF) ? 0xFF : retVal;
		retVal = (retVal < 0)    ? 0x00 : retVal;
		return (unsigned char)retVal;
	}

	//get binary thresholded pixel
	pixelRGB pixelRGB::toNegative() const {
		pixelRGB retVal(MAX_BYTE - rgb.m_r.value(), MAX_BYTE - rgb.m_g.value(), MAX_BYTE - rgb.m_b.value());
		return retVal;
	}

	//greyscale histogram shift
	pixelRGB pixelRGB::histogramShift(unsigned int average) const {
		byte retGreyVal = getAvgIntensity();
		float midPoint = MAX_BYTE / 2;
		const unsigned int trivialCases[] = { 0, (unsigned int)midPoint, MAX_BYTE };

		//loop handles error cases of 0, 255, and trivial case of already greyscale balanced
		for (unsigned int i = 0; i < sizeof(trivialCases)/sizeof(unsigned int); i++) {
			if (trivialCases[i] == average) {
				return pixelRGB(retGreyVal, retGreyVal, retGreyVal);
			}
		}

		float newValue = retGreyVal;
		if (1 > newValue) {
			newValue = 1;
		}

		//linear transform of number line for intensities
		if (newValue < average) {
			newValue = newValue * midPoint / average;
		} else if (newValue > average) {
			newValue = ((newValue - average) * (midPoint / (MAX_BYTE - average))) + midPoint;
		}
		retGreyVal = (byte)newValue;
		return pixelRGB(retGreyVal, retGreyVal, retGreyVal);
	}

	pixelRGB pixelRGB::coordHash(unsigned int x, unsigned int y) const {
		x = x & 0x0FFF;
		y = y & 0x0FFF;
		unsigned int buf = 0;
		const int spins = 3;
		for (int j = 0; j < spins; j++) {
			for (int i = 0; i < 12; i++) {
				buf = (buf | (x & 1)) << 1;
				buf = (buf | (y & 1)) << 1;
				x = x >> 1;
				y = y >> 1;
			}
			buf = buf >> 1;
			if (j != (spins - 1)) {
				x = buf & 0x0FFF;
				y = (buf >> 12) & 0x0FFF;
				buf = 0;
			}
		}
		byte r = buf & 0xFF;
		buf = buf >> 8;
		byte g = buf & 0xFF;
		buf = buf >> 8;
		byte b = buf & 0xFF;
		return pixelRGB(r, g, b);
	}

	//get binary thresholded pixel
	pixelRGB pixelRGB::toBinary(pixelRGB thresh) const {
		byte newVal = ((*this) > thresh) ? 0xFF : 0x00;
		return pixelRGB(newVal, newVal, newVal);
	}

	byte pixelRGB::R() const {
		return rgb.m_r.value();
	}

	byte pixelRGB::G() const {
		return rgb.m_g.value();
	}

	byte pixelRGB::B() const {
		return rgb.m_b.value();
	}

};