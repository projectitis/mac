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
#ifndef _MAC_GUIH_
#define _MAC_GUIH_ 1

#include "Widget.h"
#include "Graphics.h"
#include "Messenger.h"
#include "Tween.h"
#include "Input.h"
#include "Style.h"
#include "PanelSet.h"


/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{
	
	/**
	 * GUI library for user interfaces
	 */
	class GUI: public PanelSet {
		
		public:
			//static const int default_bg_color = 0x000000;

			/**
			 * Constructor. Either pass ina graphics instance, or a display instance. If a
			 * display adapter is passed in, will create the graphics object.
			 * @param	graphics			A Graphics instance to render to
			 **/
			GUI( Graphics* graphics, Style* style = 0 );
			GUI( Display* display, Style* style = 0 );
			
			/**
			 * Destructor
			 **/
			~GUI();

			/**
			 * The graphics object
			 **/
			Graphics* graphics;

			/**
			 * Input
			 **/
			Input* input;

			/**
			 * For better performance, limit the times that the GUI can update and
			 * render every second.
			 * Note that update() should be called as often as possible, as this
			 * is required for responsive inputs etc, but any graphical updates
			 * will be limited to a slower speed. 30fps is the default.
			 * @param fps  		Set maximum update frequency in fps. 30 is default. Set to 0 for no limit.
			 */
			void maxUpdateFrequency( uint8_t fps );

			/**
			 * Update the GUI
			 */
			void update();

			/**
			 * Render the GUI
			 */
			void render();

			/**
			 * Listen for event messages
			 * @param  event The event of the message
			 * @param  data  Any user dtaa registered with the listener
			 * @return       Keep the listener?
			 */
			boolean listen( uint32_t event, void* data ) override;

		protected:

			/**
			 * Called from constructor
			 */
			void _init( Style* style = 0 );
			
			/**
			 * Flag if we are internally managing memory for certain objects
			 */
			boolean _manageGraphics = false;
			boolean _manageStyle = false;

			/**
			 * Timing
			 */
			uint32_t _minDeltaMicros = 1/30 * 1000000; // 30fps
			uint32_t _thisMicros;
			uint32_t _lastMicros;
			uint32_t _deltaMicros;
			float _deltaSecs;
			
	};
	
} // namespace

#endif