#pragma once
//This file is only to be included in the ByteIntensity class as an auxillary class

template <ColorIndex Value> class ArithmeticalIntensity {
	private:
		PrecisionType intensity;
	public:
		ArithmeticalIntensity() :
			intensity(0) {
		}

		ArithmeticalIntensity(PrecisionType brightness) :
			intensity(brightness) {
		}

		ArithmeticalIntensity(const ArithmeticalIntensity<Value>& other) :
			intensity(other.intensity) {
		}

		ArithmeticalIntensity(const ByteIntensity<Value>& other) :
			intensity(static_cast<PrecisionType>(other.value())) {
		}

		ArithmeticalIntensity& operator=(const ArithmeticalIntensity<Value> other) {
			intensity = other.value();
			return *this;
		}

		ArithmeticalIntensity& operator=(const ByteIntensity<Value> other) {
			intensity = static_cast<PrecisionType>(other.value());
			return *this;
		}

		ArithmeticalIntensity& operator=(const PrecisionType other) {
			intensity = other;
			return *this;
		}

		operator ByteIntensity<Value>() const {
			return ByteIntensity<Value>(*this);
		}

		PrecisionType value() const {
			return intensity;
		}

		ArithmeticalIntensity operator+(PrecisionType brightness) const {
			ArithmeticalIntensity retVal(value() + brightness);
			return retVal;
		}

		ArithmeticalIntensity operator-(PrecisionType brightness) const {
			ArithmeticalIntensity retVal(value() - brightness);
			return retVal;
		}

		ArithmeticalIntensity operator*(PrecisionType brightness) const {
			ArithmeticalIntensity retVal(value() * brightness);
			return retVal;
		}

		ArithmeticalIntensity operator/(PrecisionType brightness) const {
			ASSERT(brightness != 0);
			ArithmeticalIntensity retVal(value() / brightness);
			return retVal;
		}

		ArithmeticalIntensity operator+(const ArithmeticalIntensity<Value> other) const {
			ArithmeticalIntensity retVal(value() + other.value());
			return retVal;
		}

		ArithmeticalIntensity operator-(const ArithmeticalIntensity<Value> other) const {
			ArithmeticalIntensity retVal(value() - other.value());
			return retVal;
		}

		ArithmeticalIntensity operator*(const ArithmeticalIntensity<Value> other) const {
			ArithmeticalIntensity retVal(value() * other.value());
			return retVal;
		}

		ArithmeticalIntensity operator/(const ArithmeticalIntensity<Value> other) const {
			PrecisionType brightness = other.value();
			ASSERT(brightness != 0);
			ArithmeticalIntensity retVal(value() / brightness);
			return retVal;
		}
};

template <ColorIndex Value> ArithmeticalIntensity<Value> operator+(PrecisionType L, ArithmeticalIntensity<Value> R) {
	L = L + R.value();
	ArithmeticalIntensity<Value> retVal(L);
	return retVal;
}

template <ColorIndex Value> ArithmeticalIntensity<Value> operator-(PrecisionType L, ArithmeticalIntensity<Value> R) {
	L = L - R.value();
	ArithmeticalIntensity<Value> retVal(L);
	return retVal;
}

template <ColorIndex Value> ArithmeticalIntensity<Value> operator*(PrecisionType L, ArithmeticalIntensity<Value> R) {
	L = L * R.value();
	ArithmeticalIntensity<Value> retVal(L);
	return retVal;
}
