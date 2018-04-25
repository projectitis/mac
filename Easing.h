/**
 * Easing functions for "mac/μac"
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

/**
 * Easing functions lifted from https://github.com/warrenm/AHEasing
 * Modified for Teensy
 **/
 
#ifndef _MAC_EASING_H
#define _MAC_EASING_H 1

namespace mac{

	/**
	 * Function pointer to allow specifying any easing function
	 **/
	typedef float (*easingFunction)( float );

	// Linear interpolation (no easing)
	float easeNone( float p );

	// Quadratic easing; p^2
	float quadraticEaseIn( float p );
	float quadraticEaseOut( float p );
	float quadraticEaseInOut( float p );

	// Cubic easing; p^3
	float cubicEaseIn( float p );
	float cubicEaseOut( float p );
	float cubicEaseInOut( float p );

	// Quartic easing; p^4
	float quarticEaseIn( float p );
	float quarticEaseOut( float p );
	float quarticEaseInOut( float p );

	// Quintic easing; p^5
	float quinticEaseIn( float p );
	float quinticEaseOut( float p );
	float quinticEaseInOut( float p );

	// Sine wave easing; sin(p * PI/2)
	float sineEaseIn( float p );
	float sineEaseOut( float p );
	float sineEaseInOut( float p );

	// Circular easing; sqrt(1 - p^2)
	float circularEaseIn( float p );
	float circularEaseOut( float p );
	float circularEaseInOut( float p );

	// Exponential easing, base 2
	float exponentialEaseIn( float p );
	float exponentialEaseOut( float p );
	float exponentialEaseInOut( float p );

	// Exponentially-damped sine wave easing
	float elasticEaseIn( float p );
	float elasticEaseOut( float p );
	float elasticEaseInOut( float p );

	// Overshooting cubic easing; 
	float backEaseIn( float p );
	float backEaseOut( float p );
	float backEaseInOut( float p );

	// Exponentially-decaying bounce easing
	float bounceEaseIn( float p );
	float bounceEaseOut( float p );
	float bounceEaseInOut( float p );

} // ns:mac

#endif
