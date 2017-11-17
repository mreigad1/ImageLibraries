#ImageLibraries
Author: Matt Reigada
Email:  mreigad1@binghamton.edu

Theory:
	The theory component of this assignment may be found in the top level directory
	with the name Assignment4Theory.pdf

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
	To streamline the morphological operations of dilation and erosion I defined
	the behvarior of structuring element/image pixels as members of the structuring
	elements set if their values are zero or positive. If member values of the
	structuring element are negative then they are not considered members of the
	structuring morphological neighborhood. This was done in order to permit
	neighbor-only erosion/dilation without a gain/loss parameter which permits
	binary morphological operations over input binary images.

	The objects within the input image are counted by removing the morphological
	gradient from the opening of the image before writing back to the image.
	This is done 3 times in order to take the third order result of this function.
	Following this morphological process, the image is thresholded into binary and
	clustered into object groups with any group smaller than ~0.2% of the image
	discarded as noise.

Results & Easy Assignment 4 Execution:
	To launch assignment 4 please execute "make Assignment4" after resolving
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

Advanced Assignment 4 Execution:
	Please see contents of makefile

Bug Report & Missing features:
	Erosion of image is excessive, there were difficulties isolating the two pigs to the
	bottom left of the image as the edge distinguishing them was not high enough contrast
	to trim with the morphological gradient.