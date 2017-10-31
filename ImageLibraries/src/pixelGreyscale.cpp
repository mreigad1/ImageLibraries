#include "debug.hpp"
#include "constants.hpp"
#include "pixelGreyscale.h"
#include "arithmeticalRGB.h"
#include "arithmeticalHSI.h"
#include "arithmeticalGreyscale.h"
#include <math.h>

namespace GreyscalePix {
	//empty constructor
	pixelGreyscale::pixelGreyscale() :
		pixel<pixelGreyscale>() {
	}

	//explicit constructor
	pixelGreyscale::pixelGreyscale(byte intensity) :
		pixel<pixelGreyscale>(),
		m_i(intensity) {
	}

	//const constructor
	pixelGreyscale::pixelGreyscale(const pixelGreyscale& other) :
		pixel<pixelGreyscale>(),
		m_i(other.m_i) {
	}
	
	//downcast constructor
	pixelGreyscale::pixelGreyscale(const arithmeticalGreyscale& other) :
		pixel<pixelGreyscale>(),
		m_i(static_cast<byte>(other.I())) {
	}

	//toString method
	std::string pixelGreyscale::toString() const {
		std::string retVal = "{"
		+ std::to_string((int)m_i.value()) + "}";
		return retVal;
	}

	//assignment operator from rgb pixel
	pixelGreyscale& pixelGreyscale::operator=(const pixelGreyscale& other) {
		m_i = other.m_i;
		return *this;
	}

	//downcast assignment
	pixelGreyscale& pixelGreyscale::operator=(const arithmeticalGreyscale& other) {
		m_i = static_cast<byte>(other.I());
		return *this;
	}

	//addition operator for pixel primitive
	arithmeticalGreyscale pixelGreyscale::operator+(const pixelGreyscale& neighbor) const {
		return arithmeticalGreyscale(*this) + arithmeticalGreyscale(neighbor);
	}
	
	//subtraction operator for pixel primitive
	arithmeticalGreyscale pixelGreyscale::operator-(const pixelGreyscale& neighbor) const {
		return arithmeticalGreyscale(*this) - arithmeticalGreyscale(neighbor);
	}

	//division operator for integer type
	arithmeticalGreyscale pixelGreyscale::operator/(const pixelGreyscale& other) const {
		ASSERT(0 != other.I());
		return arithmeticalGreyscale(*this) / arithmeticalGreyscale(other);
	}

	//multiplication for pixel primitive
	arithmeticalGreyscale pixelGreyscale::operator*(const pixelGreyscale& other) const {
		return arithmeticalGreyscale(*this) * arithmeticalGreyscale(other);
	}

	//addition operator for pixel primitive
	arithmeticalGreyscale pixelGreyscale::operator+(const arithmeticalGreyscale& neighbor) const {
		return arithmeticalGreyscale(*this) + neighbor;
	}
	
	//subtraction operator for pixel primitive
	arithmeticalGreyscale pixelGreyscale::operator-(const arithmeticalGreyscale& neighbor) const {
		return arithmeticalGreyscale(*this) - neighbor;
	}

	//division operator for integer type
	arithmeticalGreyscale pixelGreyscale::operator/(const arithmeticalGreyscale& other) const {
		ASSERT(0 != other.I());
		return arithmeticalGreyscale(*this) / other;
	}

	//multiplication for pixel primitive
	arithmeticalGreyscale pixelGreyscale::operator*(const arithmeticalGreyscale& other) const {
		return arithmeticalGreyscale(*this) * other;
	}

	//addition operator for integer format brightness
	arithmeticalGreyscale pixelGreyscale::operator+(PrecisionType brightness) const {
		return arithmeticalGreyscale(*this) + brightness;
	}

	//subtraction operator for pixel primitive
	arithmeticalGreyscale pixelGreyscale::operator-(PrecisionType brightness) const {
		return arithmeticalGreyscale(*this) - brightness;
	}

	//division operator for integer type
	arithmeticalGreyscale pixelGreyscale::operator/(PrecisionType denom) const {
		ASSERT(0 != denom);
		return arithmeticalGreyscale(*this) / denom;
	}

	//multiplication for integer scalar
	arithmeticalGreyscale pixelGreyscale::operator*(PrecisionType scalar) const {
		return arithmeticalGreyscale(*this) * scalar;
	}

	//less than equal comparison for pixel primitives
	bool pixelGreyscale::operator<=(const pixelGreyscale& m) const {
		return getAvgIntensity() <= m.getAvgIntensity();
	}

	//strict less than comparison for pixel primitives
	bool pixelGreyscale::operator<(const pixelGreyscale& m) const {
		return getAvgIntensity() < m.getAvgIntensity();
	}

	//greater than equal for pixel primitives
	bool pixelGreyscale::operator>=(const pixelGreyscale& m) const {
		return getAvgIntensity() >= m.getAvgIntensity();
	}

	//strict greater than for pixel primitives
	bool pixelGreyscale::operator>(const pixelGreyscale& m) const {
		return getAvgIntensity() > m.getAvgIntensity();
	}

	//greater than equal for pixel primitives
	bool pixelGreyscale::operator==(const pixelGreyscale& m) const {
		bool rv = m.I() == I();
		return rv;
	}

	//luxing algorithm
	arithmeticalGreyscale pixelGreyscale::lux() const {

		return static_cast<arithmeticalGreyscale>(*this);
	}

	//unluxing algorithm
	arithmeticalGreyscale pixelGreyscale::unlux() const {
		return static_cast<arithmeticalGreyscale>(*this);
	}

	//continuous luxing algorithm
	arithmeticalGreyscale pixelGreyscale::continuousLux(double p) const {
		return static_cast<arithmeticalGreyscale>(*this);
	}

	//square root of pixel primitive
	arithmeticalGreyscale pixelGreyscale::root() const {
		arithmeticalGreyscale retVal(*this);
		return retVal.root();
	}

	//get pixel as greyscale
	pixelGreyscale pixelGreyscale::toGrey() const {
		return (*this);
	}

	//get average brightness
	byte pixelGreyscale::getAvgIntensity() const {
		return m_i.value();
	}

	//get binary thresholded pixel
	pixelGreyscale pixelGreyscale::toNegative() const {
		return pixelGreyscale(MAX_BYTE - getAvgIntensity());
	}

	//greyscale histogram shift
	pixelGreyscale pixelGreyscale::histogramShift(unsigned int average) const {
		TBI;
		return (*this);
	}

	pixelGreyscale pixelGreyscale::coordHash(unsigned int x, unsigned int y) const {
		TBI;
		return (*this);
	}

	//get binary thresholded pixel
	pixelGreyscale pixelGreyscale::toBinary(pixelGreyscale thresh) const {
		byte newVal = ((*this) > thresh) ? 0xFF : 0x00;
		return pixelGreyscale(newVal);
	}

	byte pixelGreyscale::I() const {
		return m_i.value();
	}

	pixelGreyscale::operator GreyscalePix::arithmeticalGreyscale() const {
		return GreyscalePix::arithmeticalGreyscale(I());
	}

	template<typename T>
	T min(T x, T y) {
		return (x < y ? x : y);
	}

	template<typename T>
	T max(T x, T y) {
		return (x > y ? x : y);
	}

	pixelGreyscale::operator HSIPix::arithmeticalHSI() const {
		return HSIPix::arithmeticalHSI(0, 0, I());
	}

	pixelGreyscale::operator RGBPix::arithmeticalRGB() const {
		return RGBPix::arithmeticalRGB(I(), I(), I());
	}
};