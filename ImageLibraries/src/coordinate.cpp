#include "coordinate.h"

coordinate::coordinate(unsigned int _x, unsigned int _y) : 
	x(_x),
	y(_y) {
}

coordinate::coordinate(const coordinate& other) :
	x(other.x),
	y(other.y) {
}

coordinate& coordinate::operator=(const coordinate& other) {
	x = other.x;
	y = other.y;
	return *this;
}