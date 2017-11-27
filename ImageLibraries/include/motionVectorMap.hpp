#include "debug.hpp"
#include "constants.hpp"
#include "constants.hpp"
#include "imageGrid.hpp"
#include "mask.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <functional>


//class is designed to process an entire image for a single vector
//image will need to be broken into subimages before processing for
//many different motion vectors
template <typename PixType> class motionVectorMap {
	//mask will contain 0's for non-members, will contain positive ID's for neighbors
	const mask& neighborhood; 
	const imageGrid<PixType>& I0;
	const imageGrid<PixType>& I1;
	imageGrid<unsigned> imageNeighbors;

	public:

		//forces user context to make only temporary objects, processing directives to be received
		//and state to be maintained internally only
		static motionVectorMap create(const imageGrid<PixType> img0, const imageGrid<PixType> img1, const mask neighbors) {
			return motionVectorMap(const imageGrid<PixType> img0, const imageGrid<PixType> img1, const mask neighbors);
		}



	private:
		//No empty constructor use, just no
		motionVectorMap() {
		}

		motionVectorMap(const imageGrid<PixType> img0, const imageGrid<PixType> img1, const mask neighbors) :
			I0(img0),
			I1(img1),
			neighborhood(neighbors) {
		}
};

