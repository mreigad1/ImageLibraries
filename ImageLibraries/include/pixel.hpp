#pragma once
#include "debug.hpp"
#include "colorModelEnums.hpp"

template<typename Implementation> class pixel {
	public:
		//empty constructor
		pixel() {
		}

		//cast operator to cast to Implementation
		explicit operator Implementation() const {
			Implementation const* tmp = (Implementation*)this;
			Implementation retVal = *tmp;
			return retVal;
		}

		//assignment operator from pixel primitive
		Implementation& operator=(const pixel<Implementation> data) {
			return (static_cast<Implementation>(*this)) = (static_cast<const Implementation>(data));
		}

		//assignment operator from pixel primitive
		Implementation& operator=(const Implementation data) {
			return (static_cast<Implementation>(*this)) = data;
		}

		//addition operator for pixel primitive
		pixel<Implementation> operator+(const Implementation& neighbor) const {
			return (static_cast<Implementation>(*this)) + neighbor;
		}

		//addition operator for integer format brightness
		pixel<Implementation> operator+(const int& brightness) const {
			return (static_cast<Implementation>(*this)) + brightness;
		}

		//subtraction operator for pixel primitive
		pixel<Implementation> operator-(const Implementation& neighbor) const {
			return (static_cast<Implementation>(*this)) - neighbor;
		}

		//subtraction operator for pixel primitive
		pixel<Implementation> operator-(const int& brightness) const {
			return (static_cast<Implementation>(*this)) - brightness;
		}

		//division operator for integer type
		pixel<Implementation> operator/(const int& denom) const {
			return (static_cast<Implementation>(*this)) / denom;
		}

		//multiplication for pixel primitive
		pixel<Implementation> operator*(const Implementation& other) const {
			return (static_cast<Implementation>(*this)) * other;
		}

		//multiplication for integer scalar
		pixel<Implementation> operator*(const int& scalar) const {
			return (static_cast<Implementation>(*this)) * scalar;
		}

		//less than equal comparison for pixel primitives
		bool operator<=(const pixel<Implementation>& m) const {
			return (static_cast<Implementation>(*this)) <= (static_cast<Implementation>(m));
		}

		//strict less than comparison for pixel primitives
		bool operator<(const pixel<Implementation>& m) const {
			return (static_cast<Implementation>(*this)) < (static_cast<Implementation>(m));
		}

		//greater than equal for pixel primitives
		bool operator>=(const pixel<Implementation>& m) const {
			return (static_cast<Implementation>(*this)) >= (static_cast<Implementation>(m));
		}

		//strict greater than for pixel primitives
		bool operator>(const pixel<Implementation>& m) const {
			return (static_cast<Implementation>(*this)) > (static_cast<Implementation>(m));
		}

		//Method to return pixel with color based on coordinate
		Implementation coordHash(unsigned int x, unsigned int y) const {
			return static_cast<Implementation>(*this).coordHash(x,y);
		}

		//luxing algorithm
		Implementation lux() const {
			return static_cast<Implementation>(*this).lux();
		}

		//unluxing algorithm
		Implementation unlux() const {
			return static_cast<Implementation>(*this).unlux();
		}

		//continuous luxing algorithm
		Implementation continuousLux(double p) const {
			return static_cast<Implementation>(*this).continuousLux(p);
		}

		//square root of pixel primitive
		Implementation root() const {
			return static_cast<Implementation>(*this).root();
		}

		//get pixel as greyscale
		Implementation toGrey() const {
			return static_cast<Implementation>(*this).toGrey();
		}

		//perform greyscale histogram shift
		Implementation histogramShift(unsigned int average) const {
			return static_cast<Implementation>(*this).histogramShift(average);
		}

		//invert pixel intensities()
		Implementation toNegative() const {
			return static_cast<Implementation*>(this)->toNegative();
		}

		//get average brightness
		byte getAvgIntensity() const {
			return static_cast<Implementation>(*this).getAvgIntensity();
		}

		//get binary thresholded pixel
		Implementation toBinary(Implementation& thresh) const {
			return static_cast<Implementation>(*this).toBinary(thresh);
		}
};