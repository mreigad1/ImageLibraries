#include "debug.hpp"
#include "constants.hpp"
#include "constants.hpp"
#include "imageGrid.hpp"
#include "coordinate.h"
#include "mask.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <vector>
#include <map>

bool sortByX(coordinate c1, coordinate c2) {
	return c1.x < c2.x;
}

bool sortByY(coordinate c1, coordinate c2) {
	return c1.y < c2.y;
}

//class is designed to process an entire image for a single vector
//image will need to be broken into subimages before processing for
//many different motion vectors
template <typename PixType> class motionVectorMap {
	//mask will contain 0's for non-members, will contain positive ID's for neighbors
	const mask& neighborhood; 
	const imageGrid<PixType>& I0;
	const imageGrid<PixType>& I1;
	Array2D<coordinate> bestNeighbors;

	public:
		static Array2D<coordinate> motionEstimation(const imageGrid<PixType>& img0, const imageGrid<PixType>& img1, const mask& neighbors, const int blockWidth) {
			//get the windowed CDF of each frame
			auto cdf0 = img0;//.windowedCDF(blockWidth);
			auto cdf1 = img1;//.windowedCDF(blockWidth);

			//get the image of translation vectors
			auto translationImage = process(cdf0, cdf1, neighbors);

			//for each sub block in image
			for (unsigned int i = 0; i < translationImage.Height(); i += blockWidth) {
				for (unsigned int j = 0; j < translationImage.Width(); j += blockWidth) {

					//get container of coordinate translations
					std::vector<coordinate> blockCoords;
					for (int sub_i = 0; sub_i < blockWidth && (i + sub_i) < translationImage.Height(); sub_i++) {
						for (int sub_j = 0; sub_j < blockWidth && (j + sub_j) < translationImage.Width(); sub_j++) {
							blockCoords.push_back(translationImage[i + sub_i][j + sub_j]);
						}
					}

					//get median change vector
					const int mdPt = blockCoords.size() / 2;
					std::sort(blockCoords.begin(), blockCoords.end(), sortByX);
					const int med_x = blockCoords[mdPt].x;
					std::sort(blockCoords.begin(), blockCoords.end(), sortByY);
					const int med_y = blockCoords[mdPt].y;

					coordinate medianVec(med_x, med_y);
					auto& cd = medianVec;
					std::cout << "(" << cd.x << "," << cd.y << ")";

					//overwrite block vector with median translation vector
					for (int sub_i = 0; sub_i < blockWidth && (i + sub_i) < translationImage.Height(); sub_i++) {
						for (int sub_j = 0; sub_j < blockWidth && (j + sub_j) < translationImage.Width(); sub_j++) {
							translationImage[i + sub_i][j + sub_j] = medianVec;
						}
					}
				}

				std::cout << std::endl << std::endl;
			}
			return translationImage;
		}


	private:
		//No empty constructor use, just no
		motionVectorMap() {
		}

		motionVectorMap(const imageGrid<PixType>& img0, const imageGrid<PixType>& img1, const mask& neighbors) :
			neighborhood(neighbors),
			I0(img0),
			I1(img1),
			bestNeighbors(I0.Height(), I0.Width()) {
		}

		//forces user context to make only temporary objects, processing directives to be received
		//and state to be maintained internally only
		static Array2D<coordinate> process(const imageGrid<PixType>& img0, const imageGrid<PixType>& img1, const mask& neighbors) {
			return motionVectorMap<PixType>(img0, img1, neighbors).getCoordTranslations();
		}

		coordinate setCoord(unsigned int y, unsigned int x) const {
			const int off = neighborhood.Width() / 2;

			PixType diff = I1.getPixel(y,x) - I0.getPixel(y,x);
			coordinate rv(0,0);

			//for each mask component
			for (int i = 0; i < (int)neighborhood.Width(); i++) {
				for (int j = 0; j < (int)neighborhood.Width(); j++) {

					//if valid mask component
					if (neighborhood.getMaskData()[i][j] != 0) {

						//get relative neighbor coordinates
						const int y_off = i - off;
						const int x_off = j - off;

						//get absolute neighbor coordinates
						const int y_prime = y + y_off;
						const int x_prime = x + x_off;

						//if pixel in image range
						if (y_prime >= 0 && x_prime >= 0 &&  y_prime < (int)I1.Height() && x_prime < (int)I1.Width()) {

							//take difference of this pixel and neighbor
							PixType tmpDiff = (I1.getPixel(y,x) - I0.getPixel(y_prime, x_prime));

							//if noted difference is smaller than candidate then replace candidate
							if (tmpDiff < diff) {
								diff = tmpDiff;
								rv = coordinate(x_off, y_off);
							}
						}
					}
				}
			}
			return rv;
		}

		//calculates each pixel's translation coordinates
		//then averages the coordinate translations within the image
		Array2D<coordinate> getCoordTranslations() const {
			for (unsigned int i = 0; i < bestNeighbors.Height(); i++) {
				for (unsigned int j = 0; j < bestNeighbors.Width(); j++) {
					bestNeighbors[i][j] = setCoord(i, j);
				}
			}

			return bestNeighbors;
		}
};

