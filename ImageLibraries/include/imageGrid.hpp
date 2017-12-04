#pragma once
#include "debug.hpp"
#include "Array2D.hpp"
#include "mask.h"
#include "pixel.hpp"
#include "colorModelEnums.hpp"
#include "colorModelTypes.hpp"
#include "coordinate.h"
#include <vector>
#include <algorithm>
#include <functional>
#include <typeinfo>

//imageGrid stores a copy of an imageGrid to be operated upon and allows copying
//back to imageGrid buffer following operations
//implements pixels buffer with pixel implementation T
template <typename T> class imageGrid {
	protected:
		unsigned int h;
		unsigned int w;
		Array2D<T> img;

	public:
		imageGrid(unsigned height, unsigned width, T* old_data) :
			h(height),
			w(width),
			img(h, w, old_data) {
		}

		imageGrid(unsigned height, unsigned width) :
			h(height),
			w(width),
			img(h, w) {
		}

		imageGrid() :
			h(0),
			w(0),
			img() {
		}

		~imageGrid() {
		}

		//constructor for converting color types
		template <typename U>
		imageGrid(const imageGrid<U>& other) :
			h(other.Height()),
			w(other.Width()),
			img(other.Height(), other.Width()) {
				for (unsigned int i = 0; i < h; i++) {
					for (unsigned int j = 0; j < w; j++) {
						img[i][j] = static_cast<decltype(img[i][j].Arithmetical())>(other.getPixel(i, j));
					}
				}
		}

		imageGrid& operator=(const imageGrid& other) {
			h = other.h;
			w = other.w;
			img = other.img;
			return *this;
		}

		unsigned int Height() const {
			return h;
		}

		unsigned int Width() const {
			return w;
		}

		T& getPixel(const unsigned int y, const unsigned int x) const {
			static T outOfBounds;
			if (x >= img.Width() || y >= img.Height()) {
				outOfBounds = T();	//always reset to empty pixel before returning
				return outOfBounds;
			}
			return img[y][x];
		}

		#define ARITH_TYPE decltype(T().Arithmetical())
			imageGrid<ARITH_TYPE> cdf() const {
				imageGrid<ARITH_TYPE> rv(*this);

				//sum right
				std::function<ARITH_TYPE(const unsigned int, const unsigned int)> cdf1 = 
					std::bind(
						&imageGrid<ARITH_TYPE>::sumRight,
						&rv,
						std::placeholders::_1,
						std::placeholders::_2
					);
				rv = rv.transformGrid(cdf1);

				//sum down
				std::function<ARITH_TYPE(const unsigned int, const unsigned int)> cdf2 = 
					std::bind(
						&imageGrid<ARITH_TYPE>::sumDown,
						&rv,
						std::placeholders::_1,
						std::placeholders::_2
					);
				rv = rv.transformGrid(cdf2);

				return rv;
			}

			//integrates toward the right
			T sumRight(const unsigned int y, const unsigned int x) const {
				 getPixel(y, x) = getPixel(y, x) + getPixel(y, x - 1); //state must be written to original copy for dynamic programming
				 return getPixel(y, x);
			}

			//integrates downward
			T sumDown(const unsigned int y, const unsigned int x) const {
				 getPixel(y, x) = getPixel(y, x) + getPixel(y - 1, x); //state must be written to original copy for dynamic programming
				 return getPixel(y, x);
			}

			T getMotionCompensated(const imageGrid<T>& previousFrame, const Array2D<coordinate>& translations, const unsigned y, const unsigned x) const {
				T rv = getPixel(y,x) - previousFrame.getPixel(y + translations[y][x].y, x + translations[y][x].x);
				return rv;
			}

			//takes p(y,x) - p(y,x-w) - p(y-w,x) +p(y-w,x-w) to get windowed integration
			T getWindowedCDF(const unsigned int y, const unsigned x, const int windowWidth) const {
				T rv = getPixel(y, x) - (getPixel(y, x - windowWidth) + getPixel(y - windowWidth, x) - getPixel(y - windowWidth, x - windowWidth));
				return rv;
			}

			imageGrid<ARITH_TYPE> windowedCDF(const int windowWidth) const {
				imageGrid<ARITH_TYPE> rv = this->cdf();
				std::function<ARITH_TYPE(const unsigned int, const unsigned int)> windCDF = 
					std::bind(
						&imageGrid<ARITH_TYPE>::getWindowedCDF,
						&rv,
						std::placeholders::_1,
						std::placeholders::_2,
						windowWidth
					);
				rv = rv.transformGrid(windCDF);
				return rv;
			}
		#undef ARITH_TYPE

		imageGrid<T> motionCompensated(const imageGrid<T>& previousFrame, const Array2D<coordinate>& translations) const {
				std::function<T(const unsigned int, const unsigned int)> motionCompensator = 
					std::bind(
						&imageGrid<T>::getMotionCompensated,
						this,
						previousFrame,
						translations,
						std::placeholders::_1,
						std::placeholders::_2
					);
				return transformGrid(motionCompensator);
		}

		Array2D<imageGrid<T>> subImages(const unsigned width) const {
			const unsigned heightRemainder = Height() % width;
			const unsigned widthRemainder =  Width() % width;

			const unsigned sliceHeight = (Height() / width) + (heightRemainder ? 1 : 0);
			const unsigned  sliceWidth = (Width() / width)  + ( widthRemainder ? 1 : 0);

			Array2D<imageGrid<T>> rv(sliceHeight, sliceWidth);

			//for each sliced subimage tile
			for (unsigned int i = 0; i < sliceHeight; i++) {
				const unsigned tileHeightOffset = i * width;
				const unsigned tileHeight = (((i + 1) == sliceHeight && heightRemainder) ? heightRemainder : width);
				for (unsigned int j = 0; j < sliceWidth; j++) {
					const unsigned tileWidthOffset = j * width;
					const unsigned tileWidth  = (((j + 1) == sliceWidth && widthRemainder) ? widthRemainder : width);

					//size-out the subimage tile
					rv[i][j] = imageGrid<T>(tileHeight, tileWidth);

					//copy each pixel into subimage
					for (unsigned int internal_y = 0; internal_y < tileHeight; internal_y++) {
						for (unsigned int internal_x = 0; internal_x < tileWidth; internal_x++) {
							rv[i][j].getPixel(internal_y, internal_x) = getPixel(internal_y + tileHeightOffset, internal_x + tileWidthOffset);
						}
					}
				}
			}

			return rv;
		}

		//will write failure to bool if non-null pointer and if subimage grid size-fails
		static imageGrid<T> fromSubImageGrid(const Array2D<imageGrid<T>>& imageMatrix, bool* success = NULL) {
			bool checkDimensions = true;
			unsigned int totalHeight = imageMatrix[0][0].Height();
			unsigned int totalWidth = imageMatrix[0][0].Width();

			for (unsigned int i = 1; i < imageMatrix.Height(); i++) {
				totalHeight += imageMatrix[i][0].Height();
				totalWidth = imageMatrix[i][0].Width();

				for (unsigned int j = 1; j < imageMatrix.Width(); j++) {
					totalWidth += imageMatrix[0][j].Width();
					if (imageMatrix[i][j].Width() != imageMatrix[i-1][j].Width()) {
						checkDimensions = false;
					}
					if (imageMatrix[i][j].Height() != imageMatrix[i][j-1].Height()) {
						checkDimensions = false;
					}
				}
			}

			if (false == checkDimensions) {
				if (NULL != success) {
					*success = checkDimensions;
				}
				return imageGrid<T>();
			}

			imageGrid<T> rv(totalHeight, totalWidth);

			//inspect block dimensions
			const unsigned int blockHeight = imageMatrix[0][0].Height();
			const unsigned int blockWidth = imageMatrix[0][0].Width();

			//for each block
			for (unsigned int i = 0; i < imageMatrix.Height(); i++) {
				const unsigned tileHeightOffset = i * blockHeight;
				for (unsigned int j = 0; j < imageMatrix.Width(); j++) {
					const unsigned tileWidthOffset = j * blockWidth;

					//for each pixel in block
					for (unsigned int internal_y = 0; internal_y < imageMatrix[i][j].Height(); internal_y++) {
						for (unsigned int internal_x = 0; internal_x < imageMatrix[i][j].Width(); internal_x++) {

							//copy pixel contents into rv
							rv.getPixel(tileHeightOffset + internal_y, tileWidthOffset + internal_x) = imageMatrix[i][j].getPixel(internal_y, internal_x);
						}
					}
				}
			}

			if (NULL != success) {
				*success = true;
			}

			return rv;
		}

		imageGrid toMotionDisplay(const coordinate& motionVector) const {
			std::function<T(const unsigned int, const unsigned int)> func = 
				std::bind(
					&imageGrid<T>::scaleByVector,
					this,
					std::placeholders::_1,
					std::placeholders::_2,
					motionVector
				);
			return transformGrid(func);
		}

		void commitImageGrid(T* old_data) {
			img.exportContents(old_data);
		}

		//parent context must bind closure other than coordinates
		imageGrid transformGrid(std::function<T(const unsigned int, const unsigned int)> func) const {
			imageGrid retVal = (*this);
			for (unsigned int y = 0; y < Height(); y++) {
				for (unsigned int x = 0; x < Width(); x++) {
					retVal.getPixel(y, x) = func(y, x);
				}
			}
			return retVal;
		}

		imageGrid operator+(const imageGrid& other) const {
			std::function<T(const unsigned int, const unsigned int)> func = 
				std::bind(
					&imageGrid<T>::addPixels,
					this,
					std::placeholders::_1,
					std::placeholders::_2,
					other
				);
			return transformGrid(func);
		}

		imageGrid operator-(const imageGrid& other) const {
			std::function<T(const unsigned int, const unsigned int)> func = 
				std::bind(
					&imageGrid<T>::subtractPixels,
					this,
					std::placeholders::_1,
					std::placeholders::_2,
					other
				);
			return transformGrid(func);
		}

		imageGrid toNegative() const {
			std::function<T(const unsigned int, const unsigned int)> func = 
				std::bind(
					&imageGrid<T>::toNegativePixel,
					this,
					std::placeholders::_1,
					std::placeholders::_2
				);
			return transformGrid(func);
		}

		imageGrid toGrey() const {
			std::function<T(const unsigned int, const unsigned int)> func = 
				std::bind(
					&imageGrid<T>::toGreyPixel,
					this,
					std::placeholders::_1,
					std::placeholders::_2
				);
			return transformGrid(func);
		}

		std::vector<std::vector<coordinate>> clusterImage(const T& thresh) const {
			std::vector<std::vector<coordinate>> allClusters;
			Array2D<bool> traversed(h, w);
			traversed.unsafe_erase();

			for (unsigned int i = 0; i < h; i++) {
				for (unsigned int j = 0; j < w; j++) {

					//don't cluster "black" pixels
					if (img[i][j] > thresh) {
						auto cl = memberCluster(j, i, traversed);

						const unsigned int arbitrarySizeThresh = (h * w) / 512.0;

						//store new cluster to structure
						if (cl.size() <= arbitrarySizeThresh) {
							for (auto point : cl) {
								img[point.y][point.x] = T(0,0,0);
							}
						} else {
							allClusters.push_back(cl);

							//retrieve entire cluster for current pixel
							std::vector<coordinate>& coords = allClusters.back();

							//for each item in cluster
							for (unsigned int k = 0; k < coords.size(); k++) {
								//coordHash could be static and doesn't truly need object,
								//but CRTP to force implementation requires concrete
								img[coords[k].y][coords[k].x] = img[i][j].coordHash(j,i);
							}
						}
					}
				}
			}

			std::sort(allClusters.begin(), allClusters.end(), clusterCompare);
			return allClusters;
		}

		imageGrid assignment1Coloring(std::vector<std::vector<coordinate>>& clusters) const {
			ASSERT(clusters.size() > 2);
			imageGrid rv = *this;

			//int i = 0;
			for (auto cluster : clusters) {
				for (auto point : cluster) {
					rv.img[point.y][point.x] = T(0,0,0);
				}
			}

			for (auto biggest : clusters.front()) {
				rv.img[biggest.y][biggest.x] = T(0xFF, 0, 0);
			}

			for (auto medium : clusters[clusters.size() / 2]) {
				rv.img[medium.y][medium.x] = T(0, 0xFF, 0);
			}

			for (auto smallest : clusters.back()) {
				rv.img[smallest.y][smallest.x] = T(0, 0, 0xFF);
			}

			return rv;
		}

		imageGrid assignment3Coloring(std::vector<std::vector<coordinate>>& clusters, imageGrid original) const {
			imageGrid rv = imageGrid(original.Height(), original.Width());
			ASSERT(clusters.size() > 2);

			//int i = 0;
			for (auto cluster : clusters) {
				for (auto point : cluster) {
					if (!(static_cast<T>(img[point.y][point.x] * 0) == img[point.y][point.x])) {
						rv.img[point.y][point.x] = original.img[point.y][point.x];
					}
				}
			}
			return rv;
		}

		imageGrid multiplyByMask(const mask& _mask) const{
			std::function<T(const unsigned int, const unsigned int)> func = 
				std::bind(
					&imageGrid<T>::multiplyPixelByMask, 
					this,
					std::placeholders::_1,
					std::placeholders::_2,
					_mask
				);
			return transformGrid(func);
		}

		imageGrid dilate(const mask& _mask) const {
			std::function<T(const unsigned int, const unsigned int)> func = 
				std::bind(
					&imageGrid<T>::dilatePixel, 
					this,
					std::placeholders::_1,
					std::placeholders::_2,
					_mask
				);
			return transformGrid(func);
		}

		imageGrid erode(const mask& _mask) const {
			std::function<T(const unsigned int, const unsigned int)> func = 
				std::bind(
					&imageGrid<T>::erodePixel,
					this,
					std::placeholders::_1,
					std::placeholders::_2,
					_mask
				);
			return transformGrid(func);
		}

		imageGrid sobel() const {
			PrecisionType sob_x[] = {
				-1,  0,  1,
				-2,  0,  2,
				-1,  0,  1
			};

			PrecisionType sob_y[] = {
				-1, -2, -1,
				 0,  0,  0,
				 1,  2,  1
			};

			imageGrid gx = this->multiplyByMask(mask(3, sob_x));
			imageGrid gy = this->multiplyByMask(mask(3, sob_y));
			std::function<T(const unsigned int, const unsigned int)> func = 
				std::bind(
					&imageGrid<T>::sobelOperation, 
					gx,
					gy,
					std::placeholders::_1,
					std::placeholders::_2
				);
			return transformGrid(func);
		}

		imageGrid toBinary(const T& thresh) const {
			std::function<T(const unsigned int, const unsigned int)> func = 
				std::bind(
					&imageGrid::toBinaryPixel,
					this,
					std::placeholders::_1,
					std::placeholders::_2,
					thresh
				);
			return transformGrid(func);
		}

		// //splices two images together with weight factor "other_ratio"
		// void mixWith(imageGrid& other, double other_ratio) {
		// 	int min_w = MIN(other.w, w);
		// 	int min_h = MIN(other.h, h);

		// 	for (unsigned int i = 0; i < min_h; i++) {
		// 		for (unsigned int j = 0; j < min_w; j++) {
		// 			img[i][j] = (img[i][j] * (1 - other_ratio) + other.img[i][j] * other_ratio);
		// 		}
		// 	}
		// }

		// //splices two images together by taking geometric pixel averages
		// void geometricMix(imageGrid& other) {
		// 	int min_w = MIN(other.w, w);
		// 	int min_h = MIN(other.h, h);

		// 	for (unsigned int i = 0; i < min_h; i++) {
		// 		for (unsigned int j = 0; j < min_w; j++) {
		// 			img[i][j] = (img[i][j] * other.img[i][j]).root();
		// 		}
		// 	}
		// }

		// //continuous color lux rotation implementation
		// void continuousLux(double p) {
		// 	for (unsigned int i = 0; i < h; i++) {
		// 		for (unsigned int j = 0; j < w; j++) {
		// 			img[i][j] = img[i][j].continuousLux(p);
		// 		}
		// 	}
		// }

		// //single color lux inversion
		// void lux() {
		// 	for (unsigned int i = 0; i < h; i++){
		// 		for (unsigned int j = 0; j < w; j++) {
		// 			img[i][j] = img[i][j].lux();
		// 		}
		// 	}
		// }

		// //reverse lux inversion
		// void unlux() {
		// 	for (unsigned int i = 0; i < h; i++){
		// 		for (unsigned int j = 0; j < w; j++) {
		// 			img[i][j] = img[i][j].unlux();
		// 		}
		// 	}
		// }

	private:
		static bool clusterCompare(const std::vector<coordinate>& l, const std::vector<coordinate> r) {
			return l.size() > r.size();
		}

		std::vector<coordinate> memberCluster(unsigned int x, unsigned int y, Array2D<bool>& traversed) const {
			std::vector<coordinate> retVals;
			const byte clusteringThreshold = 10;

			//only return a populated list if untraversed
			if (false == traversed[y][x]){
				retVals.push_back(coordinate(x, y));
				traversed[y][x] = true;
			}

			for (unsigned int k = 0; k < retVals.size(); k++) {
				x = retVals[k].x;
				y = retVals[k].y;

				//for each neighbor
				for(int ni = -1; ni <= 1; ni++) {
					for (int nj = -1; nj <= 1 ; nj++) {

						//if valid neighbor
						if (abs(ni) + abs(nj) == 1) {
							int abs_ni = y + ni;
							int abs_nj = x + nj;
							if (0 <= abs_ni && 0 <= abs_nj && abs_ni < (int)h && abs_nj < (int)w) {
								//if neighbor not yet traversed
								if (false == traversed[abs_ni][abs_nj]) {

									//get difference between inspected pixel and its neighbor
									byte brighter = std::max(img[y][x].getAvgIntensity(), img[abs_ni][abs_nj].getAvgIntensity());
									byte darker = std::min(img[y][x].getAvgIntensity(), img[abs_ni][abs_nj].getAvgIntensity());
									byte diff = brighter - darker;

									//if difference sufficiently small then push to cluster and mark traversed
									if (diff < clusteringThreshold) {
										retVals.push_back(coordinate(abs_nj, abs_ni));
										traversed[abs_ni][abs_nj] = true;
									}
								}
							}
						}
					}
				}
			}

			return retVals;
		}

		T multiplyPixelByMask(const unsigned int y, const unsigned int x, const mask& _mask) const {
			static T dummy;
			decltype(dummy.Arithmetical()) accum = T(this->img[y][x]) * 0;
			int off = _mask.Width() / 2;
			//iterate over all valid pixels in neighborhood
			for (int y_off = -off; y_off <= off; y_off++) {
				int y_prime = y + y_off;
				for (int x_off = -off; x_off <= off; x_off++) {
					int x_prime = x + x_off;
					if (_mask.getMaskData()[y_off + off][x_off + off] != 0) {
						if (x_prime >= 0 && x_prime < static_cast<int>(w) && y_prime >= 0 && y_prime < static_cast<int>(h)) {
							accum = accum + img[y_prime][x_prime] * _mask.getMaskData()[y_off + off][x_off + off];
						} else {
							accum = accum + img[y][x] * _mask.getMaskData()[y_off + off][x_off + off];
						}
					}
				}
			}
			return accum;
		}

		T addPixels(const unsigned int y, const unsigned int x, const imageGrid& other) const {
			static const T zeroPixel;
			const T& otherPix = (y < other.Height() && x < other.Width()) ? other.img[y][x] : zeroPixel;
			return T(img[y][x] + otherPix);
		}

		T subtractPixels(const unsigned int y, const unsigned int x, const imageGrid& other) const {
			static const T zeroPixel;
			const T& otherPix = (y < other.Height() && x < other.Width()) ? other.img[y][x] : zeroPixel;
			return T(img[y][x] - otherPix);
		}

		T toNegativePixel(const unsigned int y, const unsigned int x) const {
			return img[y][x].toNegative();
		}

		T toGreyPixel(const unsigned int y, const unsigned int x) const {
			return img[y][x].toGrey();
		}

		T sobelOperation(const imageGrid& other, const unsigned int y, const unsigned int x) const {
			return T((img[y][x] * img[y][x] + other.img[y][x] * other.img[y][x]).root());
		}

		T toBinaryPixel(const unsigned int y, const unsigned int x, const T& thresh) const {
			return img[y][x].toBinary(thresh);
		}

		T erodePixel(unsigned int y, unsigned int x, const mask& _mask) const {
			T retVal(MAX_BYTE, MAX_BYTE, MAX_BYTE);
			const int off = _mask.Width() / 2;

			//iterate over all valid pixels in neighborhood
			for (int y_off = -off; y_off <= off; y_off++) {
				const unsigned int y_prime = y + y_off;
				for (int x_off = -off; x_off <= off; x_off++) {
					const unsigned int x_prime = x + x_off;

					//if neighbor image
					if (x_prime < w && y_prime < h) {
						const PrecisionType b_xy = _mask.getMaskData()[y_off + off][x_off + off];
						const bool enable = (b_xy >= 0.0);
						
						if (enable) {
							T neighborIncremented = img[y_prime][x_prime] - b_xy;
							if (neighborIncremented < retVal) {
								retVal = neighborIncremented;
							}
						}
					} 
				}
			}
			return retVal;
		}

		T dilatePixel(unsigned int y, unsigned int x, const mask& _mask) const {
			T retVal(0, 0, 0);
			const int off = _mask.Width() / 2;

			//iterate over all valid pixels in neighborhood
			for (int y_off = -off; y_off <= off; y_off++) {
				const unsigned int y_prime = y + y_off;
				for (int x_off = -off; x_off <= off; x_off++) {
					const unsigned int x_prime = x + x_off;

					//if neighbor image
					if (x_prime < w && y_prime < h) {
						const PrecisionType b_xy = _mask.getMaskData()[y_off + off][x_off + off];
						const bool enable = (b_xy >= 0.0);
						
						if (enable) {
							T neighborIncremented = img[y_prime][x_prime] + b_xy;
							if (neighborIncremented > retVal) {
								retVal = neighborIncremented;
							}
						}
					} 
				}
			}
			return retVal;
		}

		T scaleByVector(unsigned int y, unsigned int x, const coordinate& motionVector) const {
			const int midPtY = Height() / 2;
			const int midPtX =  Width() / 2;
			const int delta_y = y - midPtY;
			const int delta_x = x - midPtX;
			PrecisionType scaleFactor = motionVector.x * delta_x + motionVector.y * delta_y;
			scaleFactor = (scaleFactor > 0) ? scaleFactor : 0;
			return getPixel(y,x) * scaleFactor;
		}
};