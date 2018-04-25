/**
 * Graphics library for "mac/μac"
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
 
#ifndef _MAC_GRAPHICSH_
#define _MAC_GRAPHICSH_ 1

#include "Display.h"
#include "GraphicsBitmapExtn.h"
#include "GraphicsVectorExtn.h"

/**
 * mac (or μac) stands for "Microprocessor Adventure Creator"
 * mac is a project that enables creating and playing adventure games on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{
	
	/**
	 * A device and display independent graphics library. All the methods in
	 * this library work directly on an in-memory framebuffer. It is up to the
	 * user to write the framebuffer to the display.
	 */
	class Graphics {
		
		public:
			/**
			 * Constructor. Must pass in a display adapter for the hardware display being used. Can optionally
			 * pass in any number of supported extensions (current "bitmap" and "vector", but in the future could
			 * include "video", "mandelbrot" etc).
			 * @param	display			A Display instance for the hardware display being used
			 **/
			Graphics( Display* display );
			
			/**
			 * Destructor
			 **/
			~Graphics();
			
			/**
			 * Update the framebuffer to the display
			 * @param	useBufferRect	If true, will update only the portion of the framebuffer that
			 *							has changed since the last call to update.
			 **/
			void update( boolean useBufferRect = false );
			
			/**
			 * Fill the framebuffer with a color
			 **/
			void clear( color color );
			
			/***
			 *** EXTENSIONS
			 ***/
			
			/**
			 * Bitmap graphic support
			 **/
			GraphicsBitmapExtn* bitmap;
			
			/**
			 * Vector graphic support
			 **/
			GraphicsVectorExtn* vector;

		protected:
			/**
			 * The display instance for the hardware
			 **/
			Display* _display;
			
			/**
			 * The framebuffer
			 **/
			BufferRect _framebuffer;
			
			/**
			 * The area of the framebuffer that has changed since the last call to getBuffer.
			 * The back and front BufferRects are swapped when getBuffer is called.
			 * _bufferRectIndex is the index of the active BufferRect (the one that changes
			 * when draw calls are made).
			 **/
			BufferRect* _bufferRect[2];
			uint8_t _bufferRectIndex;
			
			/**
			 * Grab the address of the buffer and swap the buffer rect so that it is preserved
			 * during transfer to the display. This is done during update. All other drawing
			 * commands that access the framebuffer should do so using _framebuffer directly.
			 **/
			uint16_t* _getBuffer( void );
			
			/**
			 * Returns pointer to structure that defines the area of the framebuffer that has
			 * changed since the last call to getBuffer.
			 **/
			BufferRect* _getBufferRect( void );
			
			/**
			 * Reset the BufferRect back to default. The default has w and h of 0, and x and y at
			 * their maximum values of framebuffer width and height respectively. Always check that
			 * the w and h are > 0 before applying the rect.
			 **/
			void _resetBufferRect( uint8_t index );
			
			/**
			 * Copy an area of the active buffer described by a BufferRect (the src) to the destination buffer.
			 * @param	src		A BufferRect which describes the src
			 * @param	dst		The destination buffer
			 **/
			void _copyArea( BufferRect* src, uint16_t* dst );
			uint16_t* _copyAreaSrc;
			uint16_t* _copyAreaDst;
			int16_t _copyAreaIter;
			
	};
	
} // namespace

#endif