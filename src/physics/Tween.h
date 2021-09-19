/**
 * GUI library for "mac/μac"
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
#ifndef _MAC_TWEENH_
#define _MAC_TWEENH_ 1

#include "Common.h"
#include "utils/LinkedList.h"
#include "physics/Easing.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	/**
	 * Listener callback. If the callback returns true, the listener remains
	 * in place and continues to listen for future events. If it returns false
	 * the listener will be removed from the list. 
	 * @param  void*		Pointer to user data associated with the listener
	 * @return     true to keep listening, false to remove the listener
	 */
	typedef bool (*TweenCallback)(void*);

	/**
	 * Tween item. Holds details of a single tween in a linked list.
	 */
	class TweenItem: public LinkedList {
		public:
			TweenItem( float_t* property, float_t from, float_t to, float_t seconds, EasingFunction ease, TweenCallback callback, void* callbackData );

			float_t* property;

			float_t from;
			float_t change;
			float_t current;
			float_t seconds;
			float_t seconds_inv;
			EasingFunction ease;
			TweenCallback callback;
			void* callbackData;
	};
	
	/**
	 * Tweening functionallity
	 */
	class Tween {
		
		public:
			/**
			 * Add a tween
			 */
			void addTween( float_t* property, float_t from, float_t to, float_t seconds, EasingFunction ease, TweenCallback callback, void* callbackData );

			/**
			 * Remove a tween
			 */
			void removeTween( float_t* property );

			/**
			 * Update all tweens
			 */
			void update( float_t dt );

		private:

			TweenItem* _tweens = 0;

	};
	
} // namespace

#endif