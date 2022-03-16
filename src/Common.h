/**
 * Common types for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 *
 * MIT LICENCE
 * -----------
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */	

#pragma once
#ifndef _MAC_COMMONH_
#define _MAC_COMMONH_ 1

#include <Arduino.h>

namespace mac{

	/**
	 * Use double-precision floating point. Default is 'float' (single precision) 
	 */
	//#define MAC_USE_DOUBLE

	/**
	 * XXX: Implement this flag
	 * Mac makes extensive use of object pooling to recycle objects instead of
	 * creating new ones. This results in better performance, and less heap
	 * fragmentation, but does require more RAM. On low RAM systems, or for
	 * situations where you are using 1000s of simultanious objects you may need
	 * to switch this off to use less RAM.
	 */
	#define MAC_OBJECT_REUSE 1

	/**
	 * XXX: Implement this flag
	 * Use DMA for memory operations where possible
	 */
	#define MAC_USE_DMA 0

	/**
	 * Define the floating point type to be used by mac
	 * Possible: float (default), double
	 */
	#ifdef MAC_USE_DOUBLE
	typedef double float_t;
	#else
	typedef float float_t;
	#endif

	/**
	 * @brief Swap two values
	 **/
	template<typename T>
	inline void swap(T &a, T &b){
		T t = a;
		a = b;
		b = t;
	}

	/**
	 * @brief Linear interpolation between two values
	 */
	inline float_t lerp( float_t a, float_t b, float_t k ) {
		return a + k * (b - a);
	}

} // ns::mac

#endif