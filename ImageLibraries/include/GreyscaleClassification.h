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
		unsigned counter;
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
			if (!inClass(p)) {
				return false;
			} else {
				counter++;
				totalSum += p;
			}
			return true;
		}

		PrecisionType getLabel() const {
			return classLabel;
		}

		PrecisionType getClassAvgIntensity() const {
			return totalSum / counter;
		}
};

class GreyscaleClassification {

	std::vector<GreyscaleClass> pClasses;

	public:
		GreyscaleClassification() {
			pClasses.push_back(GreyscaleClass(   0, true, 125, false,   0 ));
			pClasses.push_back(GreyscaleClass( 125, true, 175, false, 128 ));
			pClasses.push_back(GreyscaleClass( 175, true, 255,  true, 255 ));
		}

		PrecisionType addAndReturnLabel(const PrecisionType& pix, bool* const success = NULL) {
			bool wasSuccess = false;
			PrecisionType label = 0;
			for (auto gClass : pClasses) {
				if (gClass.insertInClass(pix)) {
					label = gClass.getLabel();
					wasSuccess = true;
					break;
				}
			}

			if (NULL != success) {
				*success = wasSuccess;
			}
			return label;
		}

		PrecisionType getClassAverage(const PrecisionType& pix, bool* success = NULL) const {
			bool wasSuccess = false;
			PrecisionType avg = 0;

			for (auto gClass : pClasses) {
				if (gClass.inClass(pix)) {
					avg = gClass.getClassAvgIntensity();
					wasSuccess = true;
					break;
				}
			}

			if (NULL != success) {
				*success = wasSuccess;
			}
			return avg;
		}
};

class NearestNeighbor {
	public:
		NearestNeighbor(const Array2D<imageGrid<Grey_P>>& image) : classifier(), trainedImg(image), origImg(image) {
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
					rv[i][j] = func(rv[n_y][n_x]);
				}
			}

			//return transformed image copy
			return rv;
		}


	private:

		imageGrid<Grey_P> writeFromNeighborTrainingLabel(const imageGrid<Grey_P>& src) const {

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
		Array2D<imageGrid<Grey_P>> origImg;
};

