#pragma once
#ifndef _MAC_COMMONH_
#define _MAC_COMMONH_ 1

#include <Arduino.h>

/**
 * Common types and functions

 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project for creating beautiful and useful
 * apps on various microprocessor boards.
 *
 * mac is distributed under the MIT licence
 **/
namespace mac {

	/**
	 * Use double-precision floating point. Default is 'float' (single precision)
	 */
	#define MAC_USE_DOUBLE 0

	 /**
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
	inline void swap( T& a, T& b ) {
		T t = a;
		a = b;
		b = t;
	}

	/**
	 * @brief Linear interpolation between two values
	 */
	inline float_t lerp( float_t a, float_t b, float_t k ) {
		return a + k * ( b - a );
	}

} // ns::mac

#endif