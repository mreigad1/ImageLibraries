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
		//forces user context to make only temporary objects, processing directives to be received
		//and state to be maintained internally only
		static coordinate process(const imageGrid<PixType>& img0, const imageGrid<PixType>& img1, const mask& neighbors) {
			return motionVectorMap<PixType>(img0, img1, neighbors).getAverageMotionVector();
		}


	private:
		//No empty constructor use, just no
		motionVectorMap() {
		}

		motionVectorMap(const imageGrid<PixType>& img0, const imageGrid<PixType>& img1, const mask& neighbors) :
			neighborhood(neighbors),
			I0(img0),
			I1(img1),
			bestNeighbors(img0.Height(), img0.Width()) {
		}

		coordinate setCoord(unsigned int y, unsigned int x) const {
			const unsigned int off = neighborhood.Width() / 2;

			decltype(I0.getPixel(0,0).Arithmetical()) diff = I0.getPixel(y,x) - I1.getPixel(y,x);
			coordinate rv(0,0);

			//for each mask component
			for (unsigned int i = 0; i < neighborhood.Width(); i++) {
				for (unsigned int j = 0; j < neighborhood.Width(); j++) {

					//if valid mask component
					if (neighborhood.getMaskData()[i][j] != 0) {
						const unsigned int i_prime = i - off;
						const unsigned int j_prime = j - off;

						//if pixel in image range
						if (i_prime < I0.Height() && j_prime < I0.Width()) {

							decltype(I0.getPixel(0,0).Arithmetical()) tmpDiff = (I0.getPixel(y,x) - I1.getPixel(i_prime, j_prime));

							if (tmpDiff <= diff) {
								diff = tmpDiff;
								rv = coordinate(j, i);
							}
						}
					}
				}
			}
			return rv;
		}

		//calculates each pixel's translation coordinates
		//then averages the coordinate translations within the image
		coordinate getAverageMotionVector() const {
			long long x = 0;
			long long y = 0;
			unsigned long long size = bestNeighbors.Height() * bestNeighbors.Width();
			for (unsigned int i = 0; i < bestNeighbors.Height(); i++) {
				for (unsigned int j = 0; j < bestNeighbors.Width(); j++) {
					bestNeighbors[i][j] = setCoord(i, j);
					x += bestNeighbors[i][j].x;
					y += bestNeighbors[i][j].y;
				}
			}
			x = static_cast<long long>((static_cast<double>(x) / size) + 0.5);
			y = static_cast<long long>((static_cast<double>(y) / size) + 0.5);
			return coordinate(x, y);
		}
};

