#include "debug.hpp"
#include "constants.hpp"
#include "pixelHSI.h"
#include "arithmeticalRGB.h"
#include "arithmeticalHSI.h"
#include "arithmeticalGreyscale.h"
#include <math.h>

namespace HSIPix {
	//empty constructor
	pixelHSI::pixelHSI() :
		pixel<pixelHSI>() {
	}

	pixelHSI::pixelHSI(byte intensity) {
		hsi.m_h = 0;
		hsi.m_s = 0;
		hsi.m_i = intensity;
	}

	//explicit constructor
	pixelHSI::pixelHSI(PrecisionType h, PrecisionType s, PrecisionType i) :
		pixel<pixelHSI>() {
			hsi.m_h = h;
			hsi.m_s = s;
			hsi.m_i = i;
	}

	//const constructor
	pixelHSI::pixelHSI(const pixelHSI& other) :
		pixel<pixelHSI>() {
			ASSERT(other.H() >= 0);
			ASSERT(other.H() <= tPI);
			hsi.m_h = other.hsi.m_h;
			hsi.m_s = other.hsi.m_s;
			hsi.m_i = other.hsi.m_i;
	}
	
	//downcast constructor
	pixelHSI::pixelHSI(const arithmeticalHSI& other) {
		hsi.m_h = other.H();
		hsi.m_s = other.S();
		hsi.m_i = other.I();
		while(H() < 0)   hsi.m_h = hsi.m_h + tPI;
		while(H() > tPI) hsi.m_h = hsi.m_h - tPI;
		if (S() < 0)     hsi.m_s = 0;
		if (S() > 1)     hsi.m_s = 1;
		if (I() < 0)     hsi.m_i = 0;
		if (I() > 1)     hsi.m_i = 1;
	}

	//toString method
	std::string pixelHSI::toString() const {
		std::string retVal = "{"
		+ std::to_string(hsi.m_h.value()) + ", "
		+ std::to_string(hsi.m_s.value()) + ", "
		+ std::to_string(hsi.m_i.value()) + "}";
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
		hsi.m_h = other.H();
		hsi.m_s = other.S();
		hsi.m_i = other.I();
		while(H() < 0)   hsi.m_h = hsi.m_h + tPI;
		while(H() > tPI) hsi.m_h = hsi.m_h - tPI;
		if (S() < 0)     hsi.m_s = 0;
		if (S() > 1)     hsi.m_s = 1;
		if (I() < 0)     hsi.m_i = 0;
		if (I() > 1)     hsi.m_i = 1;
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

	//greater than equal for pixel primitives
	bool pixelHSI::operator==(const pixelHSI& m) const {
		bool rv = true;
		rv = rv && (H() == m.H());
		rv = rv && (S() == m.S());
		rv = rv && (I() == m.I());
		return rv;
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
		return pixelHSI(0, 0, I());
	}

	//get average brightness
	PrecisionType pixelHSI::getAvgIntensity() const {
		return I();
	}

	//get binary thresholded pixel
	pixelHSI pixelHSI::toNegative() const {
		return pixelHSI((tPI - hsi.m_h.value()), (1.0 - hsi.m_s.value()), (1.0 - hsi.m_i.value()));
	}

	//greyscale histogram shift
	pixelHSI pixelHSI::histogramShift(unsigned int average) const {
		TBI;
		return (*this);
	}

	pixelHSI pixelHSI::coordHash(unsigned int x, unsigned int y) const {
		TBI;
		return (*this);
	}

	//get binary thresholded pixel
	pixelHSI pixelHSI::toBinary(pixelHSI thresh) const {
		byte newVal = ((*this) > thresh) ? 0xFF : 0x00;
		return pixelHSI(newVal, newVal, newVal);
	}

	PrecisionType pixelHSI::H() const {
		return hsi.m_h.value();
	}

	PrecisionType pixelHSI::S() const {
		return hsi.m_s.value();
	}

	PrecisionType pixelHSI::I() const {
		return hsi.m_i.value();
	}

	PrecisionType pixelHSI::dataComponent1() const {
		return static_cast<PrecisionType>((H() / tPI));
	}

	PrecisionType pixelHSI::dataComponent2() const {
		return static_cast<PrecisionType>(S());
	}
	
	PrecisionType pixelHSI::dataComponent3() const {
		return static_cast<PrecisionType>(I());
	}

	pixelHSI pixelHSI::denormalized(PrecisionType c1, PrecisionType c2, PrecisionType c3) {
		return pixelHSI(c1 * tPI, c2, c3);
	}

	arithmeticalHSI pixelHSI::Arithmetical() const {
		return static_cast<arithmeticalHSI>(*this);
	}

	pixelHSI::operator HSIPix::arithmeticalHSI() const {
		return HSIPix::arithmeticalHSI(H(), S(), I());
	}

	template<typename T>
	T min(T x, T y) {
		return (x < y ? x : y);
	}

	template<typename T>
	T max(T x, T y) {
		return (x > y ? x : y);
	}

	pixelHSI::operator RGBPix::arithmeticalRGB() const {
		PrecisionType r, g, b;
		PrecisionType h = H();
		PrecisionType s = S();
		PrecisionType i = I();

		s = max(s, static_cast<PrecisionType>(0.0));
		i = max(i, static_cast<PrecisionType>(0.0));
		s = min(s, static_cast<PrecisionType>(1.0));
		i = min(i, static_cast<PrecisionType>(1.0));

		ASSERT(h >= 0);
		ASSERT(h <= tPI);
		int caseNum = (h / tPI) * 3;

		h -= (caseNum * (tPI / 3));
		PrecisionType x, y, z, divisor;

		PrecisionType c = i * MAX_BYTE;
		x = c * (1 - s);
		divisor = cos((PI / 3) - h) != 0 ? cos((PI / 3) - h) : 1;
		ASSERT(!isnan(divisor));
		y = c * (1 + (s * cos(h) / divisor));
		z = c * 3 - (x + y);

		switch (caseNum) {
			case 0:
			case 3:	//allowed to be 3 if precisely 2 PI
				b = x;
				r = y;
				g = z;
			break;
			case 1:
				r = x;
				g = y;
				b = z;
			break;
			case 2:
				g = x;
				b = y;
				r = z;
			break;
			default:
				std::cout << caseNum << std::endl;
				ASSERT("Error converting HSI to RGB" == nullptr);
			break;
		}

		return RGBPix::arithmeticalRGB(r, g, b);
	}

	pixelHSI::operator GreyscalePix::arithmeticalGreyscale() const {
		return GreyscalePix::arithmeticalGreyscale(I() * MAX_BYTE);
	}
};