#include <iostream>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "debug.hpp"
#include "colorModelEnums.hpp"
#include "diffCalculator.h"
using namespace std;

//dirties the value flag
void diffCalculator::invalidateDiff() {
	diffReady = false;
}

double diffCalculator::imageDifference() {
	return imageIteration(NULL);
}

double diffCalculator::exportDifference(byte* const outBuffer) {
	return imageIteration(outBuffer);
}

double diffCalculator::imageIteration(byte* const outBuffer) {
	if (diffReady && NULL == outBuffer) {
		return imageDiff;
	} else {
		const long long imageSize = getTotalSize(height, width);
		int countDiff = 0;
		const int diffThresh = 25;
		int k = 0;
		int diff = 0;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				k = i * width + j;
				diff = imgMap[0][k];
				diff -= imgMap[1][k];
				diff = (diff < 0) ? (diff * -1) : diff;

				if (diff >= diffThresh) {
					countDiff++;
					if (outBuffer) {
						outBuffer[k] = 0xFF;
					}
				} else if (outBuffer) {
					outBuffer[k] = 0x00;
				}
			}
		}
		imageDiff = static_cast<double>(countDiff) / imageSize; //cache new value before returning
		diffReady = true;
		return imageDiff;
	}
}

void diffCalculator::exportImage(byte* const outBuffer, int imageNumber) {
	if (0 == imageNumber || 1 == imageNumber) {
		const long long imageSize = getTotalSize(height, width);
		for (int k = 0; k < imageSize; k++) {
			outBuffer[k] = imgMap[imageNumber][k];
		}
	}
}

diffCalculator diffCalculator::makeCalculator(const int height, const int width, const string& image1, const string& image2, const byte* const i0, const byte* const i1) {
	//const long long imageSize = getTotalSize(height, width);
	int _fd[2] = { 
		open(image1.c_str(), O_RDWR) ,
		open(image2.c_str(), O_RDWR)
	};
	checkFDSuccess(_fd[0], _fd[1]);

	const byte* const _imgMap[2] = {
		// (const byte*) mmap(NULL, imageSize, PROT_READ, MAP_SHARED, _fd[0], 0) ,
		// (const byte*) mmap(NULL, imageSize, PROT_READ, MAP_SHARED, _fd[1], 0)
		i0, i1
	};
	checkMapSuccess(_imgMap[0], _imgMap[1]);

	return diffCalculator(height, width, _fd[0], _fd[1], _imgMap[0], _imgMap[1]);
}

diffCalculator::diffCalculator(
	const int Height,
	const int Width,
	const int fd0,
	const int fd1,
	const byte* const imageMap0,
	const byte* const imageMap1
) :
	height(Height),
	width(Width),
	fd{fd0, fd1},
	imgMap{imageMap0, imageMap1},
	imageDiff(0),
	diffReady(false),
	resourcesClosed(false)
{
}

//To be used as a move semantic constructor, compiler is pre-C++11
//with no smart pointers this will allow the most recent class copy
//to own freeing mmap'd region
diffCalculator::diffCalculator(const diffCalculator& other) :
	height(other.height),
	width(other.width),
	fd{other.fd[0], other.fd[1]},
	imgMap{other.imgMap[0], other.imgMap[1]},
	imageDiff(0),
	diffReady(false),
	resourcesClosed(false)
{
	other.resourcesClosed = true;
}

//To be used as a move semantic constructor, compiler is pre-C++11
//with no smart pointers this will allow the most recent class copy
//to own freeing mmap'd region
diffCalculator::diffCalculator(diffCalculator& other) :
	height(other.height),
	width(other.width),
	fd{other.fd[0], other.fd[1]},
	imgMap{other.imgMap[0], other.imgMap[1]},
	imageDiff(0),
	diffReady(false),
	resourcesClosed(false)
{
	other.resourcesClosed = true;
}


diffCalculator::~diffCalculator() {
	closeResources();
}

void diffCalculator::closeResources() {
	if (false == resourcesClosed) {
		const long long imageSize = getTotalSize(height, width);
		munmap((void*)imgMap[0], imageSize);
		munmap((void*)imgMap[1], imageSize);
		close(fd[0]);
		close(fd[1]);
		resourcesClosed = true;
	}
}

long long diffCalculator::getTotalSize(const int height, const int width) {
	if (height <= 0) {
		LINE_LOG;
		abort();
	}
	if (width <= 0) {
		LINE_LOG;
		abort();
	}
	const long long imageSize = height * width;
	if (imageSize <= 0) {
		LINE_LOG;
		abort();
	}
	return imageSize;
}

void diffCalculator::checkFDSuccess(const int fd0, const int fd1) {
	if (0 == fd0) {
		LINE_LOG;
		abort();
	}
	if (0 == fd1) {
		LINE_LOG;
		abort();
	}
}

void diffCalculator::checkMapSuccess(const byte* const img0, const byte* const img1 ) {
	const byte* const imgMap[2] = { img0, img1 }; 
	if (0 == imgMap[0]) {
		LINE_LOG;
		abort();
	} else if (0 == imgMap[1]) {
		LINE_LOG;
		abort();
	} else if (MAP_FAILED == imgMap[0]) {
		LINE_LOG;
		abort();
	} else if (MAP_FAILED == imgMap[1]) {
		LINE_LOG;
		abort();
	}
}
