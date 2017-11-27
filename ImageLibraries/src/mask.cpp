#include "debug.hpp"
#include "mask.h"
#include "constants.hpp"
#include "Array2D.hpp"
#include <math.h>
#include <sstream>

#define             EPS  (2.718282)

mask::mask() : 
	w(0), 
	maskVals() {
}

mask::mask(unsigned int width, PrecisionType* initList, PrecisionType coefficient) :
	w(width),
	fractional(coefficient),
	maskVals(width, width) {
		PrecisionType* curr = initList;
		for (unsigned int i = 0; i < width; i++) {
			for (unsigned int j = 0; j < width; j++) {
				maskVals[i][j] = (*(curr++)) * fractional;
			}
		}
}

mask::~mask() {
}

mask& mask::operator=(const mask& other) {
	w = other.w;
	maskVals = other.maskVals;
	return *this;
}

unsigned int mask::Width() const {
	return w;
}

PrecisionType mask::LOG(PrecisionType x, PrecisionType y, PrecisionType sigma) {
	PrecisionType sig2 = sigma * sigma;
	PrecisionType sig4 = sig2 * sig2;
	PrecisionType x2 = x * x;
	PrecisionType y2 = y * y;
	PrecisionType term1 = (1.0 / (PI * sig4));
	PrecisionType f_xy = ((x2 + y2) / (2.0 * sig2));
	PrecisionType term2 = 1 - f_xy;
	PrecisionType term3 = pow(EPS, -f_xy);
	return (term1 * term2 * term3);
}

mask mask::makeLOG(int width, PrecisionType sigma) {
	//undefined for even block size
	ASSERT(width % 2 == 1);
	
	// Create the data array
	Array2D<PrecisionType> list(width, width);
	PrecisionType scaleFactor = 1;//MAX_BYTE; //maximum intensity for a single byte

	// Work out the "min and max" values. Log is centered around 0, 0
	// so, for a size 5 template (say) we want to get the values from
	// -2 to +2, ie: -2, -1, 0, +1, +2 and feed those into the formula.
	int min = -(width / 2);
	int max = (width / 2);
	PrecisionType scale = 1;

	// We also need a count to index into the data array...
	int lCount = 0;

	for(int y = min; y <= max; ++y) {
		for(int x = min; x <= max; ++x) {
			// Get the LoG value for this (x,y) pair
			list[0][lCount++] = LOG(x/scale, y/scale, sigma) * scaleFactor;
		}
	}

	mask retVal(width,list[0],10.0);
	return retVal;
}

PrecisionType mask::getCoefficient() const {
	return fractional;
}

std::string mask::toString() const {
	std::stringstream ss;
	PrecisionType p = 0;
	ss << "[\n";
	for (unsigned int i = 0; i < w; i++) {
		ss << "\t[";
		for (unsigned int j = 0; j < w; j++) {
			ss << maskVals[i][j];
			p += maskVals[i][j];
			if (j != w - 1) {
				ss << "|";
			}
		}
		ss << "]\n";
	}
	ss << "SUM(" << p <<")]\n";
	return ss.str();
}

const Array2D<PrecisionType>& mask::getMaskData() const {
	return maskVals;
}
