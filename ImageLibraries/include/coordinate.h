#pragma once
#ifndef IG_CODE_DEF
	class coordinate {

		long long raw_x;
		long long raw_y;

		public:
			const long long& x;
			const long long& y;

			coordinate();
			coordinate(long long _x, long long _y);
			coordinate(const coordinate& other);

			//only way to overwrite const quality above
			coordinate& operator=(const coordinate& other);
	};
#endif
#define IG_CODE_DEF