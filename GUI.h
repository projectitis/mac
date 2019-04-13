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
 
#ifndef _MAC_GUIH_
#define _MAC_GUIH_ 1

#include "Graphics.h"

/**
 * mac (or μac) stands for "Microprocessor Adventure Creator"
 * mac is a project that enables creating and playing adventure games on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{
	
	/**
	 * A device- and display- independent GUI library. All the methods in
	 * this library work directly on an in-memory framebuffer.
	 */
	class GUI {
		
		public:
			/**
			 * Constructor. Must pass in a graphics object to render the interface to.
			 * @param	graphics			A Graphics instance to render to
			 **/
			GUI( Graphics* graphics );
			
			/**
			 * Destructor
			 **/
			~GUI();

		protected:
			/**
			 * A list of display objects
			 **/
			DisplayObject* _displayList;
			
	};
	
} // namespace

#endif