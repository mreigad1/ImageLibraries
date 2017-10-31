#pragma once
#include "debug.hpp"
#include "colorModelEnums.hpp"
#include "colorModelTypes.hpp"
#include "ByteIntensity.hpp"
#include "pixel.hpp"

namespace GreyscalePix {

	class arithmeticalGreyscale : pixel<arithmeticalGreyscale> {
		private:
			AI_grey m_i;
		public:
			arithmeticalGreyscale();														//empty constructor
			arithmeticalGreyscale(byte intensity);											//explicit constructor
			arithmeticalGreyscale(const arithmeticalGreyscale& other);						//const constructor
			arithmeticalGreyscale& operator=(const arithmeticalGreyscale& data);			//assignment operator from rgb pixel
			std::string toString() const;													//return stringified object
			arithmeticalGreyscale operator+(const arithmeticalGreyscale& neighbor)	const;	//addition operator for pixel primitive
			arithmeticalGreyscale operator+(const PrecisionType  & brightness)		const;	//addition operator for PrecisionType format brightness
			arithmeticalGreyscale operator-(const arithmeticalGreyscale& neighbor)	const;	//subtraction operator for pixel primitive
			arithmeticalGreyscale operator-(const PrecisionType  & brightness)		const;	//subtraction operator for PrecisionType primitive
			arithmeticalGreyscale operator/(const arithmeticalGreyscale& other)		const;	//division operator for integer type
			arithmeticalGreyscale operator/(const PrecisionType  & denom)			const;	//division operator for PrecisionType type
			arithmeticalGreyscale operator*(const arithmeticalGreyscale& other)		const;	//multiplication for pixel primitive
			arithmeticalGreyscale operator*(const PrecisionType  & scalar)			const;	//multiplication for PrecisionType scalar
			arithmeticalGreyscale root() const;												//return root of this pixel
			PrecisionType I() const;
	};
};