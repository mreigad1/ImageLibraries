#include "debug.hpp"
#include "constants.hpp"
#include "arithmeticalHSI.h"
#include <sstream>
#include <cmath>

namespace HSIPix {
	arithmeticalHSI::arithmeticalHSI() {
	}

	arithmeticalHSI::arithmeticalHSI(PrecisionType h, PrecisionType s, PrecisionType i) {
		hsi.m_h = h;
		hsi.m_s = s;
		hsi.m_i = i;
	}

	arithmeticalHSI::arithmeticalHSI(const arithmeticalHSI& other) {
		hsi.m_h = other.hsi.m_h;
		hsi.m_s = other.hsi.m_s;
		hsi.m_i = other.hsi.m_i;
	}

	arithmeticalHSI& arithmeticalHSI::operator=(const arithmeticalHSI& other) {
		hsi.m_h = other.hsi.m_h;
		hsi.m_s = other.hsi.m_s;
		hsi.m_i = other.hsi.m_i;
		return *this;
	}

	PrecisionType arithmeticalHSI::H() const {
		return hsi.m_h.value();
	}

	PrecisionType arithmeticalHSI::S() const {
		return hsi.m_s.value();
	}

	PrecisionType arithmeticalHSI::I() const {
		return hsi.m_i.value();
	}

	//return stringified object
	std::string arithmeticalHSI::toString() const {
		std::stringstream rv;
		rv << "[" << hsi.m_h.value() << "|" << hsi.m_s.value() << "|" << hsi.m_i.value() << "]\n";
		std::string retVal = rv.str();
		return retVal;
	}

	arithmeticalHSI arithmeticalHSI::operator+(const arithmeticalHSI& neighbor) const {
		arithmeticalHSI retVal(*this);
		//retVal.hsi.m_h = retVal.hsi.m_h + neighbor.hsi.m_h;
		//retVal.hsi.m_s = retVal.hsi.m_s + neighbor.hsi.m_s;
		retVal.hsi.m_i = retVal.hsi.m_i + neighbor.hsi.m_i;
		return retVal;
	}

	arithmeticalHSI arithmeticalHSI::operator-(const arithmeticalHSI& neighbor) const {
		arithmeticalHSI retVal(*this);
		//retVal.hsi.m_h = retVal.hsi.m_h - neighbor.hsi.m_h;
		//retVal.hsi.m_s = retVal.hsi.m_s - neighbor.hsi.m_s;
		retVal.hsi.m_i = retVal.hsi.m_i - neighbor.hsi.m_i;
		return retVal;
	}

	arithmeticalHSI arithmeticalHSI::operator/(const arithmeticalHSI& neighbor) const {
		ASSERT(0 != neighbor.hsi.m_h.value());
		ASSERT(0 != neighbor.hsi.m_s.value());
		ASSERT(0 != neighbor.hsi.m_i.value());
		arithmeticalHSI retVal(*this);
		//retVal.hsi.m_h = retVal.hsi.m_h / neighbor.hsi.m_h;
		//retVal.hsi.m_s = retVal.hsi.m_s / neighbor.hsi.m_s;
		retVal.hsi.m_i = retVal.hsi.m_i / neighbor.hsi.m_i;
		return retVal;
	}

	arithmeticalHSI arithmeticalHSI::operator*(const arithmeticalHSI& neighbor) const {
		arithmeticalHSI retVal(*this);
		//retVal.hsi.m_h = retVal.hsi.m_h * neighbor.hsi.m_h;
		//retVal.hsi.m_s = retVal.hsi.m_s * neighbor.hsi.m_s;
		retVal.hsi.m_i = retVal.hsi.m_i * neighbor.hsi.m_i;
		return retVal;
	}

	arithmeticalHSI arithmeticalHSI::operator+(const PrecisionType& brightness) const {
		arithmeticalHSI retVal(*this);
		//retVal.hsi.m_h = retVal.hsi.m_h + brightness;
		//retVal.hsi.m_s = retVal.hsi.m_s + brightness;
		retVal.hsi.m_i = retVal.hsi.m_i + brightness;
		return retVal;
	}
	
	arithmeticalHSI arithmeticalHSI::operator-(const PrecisionType& brightness) const {
		arithmeticalHSI retVal(*this);
		//retVal.hsi.m_h = retVal.hsi.m_h - brightness;
		//retVal.hsi.m_s = retVal.hsi.m_s - brightness;
		retVal.hsi.m_i = retVal.hsi.m_i - brightness;
		return retVal;
	}

	arithmeticalHSI arithmeticalHSI::operator/(const PrecisionType& brightness) const {
		ASSERT(0 != brightness);
		arithmeticalHSI retVal(*this);
		//retVal.hsi.m_h = retVal.hsi.m_h / brightness;
		//retVal.hsi.m_s = retVal.hsi.m_s / brightness;
		retVal.hsi.m_i = retVal.hsi.m_i / brightness;
		return retVal;
	}

	arithmeticalHSI arithmeticalHSI::operator*(const PrecisionType& brightness) const {
		arithmeticalHSI retVal(*this);
		//retVal.hsi.m_h = retVal.hsi.m_h * brightness;
		//retVal.hsi.m_s = retVal.hsi.m_s * brightness;
		retVal.hsi.m_i = retVal.hsi.m_i * brightness;
		return retVal;
	}

	//return root of this pixel
	arithmeticalHSI arithmeticalHSI::root() const {
		arithmeticalHSI retVal;
		//retVal.hsi.m_h = sqrt(H());
		//retVal.hsi.m_s = sqrt(S());
		retVal.hsi.m_i = sqrt(I());
		return retVal;
	}

	PrecisionType arithmeticalHSI::dataComponent1() const {
		return H();
	}

	PrecisionType arithmeticalHSI::dataComponent2() const {
		return S();
	}

	PrecisionType arithmeticalHSI::dataComponent3() const {
		return I();
	}

};