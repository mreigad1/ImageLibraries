#pragma once
#include "debug.hpp"
#include "Array2D.hpp"
#include "mask.h"
#include "pixel.hpp"
#include "colorModelEnums.hpp"
#include "colorModelTypes.hpp"
#include "arithmeticalRGB.h"
#include "pixelRGB.h"
#include <vector>
#include <algorithm>
#include <typeinfo>

#ifndef IG_CODE_DEF
	struct coordinate {
		unsigned int x;
		unsigned int y;

		coordinate(unsigned int _x, unsigned int _y) : 
			x(_x),
			y(_y) {
		}

		coordinate(const coordinate& other) :
			x(other.x),
			y(other.y) {
		}

		coordinate& operator=(const coordinate& other) {
			x = other.x;
			y = other.y;
			return *this;
		}
	};
#endif
#define IG_CODE_DEF

//imageGrid stores a copy of an imageGrid to be operated upon and allows copying
//back to imageGrid buffer following operations
//implements pixels buffer with pixel implementation T
template <typename T> class imageGrid {
	private:
		unsigned int h;
		unsigned int w;
		Array2D<T> img;

	public:
		imageGrid(unsigned height, unsigned width, T* old_data) :
			h(height),
			w(width),
			img(h, w, old_data) {
		}

		imageGrid() :
			h(0),
			w(0),
			img() {
		}

		~imageGrid() {
		}

		imageGrid& operator=(const imageGrid& other) {
			h = other.h;
			w = other.w;
			img = other.img;
			return *this;
		}

		void commitImageGrid(T* old_data) {
			//#pragma GCC diagnostic push
			//#pragma GCC diagnostic ignored "-Wuninitialized"
			img.exportContents(old_data);
			//#pragma GCC diagnostic pop
		}

		void toNegative() {
			//iterate over all pixels
			for (unsigned int i = 0; i < h; i++) {
				for (unsigned int j = 0; j < w; j++) {
					img[i][j] = img[i][j].toNegative();
				}
			}
		}

		unsigned int Height() const {
			return h;
		}

		unsigned int Width() const {
			return w;
		}

		T& getPixel(const unsigned int x, const unsigned int y) const {
			return img[y][x];
		}

		void toGrey() {
			for (unsigned int i = 0; i < h; i++){
				for (unsigned int j = 0; j < w; j++) {
					img[i][j] = img[i][j].toGrey();
				}
			}
		}

		void medianFilter() {
			//for each pixel
			for (unsigned int i = 0; i < h; i++){
				for (unsigned int j = 0; j < w; j++) {

					//list of neighbor intensities
					std::vector<byte> neighbors;

					//for each neighbor of pixel (including self)
					for (int ni = -1; ni <= 1; ni++) {
						for (int nj = -1; nj <= 1; nj++) {
							int abs_ni = i - ni;
							int abs_nj = j - nj;

							//if valid neighbor
							if (abs(ni) + abs(nj) < 2) {
								if (0 <= abs_ni && 0 <= abs_nj && abs_ni < (int)h && abs_nj < (int)w) {
									//push to list
									neighbors.push_back(img[abs_ni][abs_nj].getAvgIntensity());
								}
							}
						}
					}

					//sort list
					std::sort(neighbors.begin(), neighbors.end());

					//take median
					byte newVal = neighbors[neighbors.size() / 2];

					//assign new value
					img[i][j] = T(newVal, newVal, newVal);
				}
			}
		}

		void darkMedianFilter() {
			//for each pixel
			for (unsigned int i = 0; i < h; i++){
				for (unsigned int j = 0; j < w; j++) {

					//if pixel is near 0 intensity
					if (img[i][j].getAvgIntensity() < 15) {

						//list of neighbor intensities
						std::vector<byte> neighbors;

						//for each neighbor of pixel (including self)
						const int dist = 2;
						for (int ni = -dist; ni <= dist; ni++) {
							for (int nj = -dist; nj <= dist; nj++) {
								int abs_ni = i - ni;
								int abs_nj = j - nj;

								//if valid neighbor
								if (0 <= abs_ni && 0 <= abs_nj && abs_ni < (int)h && abs_nj < (int)w) {
									//push to list
									neighbors.push_back(img[abs_ni][abs_nj].getAvgIntensity());
								}
							}
						}

						//sort list
						std::sort(neighbors.begin(), neighbors.end());

						//take median
						byte newVal = neighbors[neighbors.size() / 2];

						//assign new value
						img[i][j] = T(newVal, newVal, newVal);
					}
				}
			}
		}

		std::vector<std::vector<coordinate>> clusterImage(const T& thresh) {
			std::vector<std::vector<coordinate>> allClusters;
			Array2D<bool> traversed(h, w);
			traversed.unsafe_erase();

			for (unsigned int i = 0; i < h; i++) {
				for (unsigned int j = 0; j < w; j++) {

					//don't cluster "black" pixels
					if (img[i][j] > thresh) {
						auto cl = memberCluster(j, i, traversed);

						const unsigned int arbitrarySizeThresh = std::min(h,w) / 4;

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
			//std::cout << "Size of vector: " << allClusters.size() << std::endl;

			return allClusters;
		}

		void assignment1Coloring(std::vector<std::vector<coordinate>>& clusters) {
			ASSERT(clusters.size() > 2);

			//int i = 0;
			for (auto cluster : clusters) {
				for (auto point : cluster) {
					img[point.y][point.x] = T(0,0,0);
				}
				//std::cout << "size of cluster " << i++ << ":\t" << cluster.size() << std::endl;
			}

			for (auto biggest : clusters.front()) {
				img[biggest.y][biggest.x] = T(0xFF, 0, 0);
			}

			for (auto medium : clusters[clusters.size() / 2]) {
				img[medium.y][medium.x] = T(0, 0xFF, 0);
			}

			for (auto smallest : clusters.back()) {
				img[smallest.y][smallest.x] = T(0, 0, 0xFF);
			}
		}

		void multiply(const mask& _mask) {
			imageGrid buf = *this;
			//iterate over all pixels
			for (unsigned int i = 0; i < h; i++) {
				for (unsigned int j = 0; j < w; j++) {
					buf.img[i][j] = multiplyPixel(i, j, _mask);
				}
			}
			*this = buf;
		}

		// void dilate(mask& _mask) {
		// 	imageGrid buf = *this;
		// 	//iterate over all pixels
		// 	for (unsigned int i = 0; i < h; i++) {
		// 		for (unsigned int j = 0; j < w; j++) {
		// 			buf.img[i][j] = dilatePixel(i, j, _mask);
		// 		}
		// 	}
		// 	*this = buf;
		// }

		// void dilateBinary(mask& _mask) {
		// 	imageGrid buf = *this;
		// 	//iterate over all pixels
		// 	for (unsigned int i = 0; i < h; i++) {
		// 		for (unsigned int j = 0; j < w; j++) {
		// 			buf.img[i][j] = dilatePixelBinary(i, j, _mask);
		// 		}
		// 	}
		// 	*this = buf;
		// }

		// void erode(mask& _mask) {
		// 	imageGrid buf = *this;
		// 	//iterate over all pixels
		// 	for (unsigned int i = 0; i < h; i++) {
		// 		for (unsigned int j = 0; j < w; j++) {
		// 			buf.img[i][j] = erodePixel(i, j, _mask);
		// 		}
		// 	}
		// 	*this = buf;
		// }

		// void erodeBinary(mask& _mask) {
		// 	imageGrid buf = *this;
		// 	//iterate over all pixels
		// 	for (unsigned int i = 0; i < h; i++) {
		// 		for (unsigned int j = 0; j < w; j++) {
		// 			buf.img[i][j] = erodePixelBinary(i, j, _mask);
		// 		}
		// 	}
		// 	*this = buf;
		// }

		void sobel() {

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

			mask sobelX(3, sob_x);
			mask sobelY(3, sob_y);

			imageGrid gx;
			imageGrid gy;
			gx = *this;
			gy = *this;

			gx.multiply(sobelX);
			gy.multiply(sobelY);

			for (unsigned int i = 0; i < h; i++) {
				for (unsigned int j = 0; j < w; j++) {
					img[i][j] = (gx.img[i][j] * gx.img[i][j] + gy.img[i][j] * gy.img[i][j]).root();
				}
			}
		}

		// pixel<T> dilatePixel(unsigned int y, unsigned int x, mask& _mask) {
		// 	pixel<T> retVal(0, 0, 0);
		// 	int off = _mask.w / 2;
		// 	//iterate over all valid pixels in neighborhood
		// 	for (int y_off = -off; y_off <= off; y_off++) {
		// 		int y_prime = y + y_off;
		// 		for (int x_off = -off; x_off <= off; x_off++) {
		// 			int x_prime = x + x_off;
		// 			if (x_prime >= 0 && x_prime < w && y_prime >= 0 && y_prime < h) {
		// 				pixel<T> tempPix = img[y_prime][x_prime].toGrey() + _mask.maskVals[y_off + off][x_off + off];
		// 				retVal = ((retVal > tempPix) ? retVal : tempPix);
		// 			} 
		// 		}
		// 	}
		// 	return retVal;
		// }

		// pixel<T> dilatePixelBinary(unsigned int y, unsigned int x, mask& _mask) {
		// 	pixel<T> retVal(0, 0, 0);
		// 	int off = _mask.w / 2;
		// 	//iterate over all valid pixels in neighborhood
		// 	for (int y_off = -off; y_off <= off; y_off++) {
		// 		int y_prime = y + y_off;
		// 		for (int x_off = -off; x_off <= off; x_off++) {
		// 			int x_prime = x + x_off;
		// 			if (x_prime >= 0 && x_prime < w && y_prime >= 0 && y_prime < h) {
		// 				int enable = ((_mask.maskVals[y_off + off][x_off + off] != 0.0) ? 1 : 0);
		// 				pixel<T> tempPix = img[y_prime][x_prime].toBinary() * enable;
		// 				retVal = ((tempPix.getAvgIntensity() > 0) ? tempPix : retVal);
		// 			} 
		// 		}
		// 	}
		// 	return retVal;
		// }

		// pixel<T> erodePixel(unsigned int y, unsigned int x, mask& _mask) {
		// 	pixel<T> retVal(MAX_COLOR, MAX_COLOR, MAX_COLOR);
		// 	int off = _mask.w / 2;
		// 	//iterate over all valid pixels in neighborhood
		// 	for (int y_off = -off; y_off <= off; y_off++) {
		// 		int y_prime = y + y_off;
		// 		for (int x_off = -off; x_off <= off; x_off++) {
		// 			int x_prime = x + x_off;
		// 			if (x_prime >= 0 && x_prime < w && y_prime >= 0 && y_prime < h) {
		// 				pixel<T> tempPix = img[y_prime][x_prime].toGrey() + (-_mask.maskVals[y_off + off][x_off + off]);
		// 				retVal = ((retVal < tempPix) ? retVal : tempPix);
		// 			} 
		// 		}
		// 	}
		// 	return retVal;
		// }

		// pixel<T> erodePixelBinary(unsigned int y, unsigned int x, mask& _mask) {
		// 	pixel<T> retVal(MAX_COLOR, MAX_COLOR, MAX_COLOR);
		// 	int off = _mask.w / 2;
		// 	//iterate over all valid pixels in neighborhood
		// 	for (int y_off = -off; y_off <= off; y_off++) {
		// 		int y_prime = y + y_off;
		// 		for (int x_off = -off; x_off <= off; x_off++) {
		// 			int x_prime = x + x_off;
		// 			if (x_prime >= 0 && x_prime < w && y_prime >= 0 && y_prime < h) {
		// 				int enable = ((_mask.maskVals[y_off + off][x_off + off] != 0.0) ? 1 : 0);
		// 				if (enable) {
		// 					pixel<T> tempPix = img[y_prime][x_prime].toBinary();
		// 					retVal = ((tempPix.getAvgIntensity() > 0 && retVal.getAvgIntensity() > 0) ? retVal : pixel<T>(0,0,0));
		// 				}
		// 			} 
		// 		}
		// 	}
		// 	return retVal;
		// }

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

		void toBinary(T thresh) {
			for (unsigned int i = 0; i < h; i++){
				for (unsigned int j = 0; j < w; j++) {
					img[i][j] = img[i][j].toBinary(thresh);
				}
			}
		}

	private:

		static bool clusterCompare(const std::vector<coordinate>& l, const std::vector<coordinate> r) {
			//sort largest to smallest
			return l.size() > r.size();
		}

		std::vector<coordinate> memberCluster(unsigned int x, unsigned int y, Array2D<bool>& traversed) {
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

		T multiplyPixel(unsigned int y, unsigned int x, const mask& _mask) const {
			T retVal(0, 0, 0);
			auto accum = retVal + 0;
			int off = _mask.Width() / 2;
			//iterate over all valid pixels in neighborhood
			for (int y_off = -off; y_off <= off; y_off++) {
				int y_prime = y + y_off;
				for (int x_off = -off; x_off <= off; x_off++) {
					int x_prime = x + x_off;
					if (x_prime >= 0 && x_prime < static_cast<int>(w) && y_prime >= 0 && y_prime < static_cast<int>(h)) {
						accum = accum + img[y_prime][x_prime] * _mask.getMaskData()[y_off + off][x_off + off];
					} else {
						accum = accum + img[y][x] * _mask.getMaskData()[y_off + off][x_off + off];
					}
				}
			}
			retVal = accum;
			return retVal;
		}
};