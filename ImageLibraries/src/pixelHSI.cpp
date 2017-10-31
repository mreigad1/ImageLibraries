#include "debug.hpp"
#include "constants.hpp"
#include "pixelHSI.h"

namespace HSIPix {
	//empty constructor
	pixelHSI::pixelHSI() :
		pixel<pixelHSI>() {
	}

	//explicit constructor
	pixelHSI::pixelHSI(byte h, byte s, byte i) :
		pixel<pixelHSI>() {
			hsi.m_h = h;
			hsi.m_s = s;
			hsi.m_i = i;
	}

	//const constructor
	pixelHSI::pixelHSI(const pixelHSI& other) :
		pixel<pixelHSI>() {
			hsi.m_h = other.hsi.m_h;
			hsi.m_s = other.hsi.m_s;
			hsi.m_i = other.hsi.m_i;
	}
	
	//downcast constructor
	pixelHSI::pixelHSI(const arithmeticalHSI& other) {
		(*this) = other.toPixelHSI();
	}

	//toString method
	std::string pixelHSI::toString() const {
		std::string retVal = "{"
		+ std::to_string((int)hsi.m_h.value()) + ", "
		+ std::to_string((int)hsi.m_s.value()) + ", "
		+ std::to_string((int)hsi.m_i.value()) + "}";
		return retVal;
	}

	//assignment operator from hsi pixel
	pixelHSI& pixelHSI::operator=(const pixelHSI& other) {
		hsi.m_h = other.hsi.m_h;
		hsi.m_s = other.hsi.m_s;
		hsi.m_i = other.hsi.m_i;
		return *this;
	}

	//downcast assignment
	pixelHSI& pixelHSI::operator=(const arithmeticalHSI& other) {
		(*this) = other.toPixelHSI();
		return *this;
	}

	//addition operator for pixel primitive
	arithmeticalHSI pixelHSI::operator+(const pixelHSI& neighbor) const {
		return arithmeticalHSI(*this) + arithmeticalHSI(neighbor);
	}
	
	//subtraction operator for pixel primitive
	arithmeticalHSI pixelHSI::operator-(const pixelHSI& neighbor) const {
		return arithmeticalHSI(*this) - arithmeticalHSI(neighbor);
	}

	//division operator for integer type
	arithmeticalHSI pixelHSI::operator/(const pixelHSI& other) const {
		ASSERT(0 != other.hsi.m_h.value());
		ASSERT(0 != other.hsi.m_s.value());
		ASSERT(0 != other.hsi.m_i.value());
		return arithmeticalHSI(*this) / arithmeticalHSI(other);
	}

	//multiplication for pixel primitive
	arithmeticalHSI pixelHSI::operator*(const pixelHSI& other) const {
		return arithmeticalHSI(*this) * arithmeticalHSI(other);
	}

	//addition operator for pixel primitive
	arithmeticalHSI pixelHSI::operator+(const arithmeticalHSI& neighbor) const {
		return arithmeticalHSI(*this) + neighbor;
	}
	
	//subtraction operator for pixel primitive
	arithmeticalHSI pixelHSI::operator-(const arithmeticalHSI& neighbor) const {
		return arithmeticalHSI(*this) - neighbor;
	}

	//division operator for integer type
	arithmeticalHSI pixelHSI::operator/(const arithmeticalHSI& other) const {
		ASSERT(0 != other.H());
		ASSERT(0 != other.S());
		ASSERT(0 != other.I());
		return arithmeticalHSI(*this) / other;
	}

	//multiplication for pixel primitive
	arithmeticalHSI pixelHSI::operator*(const arithmeticalHSI& other) const {
		return arithmeticalHSI(*this) * other;
	}

	//addition operator for integer format brightness
	arithmeticalHSI pixelHSI::operator+(PrecisionType brightness) const {
		return arithmeticalHSI(*this) + brightness;
	}

	//subtraction operator for pixel primitive
	arithmeticalHSI pixelHSI::operator-(PrecisionType brightness) const {
		return arithmeticalHSI(*this) - brightness;
	}

	//division operator for integer type
	arithmeticalHSI pixelHSI::operator/(PrecisionType denom) const {
		ASSERT(0 != denom);
		return arithmeticalHSI(*this) / denom;
	}

	//multiplication for integer scalar
	arithmeticalHSI pixelHSI::operator*(PrecisionType scalar) const {
		return arithmeticalHSI(*this) * scalar;
	}

	//less than equal comparison for pixel primitives
	bool pixelHSI::operator<=(const pixelHSI& m) const {
		return getAvgIntensity() <= m.getAvgIntensity();
	}

	//strict less than comparison for pixel primitives
	bool pixelHSI::operator<(const pixelHSI& m) const {
		return getAvgIntensity() < m.getAvgIntensity();
	}

	//greater than equal for pixel primitives
	bool pixelHSI::operator>=(const pixelHSI& m) const {
		return getAvgIntensity() >= m.getAvgIntensity();
	}

	//strict greater than for pixel primitives
	bool pixelHSI::operator>(const pixelHSI& m) const {
		return getAvgIntensity() > m.getAvgIntensity();
	}

	//luxing algorithm
	arithmeticalHSI pixelHSI::lux() const {
		TBI;
		arithmeticalHSI retVal;
		return retVal;
	}

	//unluxing algorithm
	arithmeticalHSI pixelHSI::unlux() const {
		TBI;
		arithmeticalHSI retVal;
		return retVal;
	}

	//continuous luxing algorithm
	arithmeticalHSI pixelHSI::continuousLux(double p) const {
		TBI;
		arithmeticalHSI retVal;
		return retVal;
	}

	//square root of pixel primitive
	arithmeticalHSI pixelHSI::root() const {
		arithmeticalHSI retVal(*this);
		return retVal.root();
	}

	//get pixel as greyscale
	pixelHSI pixelHSI::toGrey() const {
		byte greyLevel = getAvgIntensity();
		return pixelHSI(greyLevel, greyLevel, greyLevel);
	}

	//get average brightness
	byte pixelHSI::getAvgIntensity() const {
		int retVal = 0;
		retVal += hsi.m_h.value();
		retVal += hsi.m_s.value();
		retVal += hsi.m_i.value();
		retVal += 1; //this allows round up for /3
		retVal = retVal / 3;
		retVal = (retVal > 0xFF) ? 0xFF : retVal;
		retVal = (retVal < 0)    ? 0x00 : retVal;
		return (unsigned char)retVal;
	}

	//get binary thresholded pixel
	pixelHSI pixelHSI::toNegative() const {
		pixelHSI retVal(MAX_BYTE - hsi.m_h.value(), MAX_BYTE - hsi.m_s.value(), MAX_BYTE - hsi.m_i.value());
		return retVal;
	}

	//greyscale histogram shift
	pixelHSI pixelHSI::histogramShift(unsigned int average) const {
		byte retGreyVal = getAvgIntensity();
		float midPoint = MAX_BYTE / 2;
		const unsigned int trivialCases[] = { 0, (unsigned int)midPoint, MAX_BYTE };

		//loop handles error cases of 0, 255, and trivial case of already greyscale balanced
		for (unsigned int index = 0; index < sizeof(trivialCases)/sizeof(unsigned int); index++) {
			if (trivialCases[index] == average) {
				return pixelHSI(retGreyVal, retGreyVal, retGreyVal);
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
		return pixelHSI(retGreyVal, retGreyVal, retGreyVal);
	}

	pixelHSI pixelHSI::coordHash(unsigned int x, unsigned int y) const {
		x = x & 0x0FFF;
		y = y & 0x0FFF;
		unsigned int buf = 0;
		const int spins = 3;
		for (int index_j = 0; index_j < spins; index_j++) {
			for (int index = 0; index < 12; index++) {
				buf = (buf | (x & 1)) << 1;
				buf = (buf | (y & 1)) << 1;
				x = x >> 1;
				y = y >> 1;
			}
			buf = buf >> 1;
			if (index_j != (spins - 1)) {
				x = buf & 0x0FFF;
				y = (buf >> 12) & 0x0FFF;
				buf = 0;
			}
		}
		byte h = buf & 0xFF;
		buf = buf >> 8;
		byte s = buf & 0xFF;
		buf = buf >> 8;
		byte i = buf & 0xFF;
		return pixelHSI(h, s, i);
	}

	//get binary thresholded pixel
	pixelHSI pixelHSI::toBinary(pixelHSI thresh) const {
		byte newVal = ((*this) > thresh) ? 0xFF : 0x00;
		return pixelHSI(newVal, newVal, newVal);
	}

	byte pixelHSI::H() const {
		return hsi.m_h.value();
	}

	byte pixelHSI::S() const {
		return hsi.m_s.value();
	}

	byte pixelHSI::I() const {
		return hsi.m_i.value();
	}
};