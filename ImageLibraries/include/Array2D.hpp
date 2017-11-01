#pragma once
#include "debug.hpp"
#include <assert.h>
#include <cstring>

template<class T> class Array2D {
	protected:
		unsigned int h;
		unsigned int w;
		T* array;

	public:
		Array2D() :
			h(0),
			w(0),
			array(NULL) {
		}

		Array2D(unsigned int _h, unsigned int _w) :
			h(_h),
			w(_w),
			array(new T[_h * _w]) {
		}

		Array2D(unsigned int _h, unsigned int _w, T* other) :
			h(_h),
			w(_w),
			array(new T[_h * _w]) {
				for (unsigned int i = 0; i < Height(); i++) {
					for (unsigned int j = 0; j < Width(); j++) {
						array[i * Width() + j] = other[i * Width() + j];
					}
				}
		}

		Array2D(const Array2D<T>& other) :
			h(other.Height()),
			w(other.Width()),
			array(new T[other.Height() * other.Width()]) {
				for (unsigned int i = 0; i < Height(); i++) {
					for (unsigned int j = 0; j < Width(); j++) {
						array[i * Width() + j] = other.array[i * Width() + j];
					}
				}
		}
		
		~Array2D() {
			delete[] array;
			array = NULL;
		}

		unsigned int Height() const {
			return h;
		}

		unsigned int Width() const {
			return w;
		}

		//returns address of first item in row
		T* operator[](const unsigned int k) const {
			ASSERT(k < Height());
			return &array[k * Width()];
		}

		//copies each data item in array into new array
		Array2D& operator=(const Array2D& other) {
			if (NULL != array) {
				delete[] array;
				array = NULL;
			}
			h = other.Height();
			w = other.Width();
			array = new T[Height() * Width()];
			for (unsigned int i = 0; i < Height(); i++) {
				for (unsigned int j = 0; j < Width(); j++) {
					array[i * Width() + j] = other.array[i * Width() + j];
				}
			}
			return *this;
		}

        void exportContents(T* output) const {
        	for (unsigned int i = 0; i < Height(); i++) {
        		for (unsigned int j = 0; j < Width(); j++) {
        			output[i * Width() + j] = array[i * Width() + j];
        		}
        	}
        }

        void unsafe_erase() {
        	memset(array, 0, Height() * Width() * sizeof(T));
        }
};

