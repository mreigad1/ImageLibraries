#pragma once
#include "debug.hpp"
#include "colorModelEnums.hpp"
#include "colorModelTypes.hpp"
#include "Array2D.hpp"
#include "imageGrid.hpp"
#include <map>
#include <vector>
#include <functional>

typedef GreyscalePix::pixelGreyscale Grey_P;

class GreyscaleClass {
	private:
		long long counter;
		PrecisionType totalSum;
		const PrecisionType lower;
		const bool lowerInclusive;
		const PrecisionType upper;
		const bool upperInclusive;
		const PrecisionType classLabel;

		bool aboveLow(const PrecisionType p) const {
			return (lower < p) || (lowerInclusive && lower == p);
		}

		bool beneathHigh(const PrecisionType p) const {
			return (p < upper) || (upperInclusive && p == upper);
		}

		GreyscaleClass():
			counter(0),
			totalSum(0),
			lower(0),
			lowerInclusive(0),
			upper(0),
			upperInclusive(0),
			classLabel(0) {
		}

	public:
		GreyscaleClass(PrecisionType low, bool l_inc, PrecisionType high, bool h_inc, PrecisionType label) :
			counter(0),
			totalSum(0),
			lower(low),
			lowerInclusive(l_inc),
			upper(high),
			upperInclusive(h_inc),
			classLabel(label)
		{
		}

		GreyscaleClass(const GreyscaleClass& other):
			counter(other.counter),
			totalSum(other.totalSum),
			lower(other.lower),
			lowerInclusive(other.lowerInclusive),
			upper(other.upper),
			upperInclusive(other.upperInclusive),
			classLabel(other.classLabel)
		{
		}

		bool inClass(const PrecisionType p) const {
			return aboveLow(p) && beneathHigh(p);
		}

		bool insertInClass(const PrecisionType p) {
			if (inClass(p)) {
				this->counter++;
				this->totalSum += p;
				return true;
			}
			return false;
		}

		PrecisionType getLabel() const {
			return classLabel;
		}

		PrecisionType getClassAvgIntensity() const {
			return totalSum / counter;
		}
};

class GreyscaleClassification {

	std::vector<GreyscaleClass*> pClasses;

	public:
		GreyscaleClassification() {
			pClasses.push_back(new GreyscaleClass(   0, true, 125, false,   0 ));
			pClasses.push_back(new GreyscaleClass( 125, true, 175, false, 128 ));
			pClasses.push_back(new GreyscaleClass( 175, true, 255,  true, 255 ));
		}

		~GreyscaleClassification() {
			for (auto ptrItem : pClasses) {
				delete ptrItem;
			}
		}

		PrecisionType addAndReturnLabel(const PrecisionType& pix, bool* const success = NULL) {
			bool wasSuccess = false;
			PrecisionType label = 0;
			for (auto ptrItem : pClasses) {
				auto& gClass = *ptrItem;
				if (gClass.insertInClass(pix)) {
					label = gClass.getLabel();
					wasSuccess = true;
					break;
				}
			}

			if (!wasSuccess) {
				std::cout << "did not find " << pix << " in " << __PRETTY_FUNCTION__ << std::endl;
			}

			if (NULL != success) {
				*success = wasSuccess;
			}
			ASSERT(wasSuccess);
			return label;
		}

		PrecisionType getClassAverage(const PrecisionType& pix, bool* success = NULL) const {
			bool wasSuccess = false;
			PrecisionType avg = 0;

			for (auto ptrItem : pClasses) {
				auto& gClass = *ptrItem;
				if (gClass.inClass(pix)) {
					avg = gClass.getClassAvgIntensity();
					wasSuccess = true;
					break;
				}
			}

			if (!wasSuccess) {
				std::cout << "did not find " << pix << " in " << __PRETTY_FUNCTION__ << std::endl;
			}

			if (NULL != success) {
				*success = wasSuccess;
			}
			return avg;
		}

		PrecisionType getLabel(const PrecisionType& pix, bool* const success = NULL) const {
			bool wasSuccess = false;
			PrecisionType avg = 0;

			for (auto ptrItem : pClasses) {
				auto& gClass = *ptrItem;
				if (gClass.inClass(pix)) {
					avg = gClass.getLabel();
					wasSuccess = true;
					break;
				}
			}

			if (!wasSuccess) {
				std::cout << "did not find " << pix << " in " << __PRETTY_FUNCTION__ << std::endl;
			}

			if (NULL != success) {
				*success = wasSuccess;
			}
			return avg;
		}
};

class NearestNeighbor {
	public:
		NearestNeighbor(const Array2D<imageGrid<Grey_P>>& image) :
			classifier(),
			trainedImg(image),
			origImg(image) {
				//convenience variable
				const unsigned hOff = trainedImg.Height() / 2;

				//for each subimage perform training
				for (unsigned  i = 0; i < hOff; i++) {
					for (unsigned j = 0; j < trainedImg.Width(); j++) {

						//default failure
						bool success = false;

						//get average intensity
						PrecisionType avgLvl = trainedImg[i][j].getAverageImageIntensity();

						//get training label
						PrecisionType pix = classifier.addAndReturnLabel(avgLvl, &success);

						//assert success
						ASSERT(success);

						//create transform to write training labels in subimage
						std::function<Grey_P(const unsigned int, const unsigned int)> assign = 
							std::bind(
								&imageGrid<Grey_P>::assignAll,
								&trainedImg[i][j],
								std::placeholders::_1,
								std::placeholders::_2,
								pix
							);

						//write all pixels with training label
						trainedImg[i][j] = trainedImg[i][j].transformGrid(assign);
					}
				}
		}

		NearestNeighbor(const NearestNeighbor& other) :
			classifier(other.classifier),
			trainedImg(other.trainedImg),
			origImg(other.origImg) 
		{
		}

		Array2D<imageGrid<Grey_P>> getM1() const {
			//return copy of trained image
			Array2D<imageGrid<Grey_P>> rv = trainedImg;
			return rv;
		}

		Array2D<imageGrid<Grey_P>> getN1() const {
			//create function to write with neighbor training label
			std::function<imageGrid<Grey_P>(const imageGrid<Grey_P>&)> doN1 = 
				std::bind(
					&NearestNeighbor::writeFromNeighborTrainingLabel,
					this,
					std::placeholders::_1
				);

			//perform transform with writeFromNeighborTrainingLabel
			return transformLowerFromUpper(doN1);
		}

		Array2D<imageGrid<Grey_P>> getN2() const {
			//create function to write with neighbor training label
			std::function<imageGrid<Grey_P>(const imageGrid<Grey_P>&)> doN1 = 
				std::bind(
					&NearestNeighbor::writeNeighborData,
					this,
					std::placeholders::_1
				);

			//perform transform with writeFromNeighborTrainingLabel
			return transformLowerFromUpper(doN1);
		}

		Array2D<imageGrid<Grey_P>> getN3() const {
			//create function to write with neighbor training label
			std::function<imageGrid<Grey_P>(const imageGrid<Grey_P>&)> doN1 = 
				std::bind(
					&NearestNeighbor::writeFromNeighborAvgGreyscale,
					this,
					std::placeholders::_1
				);

			//perform transform with writeFromNeighborTrainingLabel
			return transformLowerFromUpper(doN1);
		}

		Array2D<imageGrid<Grey_P>> getN4() const {
			//create function to write with neighbor training label
			std::function<imageGrid<Grey_P>(const imageGrid<Grey_P>&)> doN1 = 
				std::bind(
					&NearestNeighbor::writeFromNeighborAVGClass,
					this,
					std::placeholders::_1
				);

			//perform transform with writeFromNeighborTrainingLabel
			return transformLowerFromUpper(doN1);
		}

		Array2D<imageGrid<Grey_P>> getT1() const {
			Array2D<imageGrid<Grey_P>> rv = trainedImg;

			//convenience variable
			const unsigned hOff = rv.Height() / 2;

			//for each subimage perform training
			for (unsigned  i = hOff; i < 2 * hOff; i++) {
				for (unsigned j = 0; j < rv.Width(); j++) {

					//default failure
					bool success = false;

					//get average intensity
					PrecisionType avgLvl = rv[i][j].getAverageImageIntensity();

					//get training label
					PrecisionType pix = classifier.getLabel(avgLvl, &success);

					//assert success
					ASSERT(success);

					//create transform to write training labels in subimage
					std::function<Grey_P(const unsigned int, const unsigned int)> assign = 
						std::bind(
							&imageGrid<Grey_P>::assignAll,
							&rv[i][j],
							std::placeholders::_1,
							std::placeholders::_2,
							pix
						);

					//write all pixels with training label
					rv[i][j] = rv[i][j].transformGrid(assign);
				}
			}
			return rv;
		}

		Array2D<imageGrid<Grey_P>> transformLowerFromUpper(std::function<imageGrid<Grey_P>(const imageGrid<Grey_P>&)> func) const {

			//copy entire trained image
			Array2D<imageGrid<Grey_P>> rv = trainedImg;

			//convenience rename
			const unsigned int hOff = rv.Height() / 2;

			//for each square in lower half
			for (unsigned  i = hOff; i < rv.Height(); i++) {
				for (unsigned j = 0; j < rv.Width(); j++) {

					//candidate neighbor coordinates and distance
					unsigned n_y = hOff;
					unsigned n_x = 0;
					PrecisionType diff = origImg[i][j].getAverageDifference(origImg[hOff][0]);

					//for each neighbor in trained set
					for (unsigned  n_i = 0; n_i < hOff; n_i++) {
						for (unsigned n_j = 0; n_j < rv.Width(); n_j++) {

							//short circuit if perfect neighbor match
							if (diff == 0) break;

							//get neighbor distance
							PrecisionType tmpDiff = origImg[i][j].getAverageDifference(origImg[n_i][n_j]);

							//if distance to neighbor is less than-equal 
							if (tmpDiff <= diff) {
								//then it is new candidate match
								n_y = n_i;
								n_x = n_j;
								diff = tmpDiff;
							}
						}
					}

					//overwrite subimage with transformation from candidate neighbor
					rv[i][j] = func(origImg[n_y][n_x]);
				}
			}

			//return transformed image copy
			return rv;
		}


	protected:

		imageGrid<Grey_P> writeFromNeighborTrainingLabel(const imageGrid<Grey_P>& src) const {

			//default failure
			bool wasSuccess = false;

			//copy input image
			imageGrid<Grey_P> rv = src;

			//get label from average intensity
			PrecisionType avgPix = src.getAverageImageIntensity();
			PrecisionType label = classifier.getLabel(avgPix, &wasSuccess);

			//assert success
			ASSERT(wasSuccess);

			//assignment function to transform by
			std::function<Grey_P(const unsigned int, const unsigned int)> assign = 
				std::bind(
					&imageGrid<Grey_P>::assignAll,
					&rv,
					std::placeholders::_1,
					std::placeholders::_2,
					label
				);

			//perform image transformation of reassignment
			return rv.transformGrid(assign);
		}

		imageGrid<Grey_P> writeFromNeighborAvgGreyscale(const imageGrid<Grey_P>& src) const {

			//copy input image
			imageGrid<Grey_P> rv = src;

			//get label from average intensity
			PrecisionType avgPix = src.getAverageImageIntensity();
			PrecisionType label = avgPix;

			//assignment function to transform by
			std::function<Grey_P(const unsigned int, const unsigned int)> assign = 
				std::bind(
					&imageGrid<Grey_P>::assignAll,
					&rv,
					std::placeholders::_1,
					std::placeholders::_2,
					label
				);

			//perform image transformation of reassignment
			return rv.transformGrid(assign);
		}

		imageGrid<Grey_P> writeNeighborData(const imageGrid<Grey_P>& src) const {
			return src;
		}

		imageGrid<Grey_P> writeFromNeighborAVGClass(const imageGrid<Grey_P>& src) const {

			//default failure
			bool wasSuccess = false;

			//copy input image
			imageGrid<Grey_P> rv = src;

			//get label from average intensity
			PrecisionType avgPix = src.getAverageImageIntensity();
			PrecisionType label = classifier.getClassAverage(avgPix, &wasSuccess);

			//assert success
			ASSERT(wasSuccess);

			//assignment function to transform by
			std::function<Grey_P(const unsigned int, const unsigned int)> assign = 
				std::bind(
					&imageGrid<Grey_P>::assignAll,
					&rv,
					std::placeholders::_1,
					std::placeholders::_2,
					label
				);

			//perform image transformation of reassignment
			return rv.transformGrid(assign);
		}

		GreyscaleClassification classifier;
		Array2D<imageGrid<Grey_P>> trainedImg;
		const Array2D<imageGrid<Grey_P>>& origImg;
};

class kMeans : public NearestNeighbor {
	public:
		kMeans(const Array2D<imageGrid<Grey_P>>& image) :
			NearestNeighbor(image),
			clusters(image.Height(), image.Width()),
			intensities(image.Height(), image.Width()) {
				trainedImg = origImg;

				for (unsigned  i = 0; i < trainedImg.Height(); i++) {
					for (unsigned j = 0; j < trainedImg.Width(); j++) {
						clusters[i][j] = -1;
					}
				}

				const int numGreyLevels = 3;
				long long memberCounts[numGreyLevels] = {0,0,0};
				PrecisionType greyLevels[numGreyLevels] = { 255, 128, 0 };
				PrecisionType greySums[numGreyLevels] = { 0, 0, 0 };

				//convenience variable
				const unsigned hOff = trainedImg.Height() / 2;

				//set intensity of each sub image
				for (unsigned  i = 0; i < hOff; i++) {
					for (unsigned j = 0; j < trainedImg.Width(); j++) {
						intensities[i][j] = origImg[i][j].getAverageImageIntensity();
					}
				}

				//for each count of means
				const int means = 10;
				for (int meansCount = 0; meansCount < means; meansCount++) {

					//for each block
					//assign nearest cluster and determine averages
					for (unsigned  i = 0; i < hOff; i++) {
						for (unsigned j = 0; j < trainedImg.Width(); j++) {

							//initialize distance
							clusters[i][j] = 0;
							PrecisionType distance = abs(greyLevels[0] - intensities[i][j]);
							//distance *= distance;

							//for each remaining cluster
							for (int k = 1; k < numGreyLevels; k++) {

								//candidate distance
								PrecisionType tmpDist = abs(greyLevels[k] - intensities[i][j]);
								//tmpDist *= tmpDist;

								//if better fit for that cluster then set there
								if (distance >= tmpDist) {
									clusters[i][j] = k;
									distance = tmpDist;
								}
							}

							//move block to that cluster, increase count & average
							memberCounts[clusters[i][j]]++;
							greySums[clusters[i][j]] += intensities[i][j];
						}
					}

					//figure out gray scale average
					if ((meansCount + 1) != means) {
						for (int k = 0; k < numGreyLevels; k++) {
							greyLevels[k] = greySums[k] / memberCounts[k];

							//wipe counts & averages
							greySums[k] = 0;
							memberCounts[k] = 0;
						}
					}
				}

				//for each subimage perform training
				for (unsigned  i = 0; i < hOff; i++) {
					for (unsigned j = 0; j < trainedImg.Width(); j++) {
						//create transform to write training labels in subimage
						std::function<Grey_P(const unsigned int, const unsigned int)> assign = 
							std::bind(
								&imageGrid<Grey_P>::assignAll,
								&trainedImg[i][j],
								std::placeholders::_1,
								std::placeholders::_2,
								greyLevels[clusters[i][j]]
							);

						//write all pixels with training label
						trainedImg[i][j] = trainedImg[i][j].transformGrid(assign);
					}
				}

		}

	protected:
		Array2D<int> clusters;
		Array2D<PrecisionType> intensities;
};