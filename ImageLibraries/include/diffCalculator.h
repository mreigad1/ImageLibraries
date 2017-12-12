#include "debug.hpp"
#include "colorModelEnums.hpp"
using namespace std;

class diffCalculator {
	public:
		//dirties the value flag
		void invalidateDiff();
		double exportDifference(byte* const outBuffer);
		double imageDifference();

		void exportImage(byte* const outBuffer, int imageNumber);

		static diffCalculator makeCalculator(const int height, const int width, const string& image1, const string& image2, const byte* const i0, const byte* const i1);

		//To be used as a move semantic constructors, compiler is pre-C++11
		//with no smart pointers this will allow the most recent class copy
		//to own freeing mmap'd region
		diffCalculator(const diffCalculator& other);
		diffCalculator(diffCalculator& other);

		~diffCalculator();

	private:
		const int height;
		const int width;
		const int fd[2];
		const byte* const imgMap[2];
		double imageDiff;
		bool diffReady;
		mutable bool resourcesClosed;

		diffCalculator(const int Height, const int Width, const int fd0, const int fd1, const byte* const imageMap0, const byte* const imageMap1);
		void closeResources();
		double imageIteration(byte* const outBuff = NULL);

		static long long getTotalSize(const int height, const int width);
		static void checkFDSuccess(const int fd0, const int fd1);
		static void checkMapSuccess(const byte* const img0, const byte* const img1 );
};