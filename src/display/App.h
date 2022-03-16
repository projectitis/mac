/**
 * App framework for "mac/μac"
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
#ifndef _MAC_APPH_
#define _MAC_APPH_ 1

#include "display/Display.h"
#include "display/Stage.h"
#include "display/Messenger.h"
#include "physics/Tween.h"
#include "input/Input.h"

 /**
  * mac (or μac) stands for "Microprocessor App Creator"
  * mac is a project that enables creating beautiful and useful apps on the
  * Teensy microprocessor, but hopefully is generic enough to be ported to other
  * microprocessor boards. The various libraries that make up mac might also
  * be useful in other projects.
  **/
namespace mac {

	/**
	 * App framework. An app manages:
	 * 		Input (@see Input.h)
	 * 		Messages (@see Messenger.h)
	 * 		Tweens (@see Tween.h)
	 * and can optionally manage:
	 * 		Graphics and display (@see Graphics.h)
	 */
	class App : public Listener {

	public:

		/**
		 * Constructor for app without any graphical elements.
		 **/
		App();

		/**
		 * Constructor with display adapter object
		 * @param	display			A Display instance for the hardware display being used.
		 **/
		App( Display* display );

		/**
		 * Destructor
		 **/
		~App();

		/**
		 * The hardware display object
		 */
		Display* display;

		/**
		 * The drawing buffer object
		 */
		LineBuffer* buffer;

		/**
		 * The stage for rendering graphics
		 */
		Stage* stage;

		/**
		 * Input
		 **/
		Input* input;

		/**
		 * Listener for messages/events
		 */
		Messenger* messenger;

		/**
		 * Tween object for animations
		 */
		Tween* tweens;

		/**
		 * Set target FPS for rendering functions. This includes
		 * tweening and updating the display.
		 * @param 	fps 	Default 30
		 */
		void setRenderFPS( uint16_t fps );

		/**
		 * Update the app
		 */
		void update();

		/**
		 * Initialise serial for debugging messages
		 * @param boolean If true, wait until serial object is ready (maximum of 2 seconds)
		 */
		void serialBegin( boolean waitUntilReady = false );

	protected:

		/**
		 * Called from constructor
		 */
		void _init();

		/**
		 * Timing
		 */
		uint32_t _renderDeltaMicrosMax = ( 1.0 / 30.0 ) * 1000000; // 30fps
		uint32_t renderDeltaMicros = 0;
		float_t renderDeltaSecs = 0;
		uint32_t thisMicros;
		uint32_t lastMicros;
		uint32_t deltaMicros;
		float_t deltaSecs;

	};

} // namespace

#endif