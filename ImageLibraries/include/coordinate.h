#pragma once
#ifndef IG_CODE_DEF
	class coordinate {
		public:
			const unsigned int x;
			const unsigned int y;

			coordinate(unsigned int _x, unsigned int _y);
			coordinate(const coordinate& other);
			coordinate& operator=(const coordinate& other);
	};
#endif
#define IG_CODE_DEF