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
#ifndef _MAC_LINKEDLISTH_
#define _MAC_LINKEDLISTH_ 1

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{
	
	/**
	 * Double-linked list. Inherit from this object to add list functionallity.
	 */
	class LinkedList {
		
		public:
			/**
			 * Return the next item
			 */
			LinkedList* next();

			/**
			 * Return the previous item
			 */
			LinkedList* prev();

			/**
			 * Add an item to the end of the linked list
			 */
			void add( LinkedList* item );

			/**
			 * Insert an item as the previous item in the list
			 */
			void before( LinkedList* item );

			/**
			 * Insert an item as the next item in the list
			 */
			void after( LinkedList* item );

			/**
			 * Remove the current item from the list
			 * @return	Pointer to the next item in the list
			 */
			LinkedList* remove();

			/**
			 * Remove the specified item from the list
			 * @return	Pointer to the next item in the list
			 */
			LinkedList* remove( LinkedList* item );

		protected:

			/**
			 * Previous object in list
			 */
			LinkedList* _prev = 0;

			/**
			 * Next object in list
			 */
			LinkedList* _next = 0;
	};
	
} // namespace

#endif