#pragma once
#include "debug.hpp"
#include "colorModelEnums.hpp"
#include "Array2D.hpp"

//mask stores data for square masks filters
class mask {
	private:
		unsigned int w;
		PrecisionType fractional;
		Array2D<PrecisionType> maskVals;
		static PrecisionType LOG(PrecisionType x, PrecisionType y, PrecisionType sigma);
	public:
		//implicit constructor
		mask();
		//explicit constructor, pass width of square with array, optional coefficient to simplify user static PrecisionType array
		mask(unsigned int width, PrecisionType* initList, PrecisionType coefficient = 1.0);
		~mask();
		mask& operator=(const mask& other);
		static mask makeLOG(int width, PrecisionType sigma);
		unsigned int Width() const;
		PrecisionType getCoefficient() const;
		std::string toString() const;
		const Array2D<PrecisionType>& getMaskData() const;
};
