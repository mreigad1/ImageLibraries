#ImageLibraries
Author: Matt Reigada
Email:  mreigad1@binghamton.edu

Theory Assignment Component:
	Please see the attached Assignment3.pdf for answers to section A. Part B Answers:
		B1 Part a
		      first_row[] = {  10,  11,  12,  11,  12,  13,  12,  11  };
		     second_row[] = {  10, -10,   8,  -7,   8,  -8,   7,  -7  };
		   DCT_Result_1[] = { 32.5, -1.28, -1.31, 0.45, -1.41, -0.301, 0.541, 0.255  };
		   DCT_Result_2[] = { 0.354, 4.25, 0.35, 5.05, 2.47, 8.38, 1.77, 20.4  };
		reversed_DCT_R1[] = {  10,  11,  12,  11,  12,  13,  12,  11  };
		reversed_DCT_R2[] = {  10, -10,   8,  -7,   8,  -8,   7,  -7  };


		B1 Part b
			  array_size_16[] = {  10,  11,  12,  11,  12,  13,  12,  11,  10, -10,   8,  -7,   8,  -8,   7,  -7  };
			    DCT_Size_16[] = { 23.2, 21.8, -3.91, -6.96, -0.676, 6.04, -3.25, -2.57, 0.75, 3.16, -6.14, 1.61, 1.63, 5.5, -14.2, 13.6  };
			   reversed_DCT[] = {  10,  11,  12,  11,  12,  13,  12,  11,  10, -10,   8,  -7,   8,  -8,   7,  -7  };


		B1 Part c
			Question:
			Compare the results of (a) and (b). For this particular case would you 
			suggest a block size of 8 or 16 for greater compression? Justify your 
			answer.

			Answer:
				The first 8 elements are nearly identical, this means that they may be very 
				effectively compressed together with very little frequency datum. The first 8
				elements vary significantly from the second 8 elements which are more alike to
				each other than the first sequence. This means that compressing each of the two
				lines separately will yield better compression volume and accuracy than
				compressing the entire 16 numbers together.

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
	To better debug and remove redundent paths, much of my codebase was refactored
	using std::functional to reduce the number of redundent and unmaintainable code
	paths.

	HSI to RGB to HSI conversions are handled at the per pixel level with pixel
	classes overloading the cast conversions between their types. Pixels types are
	each implemented between a high precision floating point class representing
	pixels as mathematical structures, with an additional front end class intended
	for use in reinterpreting existing on-the-wire pixel buffers. Upon operation the
	low precision pixel class will upcast to the high precision pixel class.
	Upon assignment the high end precision class will downcast to the low
	precision/size representation class. This prevents round-off errors during single
	line calculations upon pixels.

	For DCT compression my algorithm stores a static cache of commonly used
	coefficients for DCT calculation. The result is a one-time constructed lookup
	table which conveniently fits in cache and significantly reduces the number of
	required floating point operations for DCT compression. This cached structure
	is reused for decompression. The downside of this cache is that it is a static
	structure, thus the 8x8 nature of the grid is built in at compile time. A generic
	function retrieves these values for arbitrary size cases.

Results & Easy Assignment 3 Execution:
	To launch assignment 3 please execute "make Assignment3" after resolving
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

Advanced Assignment 3 Execution:
	Please see contents of makefile

Bug Report & Missing features:
	Bugs were accidentally added to HSI conversion as I began implementing the
	region of interest algorithm. Due to extended time debugging bad datat (nan values)
	in my conversion routines I failed to fully implement the rest of the ROI algorithm
	on time.