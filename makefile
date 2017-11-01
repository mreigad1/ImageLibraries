##known working OSX environ
VERSION=2.4.13.2_2
OPENCVBASE=/usr/local/Cellar/opencv\@2/${VERSION}
LIB_PATHS=-L${OPENCVBASE}/lib
INCLUDE_PATHS=-I${OPENCVBASE}/include/
LIBS=-lopencv_highgui -lopencv_core -lopencv_imgproc
CVOPTIONS=${INCLUDE_PATHS} ${LIB_PATHS} ${LIBS}

##known working linux environ
#VERSION=2.4.12
#LIB_PATHS=-L/usr/local/Cellar/opencv/${VERSION}/lib
#LIBS=-lopencv_highgui -lopencv_core

BUILD_DIR=build
TEST_DIR=test
ASSETS_DIR=ImageLibraries/assets
BUILD_FLAGS=-Wall -Wfatal-errors -Werror -std=c++11

all: main

main: build_dir main.cpp pixel.cpp
	g++ main.cpp pixel.cpp ${LIB_PATHS} ${LIBS} -o main

test: 			perform_test		\
				clean				\

Assignment1:	Assignment1_Part1	\
				Assignment1_Part2	\
				clean				\

Assignment2:	Assignment2_Part1	\

perform_test: 	debugTest			\
				Array2DTest			\
				ByteIntensityTest	\
				PixelGreyscaleTest	\
				PixelRGBTest		\
				PixelHSITest		\
				PixelConversionTest	\
				MaskTest			\
				ImageGridTest		\
				HistogramTest		\

debugTest: build_dir move_tests
	g++ $(BUILD_DIR)/debugTest.cpp -Wfatal-errors -Werror -o $(TEST_DIR)/debugTest
	./$(TEST_DIR)/debugTest A > /dev/null || if [ $$? -eq 255 ] ; then exit 0 ; else exit 1 ; fi
	./$(TEST_DIR)/debugTest T > /dev/null || if [ $$? -eq 255 ] ; then exit 0 ; else exit 1 ; fi
	rm $(TEST_DIR)/debugTest

Array2DTest: build_dir move_tests
	g++ $(BUILD_DIR)/Array2DTest.cpp $(BUILD_FLAGS) -o $(TEST_DIR)/Array2DTest
	./$(TEST_DIR)/Array2DTest
	rm $(TEST_DIR)/Array2DTest

ByteIntensityTest: build_dir move_tests
	g++ $(BUILD_DIR)/ByteIntensityTest.cpp $(BUILD_FLAGS) -o $(TEST_DIR)/ByteIntensityTest
	./$(TEST_DIR)/ByteIntensityTest
	rm $(TEST_DIR)/ByteIntensityTest

PixelRGBTest: build_dir move_tests
	g++ $(BUILD_DIR)/PixelRGBTest.cpp                                          \
		$(BUILD_DIR)/pixelGreyscale.cpp $(BUILD_DIR)/arithmeticalGreyscale.cpp \
		$(BUILD_DIR)/pixelRGB.cpp $(BUILD_DIR)/arithmeticalRGB.cpp             \
		$(BUILD_DIR)/pixelHSI.cpp $(BUILD_DIR)/arithmeticalHSI.cpp             \
		$(BUILD_FLAGS) -Werror -o $(TEST_DIR)/PixelRGBTest
	./$(TEST_DIR)/PixelRGBTest
	rm $(TEST_DIR)/PixelRGBTest

PixelHSITest: build_dir move_tests
	g++ $(BUILD_DIR)/PixelHSITest.cpp                                          \
		$(BUILD_DIR)/pixelGreyscale.cpp $(BUILD_DIR)/arithmeticalGreyscale.cpp \
		$(BUILD_DIR)/pixelRGB.cpp $(BUILD_DIR)/arithmeticalRGB.cpp             \
		$(BUILD_DIR)/pixelHSI.cpp $(BUILD_DIR)/arithmeticalHSI.cpp             \
		$(BUILD_FLAGS) -Werror -o $(TEST_DIR)/PixelHSITest
	./$(TEST_DIR)/PixelHSITest
	rm $(TEST_DIR)/PixelHSITest

PixelGreyscaleTest: build_dir move_tests
	g++ $(BUILD_DIR)/PixelGreyscaleTest.cpp                                    \
		$(BUILD_DIR)/pixelGreyscale.cpp $(BUILD_DIR)/arithmeticalGreyscale.cpp \
		$(BUILD_DIR)/pixelRGB.cpp $(BUILD_DIR)/arithmeticalRGB.cpp             \
		$(BUILD_DIR)/pixelHSI.cpp $(BUILD_DIR)/arithmeticalHSI.cpp             \
		$(BUILD_FLAGS) -Werror -o $(TEST_DIR)/PixelHSITest
	./$(TEST_DIR)/PixelHSITest
	rm $(TEST_DIR)/PixelHSITest

PixelConversionTest: build_dir move_tests
	g++ $(BUILD_DIR)/PixelConversionTest.cpp                                   \
		$(BUILD_DIR)/pixelGreyscale.cpp $(BUILD_DIR)/arithmeticalGreyscale.cpp \
		$(BUILD_DIR)/pixelRGB.cpp $(BUILD_DIR)/arithmeticalRGB.cpp             \
		$(BUILD_DIR)/pixelHSI.cpp $(BUILD_DIR)/arithmeticalHSI.cpp             \
		$(BUILD_FLAGS) -Werror -o $(TEST_DIR)/PixelConversionTest
	./$(TEST_DIR)/PixelConversionTest
	rm $(TEST_DIR)/PixelConversionTest

DCTTest: build_dir move_tests
	g++ $(BUILD_DIR)/DCTTest.cpp $(BUILD_DIR)/DCT.cpp                          \
		$(BUILD_DIR)/pixelGreyscale.cpp $(BUILD_DIR)/arithmeticalGreyscale.cpp \
		$(BUILD_DIR)/pixelRGB.cpp $(BUILD_DIR)/arithmeticalRGB.cpp             \
		$(BUILD_DIR)/pixelHSI.cpp $(BUILD_DIR)/arithmeticalHSI.cpp             \
		$(BUILD_FLAGS) -o $(TEST_DIR)/MaskTest
	./$(TEST_DIR)/MaskTest $(ASSETS_DIR)/hitchhikers.bmp
	rm $(TEST_DIR)/MaskTest

MaskTest: build_dir move_tests
	g++ $(BUILD_DIR)/MaskTest.cpp $(BUILD_DIR)/mask.cpp $(BUILD_FLAGS) -o $(TEST_DIR)/MaskTest
	./$(TEST_DIR)/MaskTest
	rm $(TEST_DIR)/MaskTest

ImageGridTest: build_dir move_tests
	g++ $(BUILD_DIR)/ImageGridTest.cpp $(BUILD_DIR)/mask.cpp                   \
		$(BUILD_DIR)/pixelGreyscale.cpp $(BUILD_DIR)/arithmeticalGreyscale.cpp \
		$(BUILD_DIR)/pixelRGB.cpp $(BUILD_DIR)/arithmeticalRGB.cpp             \
		$(BUILD_DIR)/pixelHSI.cpp $(BUILD_DIR)/arithmeticalHSI.cpp             \
		$(BUILD_FLAGS) -o $(TEST_DIR)/ImageGridTest
	./$(TEST_DIR)/ImageGridTest
	rm $(TEST_DIR)/ImageGridTest

HistogramTest: build_dir move_tests
	g++ $(BUILD_DIR)/histogram.cpp                                             \
		$(BUILD_DIR)/pixelGreyscale.cpp $(BUILD_DIR)/arithmeticalGreyscale.cpp \
		$(BUILD_DIR)/pixelRGB.cpp $(BUILD_DIR)/arithmeticalRGB.cpp             \
		$(BUILD_DIR)/pixelHSI.cpp $(BUILD_DIR)/arithmeticalHSI.cpp             \
		$(BUILD_FLAGS) -DTESTING ${CVOPTIONS} -o $(TEST_DIR)/HistogramTest
	./$(TEST_DIR)/HistogramTest $(ASSETS_DIR)/darkNight.bmp
	rm $(TEST_DIR)/HistogramTest

Assignment1_Part2: build_dir move_Assignment1
	g++ $(BUILD_DIR)/assignment1_part2.cpp $(BUILD_DIR)/histogram.cpp $(BUILD_DIR)/pixelRGB.cpp $(BUILD_DIR)/arithmeticalRGB.cpp $(BUILD_FLAGS) ${CVOPTIONS} -o $(TEST_DIR)/Assignment1_Part2
	./$(TEST_DIR)/Assignment1_Part2 $(ASSETS_DIR)/shapes.bmp $(ASSETS_DIR)/hitchhikers.bmp
	#./$(TEST_DIR)/Assignment1_Part2 $(ASSETS_DIR)/doge.jpg $(ASSETS_DIR)/doge.jpg
	rm $(TEST_DIR)/Assignment1_Part2

Assignment1_Part1: build_dir move_Assignment1
	g++ $(BUILD_DIR)/assignment1_part1.cpp $(BUILD_DIR)/histogram.cpp $(BUILD_DIR)/pixelRGB.cpp $(BUILD_DIR)/arithmeticalRGB.cpp $(BUILD_FLAGS) ${CVOPTIONS} -o $(TEST_DIR)/Assignment1_Part1
	./$(TEST_DIR)/Assignment1_Part1 $(ASSETS_DIR)/darkNight.bmp $(ASSETS_DIR)/darkNight.bmp
	./$(TEST_DIR)/Assignment1_Part1 $(ASSETS_DIR)/brightNYC.bmp $(ASSETS_DIR)/brightNYC.bmp
	#./$(TEST_DIR)/Assignment1_Part1 $(ASSETS_DIR)/doge.jpg $(ASSETS_DIR)/doge.jpg
	rm $(TEST_DIR)/Assignment1_Part1

Assignment2_Part1: build_dir move_Assignment2
	g++ $(BUILD_DIR)/assignment2.cpp $(BUILD_DIR)/pixelRGB.cpp $(BUILD_DIR)/arithmeticalRGB.cpp $(BUILD_DIR)/mask.cpp $(BUILD_FLAGS) ${CVOPTIONS} -o $(TEST_DIR)/Assignment2
	./$(TEST_DIR)/Assignment2 $(ASSETS_DIR)/basel_gray.bmp
	./$(TEST_DIR)/Assignment2 $(ASSETS_DIR)/ant_gray.bmp
	#./$(TEST_DIR)/Assignment2 $(ASSETS_DIR)/doge.jpg
	rm $(TEST_DIR)/Assignment1_Part1

move_tests:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(TEST_DIR)
	cp ImageLibraries/tests/* $(BUILD_DIR)

move_Assignment1:
	mkdir -p $(BUILD_DIR)
	cp ImageLibraries/Assignment1/* $(BUILD_DIR)

move_Assignment2:
	mkdir -p $(BUILD_DIR)
	cp ImageLibraries/Assignment2/* $(BUILD_DIR)

build_dir:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(TEST_DIR)
	cp ImageLibraries/src/* $(BUILD_DIR)/
	cp ImageLibraries/include/* $(BUILD_DIR)/

.PHONY:clean

clean:
	rm -fr $(BUILD_DIR)
	rm -fr $(TEST_DIR)
	rm -f main
