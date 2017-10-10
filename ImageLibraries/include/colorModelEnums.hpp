#pragma once

//define color precision types
typedef unsigned char byte;
typedef float PrecisionType;

//enumerates different types of color models
enum ColorModel {
	GREY = 1,
	RGB  = 2,
	HSI  = 3,
	YUV  = 4
};

//enumerates color types
enum ColorIndex {
	ind_grey = 1,
	ind_r    = 2,
	ind_g    = 3,
	ind_b    = 4,
	ind_h    = 5,
	ind_s    = 6,
	ind_i    = 7,
	ind_y    = 8,
	ind_u    = 9,
	ind_v    = 10
};
