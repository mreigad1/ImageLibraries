#ImageLibraries
Author: Matt Reigada
Email:  mreigad1@binghamton.edu

Theory Assignment Component:
	Please see the attached Assignment2Theory.pdf for answers to theory
	component of assignment.

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
	Sharpen Algorithm:
		To process image kernel multiplication a generic multiply method was implemented.
		When processing a kernel member which is outside the frame of the window, the image
		adds additional weight to the pixel currently in processing.

			Sharpening Kernel:
				[ -1,   -4,   -6,   -4,   -1, ]
				[ -4,  -16,  -24,  -16,   -4, ]
				[ -6,  -24,  476,  -24,   -6, ]
				[ -4,  -16,  -24,  -16,   -4, ]
				[ -1,   -4,   -6,   -4,   -1, ]

	Sobel Operator:
		For calculation of the sobel processed pixels, two intermediate kernel processed
		images are created from the following kernels:

					[ -1,  0,  1, ]
			S1 = 	[ -2,  0,  2, ]
					[ -1,  0,  1  ]

					[ -1, -2, -1, ]
			S2 = 	[  0,  0,  0, ]
					[  1,  2,  1  ]

		From these the result S = SQRT(S1*S1 + S2*S2) is calculated.

	Laplacian of Gaussian:
		To process the Laplacian of the Gaussian of the image, a kernel is generated to
		proved kernel weights as per the LoG equation. This kernel is then multiplied by
		the image.

Results & Easy Assignment 2 Execution:
	To launch assignment 1 please execute "make Assignment2" after resolving
	any outstanding local dependencies on opencv. Upon executing makefile,
	programs will be built and launched.

	Four image demonstrations will be presented in simultaneously.
	These will present the resultant processed images with their respective
	labels placed into the Window label of the OpenCV window.
	The first instance of four windows will display the result of image processing
	upon basel_gray.bmp, upon closing these windows with the 'escape' key, the same
	algorithms will be processed upon the second input ant_gray.bmp

Advanced Assignment 2 Execution:
	Please see contents of makefile

Bug Report:
	No known bugs or missing functionality at this time.