#pragma once
#include <iostream>
#include <stdlib.h>

//Macro to create strings
#define STR(X)	#X

//Macro for assertions
#define ASSERT(X) do {							\
	if (false == (X)) {							\
		std::cout << "Error at ASSERT(" <<		\
			STR(X) << ") on line " << __LINE__	\
			<< " in " << __FILE__				\
			<< std::endl << std::flush;			\
		exit(-1);								\
	}											\
} while (0)										\

//Macro to force deferred implementation
#define TBI	do {								\
	std::cout << "Function " 					\
		<< __PRETTY_FUNCTION__ << " in file " 	\
		<< __FILE__ 							\
		<< " lacks implementation!\n"			\
		<< std::flush;							\
	exit(-1);									\
} while (0)										\

struct scopePrint {
	std::string fileName;
	std::string funcName;
	int lineNum;
	bool destructorPrint;
	scopePrint(const char* file, const char* func, int line, bool showDestructor = true) :
		fileName(file),
		funcName(func),
		lineNum(line),
		destructorPrint(showDestructor) {
			if (destructorPrint) {
				std::cout << "Enter ";
			}
			std::cout << fileName << ":" << lineNum << ":" << funcName << std::endl << std::flush;
	}
	~scopePrint() {
		if (destructorPrint) {
			std::cout << "Exit  " << fileName << ":" << funcName << std::endl << std::flush;
		}
	}
};

#ifndef DISABLE_LOGGING
	#define FUNC_LOG scopePrint LONG_NAME_NO_COLLISIONS_FOR_LOGGING(__FILE__, __PRETTY_FUNCTION__, __LINE__)
	#define LINE_LOG do {																		\
		scopePrint FIXED_TO_SCOPE_MORE_SAFE(__FILE__, __PRETTY_FUNCTION__, __LINE__, false);	\
	} while(0)
#else
	#define FUNC_LOG do { ; } while(0)
	#define LINE_LOG do { ; } while(0)
#endif