#ImageLibraries
Author: Matt Reigada
Email:  mreigad1@binghamton.edu

Compatibility:
	This library package has been written for compatibility with the Mac OS
	distribution of the Unix OS. Limited compatibility with the linux (Ubuntu)
	operating system has been tested but is not guaranteed functional. For use
	with the linux operating please adjust the environment parameters at the
	top of the makefile to build against the opencv layout of your local system.

Dependencies:
	This library requires use of a g++ (or aliased clang) compiler with
	compatibility with C++11 as well as local installation of opencv2.
	Opencv2 is used strictly for reading image files into raw pixel buffers
	and displaying images in windows. Please execute "./dependency.sh" for
	more information on installing opencv2 to your local system.

Algorithms & Methods:
	Part1:
			Algorithms for machine learning are contained within
		GreyscaleClassification.h, these algorithms will receive a sliced image
		and will perform training upon the upper half of the image as specified
		in the assignment specification. In the instance of the k-means algorithm
		distance is calculated from each candidate data component starting with a
		set of means = {0, 128, 255}. These will loop 10 times with new means and
		greyscale intensity levels being displayed upon each loop iteration.

		NOTE: 	The error rate % difference between N1 & T1 will be output to the
			console during the program execution.

	Part2:
			Algorithms for motion vector estimation may be found within
		motionVectorMap.hpp, the algorithm will iterate over each pixel applying
		a structuring element to be used to search for candidate pixel matches.
		The structuring elements used is a circular radii of 7 pixels as this 
		radii guarantees a signed 4-bit x-coordinate offset with a signed 4-bit
		y-coordinate offset, allowing for each pixel in frame at time T1 to be
		compressed to a single byte x+y coordinate offset to map from pixel in
		frame at T0. From here, the theory was to block 8x8 pixel groups for motion
		vectors, however a valuable heuristic for determining an effective block
		motion vector from individual pixel motion vectors could not be derived.
		Each of mean, median, and mode were attempted to group (x,y) offsets
		however results of each proved dissatisfactory.
			At current an 8-bit geographic offset is stored for each pixel
		with a resultant motion compensated difference calculated and reversed
		for image quality verification. Motion compensated images appear to remove
		most information within the pixel motion vector algorithm.

		NOTE:	The motion vectors of each pixel will be output to the console
			during program execution with a block printed for each horizontal
			row of pixels printing their corresponding motion vectors. Given
			the poor performance of blocked motion vectors it proved impractical
			to render each motion vector within the image frame, thus they are
			demonstrated with (x, y) offsets within the terminal.

Results & Easy Assignment 5 Execution:
	To launch assignment 5 please execute "make Assignment5" after resolving
	any outstanding local dependencies on opencv. Upon executing makefile,
	programs will be built and launched. Different steps of assignment may
	be proceded to by pressing escape key.

	Multiple image demonstrations will be presented in simultaneously for each
	required image step. These will present the resultant processed images with
	their respective labels placed into the Window label of the OpenCV window.
	These labels will explain the feature the window implements with
	respect to the assignment.

	The Region of Interest algorithm was not yet fully implemented. As of submission,
	the program will beginning the preprocessing steps of refining the edges of the 
	HSI model using masking and the sobel operator. The image will be thresholded based
	upon its histogram data before being converted to binary. The binary image is then
	used to	restore the original image with all edges thoroughly defined. The key steps
	of this algorithm following preprocessing were not yet implemented.

	Execution Screenshots:
		Please see the screenshots folder for images demonstrating execution output.

Advanced Assignment 5 Execution:
	Please see contents of makefile

Bug Report & Missing features:
	Compression of pixel motion vectors by grouping can be substantially improved to yield
	better than the slightly less than 3:1 compression ratio than the algorithm as presented
	in part 2 of the assignment.
