#pragma once
#include "debug.hpp"
#include "colorModelEnums.hpp"

template <ColorIndex Value> class ArithmeticalIntensity;

template <ColorIndex Value> class ByteIntensity {
	private:
		byte intensity;
	public:
		ByteIntensity() :
			intensity(0) {
		}

		ByteIntensity(byte brightness) :
			intensity(brightness) {
		}

		ByteIntensity(const ByteIntensity<Value>& other) :
			intensity(other.intensity) {
		}

		ByteIntensity(const ArithmeticalIntensity<Value>& other) :
			intensity(0) {
				PrecisionType newVal = other.value() + 0.5;
				newVal = (newVal < 0)    ?    0 : newVal;
				newVal = (newVal > 0xFF) ? 0xFF : newVal;
				intensity = static_cast<byte>(newVal);
		}

		ByteIntensity(int operationWord) :
			intensity(0) {
				operationWord = (operationWord <    0) ?    0 : operationWord;
				operationWord = (operationWord > 0xFF) ? 0xFF : operationWord;
				intensity = operationWord;
		}

		ByteIntensity& operator=(const ByteIntensity<Value> other) {
			intensity = other.intensity;
			return *this;
		}

		ByteIntensity& operator=(const ArithmeticalIntensity<Value> other) {
			PrecisionType newVal = other.value() + 0.5;
			newVal = (newVal < 0)    ?    0 : newVal;
			newVal = (newVal > 0xFF) ? 0xFF : newVal;
			intensity = static_cast<byte>(newVal);
			return *this;
		}

		ByteIntensity& operator=(int operationWord) {
			operationWord = (operationWord <    0) ?    0 : operationWord;
			operationWord = (operationWord > 0xFF) ? 0xFF : operationWord;
			intensity = operationWord;
			return *this;
		}

		byte value() const {
			return intensity;
		}

		ArithmeticalIntensity<Value> operator+(int brightness) const {
			return ArithmeticalIntensity<Value>(value()) + brightness;
		}

		ArithmeticalIntensity<Value> operator-(int brightness) const {
			return ArithmeticalIntensity<Value>(value()) - brightness;
		}

		ArithmeticalIntensity<Value> operator*(int brightness) const {
			return ArithmeticalIntensity<Value>(value()) * brightness;
		}

		ArithmeticalIntensity<Value> operator/(int brightness) const {
			ASSERT(brightness != 0);
			return ArithmeticalIntensity<Value>(value()) / brightness;
		}

		ArithmeticalIntensity<Value> operator+(const ByteIntensity<Value> other) const {
			return ArithmeticalIntensity<Value>(value()) + ArithmeticalIntensity<Value>(other.value());
		}

		ArithmeticalIntensity<Value> operator-(const ByteIntensity<Value> other) const {
			return ArithmeticalIntensity<Value>(value()) - ArithmeticalIntensity<Value>(other.value());
		}

		ArithmeticalIntensity<Value> operator*(const ByteIntensity<Value> other) const {
			return ArithmeticalIntensity<Value>(value()) * ArithmeticalIntensity<Value>(other.value());
		}

		ArithmeticalIntensity<Value> operator/(const ByteIntensity<Value> other) const {
			int brightness = other.value();
			ASSERT(brightness != 0);
			return ArithmeticalIntensity<Value>(value()) * brightness;
		}
};

template <ColorIndex Value> ArithmeticalIntensity<Value> operator+(int L, ByteIntensity<Value> R) {
	return ArithmeticalIntensity<Value>(L) + ArithmeticalIntensity<Value>(R.value());
}

template <ColorIndex Value> ArithmeticalIntensity<Value> operator-(int L, ByteIntensity<Value> R) {
	return ArithmeticalIntensity<Value>(L) - ArithmeticalIntensity<Value>(R.value());
}

template <ColorIndex Value> ArithmeticalIntensity<Value> operator*(int L, ByteIntensity<Value> R) {
	return ArithmeticalIntensity<Value>(L) * ArithmeticalIntensity<Value>(R.value());
}

#include "ArithmeticalIntensity.hpp"
