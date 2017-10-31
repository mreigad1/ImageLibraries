#include "debug.hpp"
#include "arithmeticalGreyscale.h"
#include <sstream>
#include <cmath>

namespace GreyscalePix {
	arithmeticalGreyscale::arithmeticalGreyscale() {
	}

	arithmeticalGreyscale::arithmeticalGreyscale(byte intensity) :
		m_i(intensity) {
	}

	arithmeticalGreyscale::arithmeticalGreyscale(const arithmeticalGreyscale& other) :
		m_i(other.I()) {
	}

	arithmeticalGreyscale& arithmeticalGreyscale::operator=(const arithmeticalGreyscale& other) {
		m_i = other.I();
		return *this;
	}

	PrecisionType arithmeticalGreyscale::I() const {
		return m_i.value();
	}

	//return stringified object
	std::string arithmeticalGreyscale::toString() const {
		std::stringstream rv;
		rv << "[" << I() << "]\n";
		std::string retVal = rv.str();
		return retVal;
	}

	arithmeticalGreyscale arithmeticalGreyscale::operator+(const arithmeticalGreyscale& neighbor) const {
		arithmeticalGreyscale retVal(*this);
		retVal.m_i = retVal.m_i + neighbor.m_i;
		return retVal;
	}

	arithmeticalGreyscale arithmeticalGreyscale::operator-(const arithmeticalGreyscale& neighbor) const {
		arithmeticalGreyscale retVal(*this);
		retVal.m_i = retVal.m_i - neighbor.m_i;
		return retVal;
	}

	arithmeticalGreyscale arithmeticalGreyscale::operator/(const arithmeticalGreyscale& neighbor) const {
		ASSERT(0 != neighbor.m_i.value());
		arithmeticalGreyscale retVal(*this);
		retVal.m_i = retVal.m_i / neighbor.m_i;
		return retVal;
	}

	arithmeticalGreyscale arithmeticalGreyscale::operator*(const arithmeticalGreyscale& neighbor) const {
		arithmeticalGreyscale retVal(*this);
		retVal.m_i = retVal.m_i * neighbor.m_i;
		return retVal;
	}

	arithmeticalGreyscale arithmeticalGreyscale::operator+(const PrecisionType& brightness) const {
		arithmeticalGreyscale retVal(*this);
		retVal.m_i = retVal.m_i + brightness;
		return retVal;
	}
	
	arithmeticalGreyscale arithmeticalGreyscale::operator-(const PrecisionType& brightness) const {
		arithmeticalGreyscale retVal(*this);
		retVal.m_i = retVal.m_i - brightness;
		return retVal;
	}

	arithmeticalGreyscale arithmeticalGreyscale::operator/(const PrecisionType& brightness) const {
		ASSERT(0 != brightness);
		arithmeticalGreyscale retVal(*this);
		retVal.m_i = retVal.m_i / brightness;
		return retVal;
	}

	arithmeticalGreyscale arithmeticalGreyscale::operator*(const PrecisionType& brightness) const {
		arithmeticalGreyscale retVal(*this);
		retVal.m_i = retVal.m_i * brightness;
		return retVal;
	}

	//return root of this pixel
	arithmeticalGreyscale arithmeticalGreyscale::root() const {
		arithmeticalGreyscale retVal;
		retVal.m_i = sqrt(I());
		return retVal;
	}
};