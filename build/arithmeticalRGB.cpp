#include "debug.hpp"
#include "arithmeticalRGB.h"
#include <sstream>
#include <cmath>

namespace RGBPix {
	arithmeticalRGB::arithmeticalRGB() {
	}

	arithmeticalRGB::arithmeticalRGB(byte r, byte g, byte b) {
		rgb.m_r = r;
		rgb.m_g = g;
		rgb.m_b = b;
	}

	arithmeticalRGB::arithmeticalRGB(const arithmeticalRGB& other) {
		rgb.m_r = other.rgb.m_r;
		rgb.m_g = other.rgb.m_g;
		rgb.m_b = other.rgb.m_b;
	}

	arithmeticalRGB& arithmeticalRGB::operator=(const arithmeticalRGB& other) {
		rgb.m_r = other.rgb.m_r;
		rgb.m_g = other.rgb.m_g;
		rgb.m_b = other.rgb.m_b;
		return *this;
	}

	PrecisionType arithmeticalRGB::R() const {
		return rgb.m_r.value();
	}

	PrecisionType arithmeticalRGB::G() const {
		return rgb.m_g.value();
	}

	PrecisionType arithmeticalRGB::B() const {
		return rgb.m_b.value();
	}

	//return stringified object
	std::string arithmeticalRGB::toString() const {
		std::stringstream rv;
		rv << "[" << rgb.m_r.value() << "|" << rgb.m_g.value() << "|" << rgb.m_b.value() << "]\n";
		std::string retVal = rv.str();
		return retVal;
	}

	arithmeticalRGB arithmeticalRGB::operator+(const arithmeticalRGB& neighbor) const {
		arithmeticalRGB retVal(*this);
		retVal.rgb.m_r = retVal.rgb.m_r + neighbor.rgb.m_r;
		retVal.rgb.m_g = retVal.rgb.m_g + neighbor.rgb.m_g;
		retVal.rgb.m_b = retVal.rgb.m_b + neighbor.rgb.m_b;
		return retVal;
	}

	arithmeticalRGB arithmeticalRGB::operator-(const arithmeticalRGB& neighbor) const {
		arithmeticalRGB retVal(*this);
		retVal.rgb.m_r = retVal.rgb.m_r - neighbor.rgb.m_r;
		retVal.rgb.m_g = retVal.rgb.m_g - neighbor.rgb.m_g;
		retVal.rgb.m_b = retVal.rgb.m_b - neighbor.rgb.m_b;
		return retVal;
	}

	arithmeticalRGB arithmeticalRGB::operator/(const arithmeticalRGB& neighbor) const {
		ASSERT(0 != neighbor.rgb.m_r.value());
		ASSERT(0 != neighbor.rgb.m_g.value());
		ASSERT(0 != neighbor.rgb.m_b.value());
		arithmeticalRGB retVal(*this);
		retVal.rgb.m_r = retVal.rgb.m_r / neighbor.rgb.m_r;
		retVal.rgb.m_g = retVal.rgb.m_g / neighbor.rgb.m_g;
		retVal.rgb.m_b = retVal.rgb.m_b / neighbor.rgb.m_b;
		return retVal;
	}

	arithmeticalRGB arithmeticalRGB::operator*(const arithmeticalRGB& neighbor) const {
		arithmeticalRGB retVal(*this);
		retVal.rgb.m_r = retVal.rgb.m_r * neighbor.rgb.m_r;
		retVal.rgb.m_g = retVal.rgb.m_g * neighbor.rgb.m_g;
		retVal.rgb.m_b = retVal.rgb.m_b * neighbor.rgb.m_b;
		return retVal;
	}

	arithmeticalRGB arithmeticalRGB::operator+(const PrecisionType& brightness) const {
		arithmeticalRGB retVal(*this);
		retVal.rgb.m_r = retVal.rgb.m_r + brightness;
		retVal.rgb.m_g = retVal.rgb.m_g + brightness;
		retVal.rgb.m_b = retVal.rgb.m_b + brightness;
		return retVal;
	}
	
	arithmeticalRGB arithmeticalRGB::operator-(const PrecisionType& brightness) const {
		arithmeticalRGB retVal(*this);
		retVal.rgb.m_r = retVal.rgb.m_r - brightness;
		retVal.rgb.m_g = retVal.rgb.m_g - brightness;
		retVal.rgb.m_b = retVal.rgb.m_b - brightness;
		return retVal;
	}

	arithmeticalRGB arithmeticalRGB::operator/(const PrecisionType& brightness) const {
		ASSERT(0 != brightness);
		arithmeticalRGB retVal(*this);
		retVal.rgb.m_r = retVal.rgb.m_r / brightness;
		retVal.rgb.m_g = retVal.rgb.m_g / brightness;
		retVal.rgb.m_b = retVal.rgb.m_b / brightness;
		return retVal;
	}

	arithmeticalRGB arithmeticalRGB::operator*(const PrecisionType& brightness) const {
		arithmeticalRGB retVal(*this);
		retVal.rgb.m_r = retVal.rgb.m_r * brightness;
		retVal.rgb.m_g = retVal.rgb.m_g * brightness;
		retVal.rgb.m_b = retVal.rgb.m_b * brightness;
		return retVal;
	}

	//return root of this pixel
	arithmeticalRGB arithmeticalRGB::root() const {
		arithmeticalRGB retVal;
		retVal.rgb.m_r = sqrt(R());
		retVal.rgb.m_g = sqrt(G());
		retVal.rgb.m_b = sqrt(B());
		return retVal;
	}
};