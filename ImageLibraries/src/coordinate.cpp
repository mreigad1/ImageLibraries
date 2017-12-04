#include "coordinate.h"

coordinate::coordinate() : 
	raw_x(0),
	raw_y(0),
	x(raw_x),
	y(raw_y) {
}

coordinate::coordinate(long long _x, long long _y) : 
	raw_x(_x),
	raw_y(_y),
	x(raw_x),
	y(raw_y) {
}

coordinate::coordinate(const coordinate& other) :
	raw_x(other.x),
	raw_y(other.y),
	x(raw_x),
	y(raw_y) {
}

coordinate& coordinate::operator=(const coordinate& other) {
	raw_x = other.x;
	raw_y = other.y;
	return *this;
}