#include "coordinate.h"

coordinate::coordinate(long long _x, long long _y) : 
	x(_x),
	y(_y) {
}

coordinate::coordinate(const coordinate& other) :
	x(other.x),
	y(other.y) {
}

coordinate& coordinate::operator=(const coordinate& other) {
	const_cast<long long&>(x) = other.x;
	const_cast<long long&>(y) = other.y;
	return *this;
}