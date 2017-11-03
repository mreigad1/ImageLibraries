#include "debug.hpp"
#include "constants.hpp"
#include "pixelRGB.h"
#include "arithmeticalRGB.h"
#include "arithmeticalHSI.h"
#include "arithmeticalGreyscale.h"
#include <iomanip>
#include <math.h>
#include <algorithm>

namespace RGBPix {
	//empty constructor
	pixelRGB::pixelRGB() :
		pixel<pixelRGB>() {
			byte intensity = 0;
			rgb.m_r = intensity;
			rgb.m_g = intensity;
			rgb.m_b = intensity;
	}

	//explicit constructor
	pixelRGB::pixelRGB(byte intensity) :
		pixel<pixelRGB>() {
			rgb.m_r = intensity;
			rgb.m_g = intensity;
			rgb.m_b = intensity;
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
		rgb.m_r = other.R();
		rgb.m_g = other.G();
		rgb.m_b = other.B();
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
		rgb.m_r = other.R();
		rgb.m_g = other.G();
		rgb.m_b = other.B();
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

	//greater than equal for pixel primitives
	bool pixelRGB::operator==(const pixelRGB& m) const {
		bool rv = true;
		rv = rv && (rgb.m_r == m.rgb.m_r);
		rv = rv && (rgb.m_g == m.rgb.m_g);
		rv = rv && (rgb.m_b == m.rgb.m_b);
		return rv;
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

	PrecisionType pixelRGB::dataComponent1() const {
		return static_cast<PrecisionType>(R() / 255.0);
	}

	PrecisionType pixelRGB::dataComponent2() const {
		return static_cast<PrecisionType>(G() / 255.0);
	}
	
	PrecisionType pixelRGB::dataComponent3() const {
		return static_cast<PrecisionType>(B() / 255.0);
	}

	arithmeticalRGB pixelRGB::Arithmetical() const {
		return static_cast<RGBPix::arithmeticalRGB>(*this);
	}

	pixelRGB pixelRGB::denormalize(PrecisionType c1, PrecisionType c2, PrecisionType c3) {
		return pixelRGB(c1 * 255, c2 * 255, c3 * 255);
	}

	pixelRGB::operator RGBPix::arithmeticalRGB() const {
		return RGBPix::arithmeticalRGB(R(), G(), B());
	}

	pixelRGB::operator HSIPix::arithmeticalHSI() const {
		PrecisionType H, S, I;

		PrecisionType rn     = 0;
		PrecisionType gn     = 0;
		PrecisionType bn     = 0;
		{
			const PrecisionType r = static_cast<PrecisionType>(rgb.m_r.value());	//intervals [0, 1]
			const PrecisionType g = static_cast<PrecisionType>(rgb.m_g.value());
			const PrecisionType b = static_cast<PrecisionType>(rgb.m_b.value());

			PrecisionType rgbSum = ((r + g + b) > 0 ? (r + g + b) : 0.0001);
			rn     = r / rgbSum;
			gn     = g / rgbSum;
			bn     = b / rgbSum;
			I = ( r + g + b ) / (3.0 * MAX_BYTE);
		}

		S = 1 - 3.0 * std::min(rn, std::min(gn, bn));
		PrecisionType trigInput = 0.5 * (2 * rn - (gn + bn));
		PrecisionType divisor = sqrt((rn - gn) * (rn - gn) + (rn - bn) * (gn - bn));

		if (divisor != 0) {
			trigInput /= divisor;
		} else {
			trigInput = 1;
		}
		if (false == (trigInput >= -1 && trigInput <= 1)) {
			std::cout << trigInput << std::endl << std::flush;
		}
		ASSERT(trigInput >= -1 && trigInput <= 1);
		H = acos(trigInput);

		ASSERT(S >= 0 && S <= 1);
		ASSERT(I >= 0 && I <= 1);
		if (H != H)                         H = 0       ; //handle NaN case
		if (bn > gn)                        H = tPI - H ; //Reflect if b > g
		if (H < 0 && H > -PI_ERROR)         H = 0       ;
		if (H > tPI && H - tPI < PI_ERROR)  H = tPI     ;
		while (H < 0)                       H += tPI    ;
		while (H > tPI)                     H -= tPI    ;
		if (H < 0   &&      H >  -PI_ERROR) H = 0       ;
		if (H > tPI && H - tPI <  PI_ERROR) H = tPI     ;

		ASSERT(H >= 0);
		ASSERT(H <= tPI);

		return HSIPix::arithmeticalHSI(H, S, I);
	}

	pixelRGB::operator GreyscalePix::arithmeticalGreyscale() const {
		return GreyscalePix::arithmeticalGreyscale(getAvgIntensity());
	}
};