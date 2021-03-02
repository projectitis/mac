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
#ifndef _MAC_MESSENGERH_
#define _MAC_MESSENGERH_ 1

#include "Common.h"
#include "LinkedList.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	/**
	 * Common events. Define any user events from 256 onward.
	 */
	typedef enum {
		input_next				= 1,	// Core input events
		input_previous			= 2,
		input_back				= 3,
		input_action			= 4,

		input_up				= 5,	// Secondary input events for movement
		input_down				= 6,
		input_left				= 7,
		input_right				= 8,

		input_action1			= 4,	// Secondary input events for buttons/actions
		input_action2			= 9,
		input_action3			= 10,
		input_action4			= 11,

		input_select			= 12,	// Specialised input events
		input_start 			= 13,
		input_key				= 14,
		input_close				= 15,
		input_quit				= 16,

		action_save				= 30,	// Common actions
		action_load				= 31,
		action_reset			= 32,
		action_home				= 33,
		action_end				= 34,
		action_pgup				= 35,
		action_pgdn				= 36,

		action_play				= 50,	// Media actions (also see action_home, action_end)
		action_stop				= 51,
		action_pause			= 51,
		action_rewind			= 52,
		action_fastforward		= 53,

		update_all				= 60,	// Update events
		update_render			= 61
	} Event;

	/**
	 * Listener. An object that is listening for a message.
	 */
	class Listener{
		public:
			/**
			 * Called when a message is sent to the listener. If the listen method
			 * returns false, the listener is removed. If it returns true, the the
			 * listener remains in place.
			 * @param  event       The event of the message
			 * @param  messageData Any user message data that was registered with the messenger
			 * @return             Keep the listener?
			 */
			virtual boolean listen( uint32_t event, void* messageData ){ return true; }
	};

	/**
	 * Listener item. Holds details of a single listener in a linked list.
	 */
	class ListenerItem: public LinkedList {
		public:
			ListenerItem( uint32_t event, Listener* listener, void* messageData );

			uint32_t event;
			Listener* listener;
			void* messageData;
	};

	/**
	 * Timer item. A special listener that is triggered on a timer
	 */
	class TimerItem: public ListenerItem {
		public:
			TimerItem( uint32_t event, float triggerTime, Listener* listener, void* messageData );

			float triggerTime;
			float currentTime;
	};
	
	/**
	 * Listener functionality (events and callbacks). The listen method is called on the Listener
	 * when an event it is interested in is fired.
	 */
	class Messenger {
		
		public:
			/**
			 * Add an event listener
			 */
			void addListener( uint32_t event, Listener* listener, void* messageData = 0 );

			/**
			 * Remove an event listener
			 */
			void removeListener( uint32_t event, Listener* listener );

			/**
			 * Remove all listeners for a specific event
			 */
			void removeListenersForEvent( uint32_t event );

			/**
			 * Add a timer
			 */
			void addTimer( uint32_t event, float triggerTime, Listener* listener, void* messageData = 0 );

			/**
			 * Remove a timer
			 */
			void removeTimer( uint32_t event, Listener* listener );

			/**
			 * Remove all timers for a specific event
			 */
			void removeTimersForEvent( uint32_t event );

			/**
			 * Trigger an message. This calls the Listener::listen method on the listener. If the method
			 * returns true, the listener remains in place. If the method returns false, the listener
			 * is removed.
			 * Events 0-255 are reserved by mac. Events >255 are user events
			 */
			void sendMessage( uint32_t event );

			/**
			 * Update the messenger to check for timers
			 * @param dt Time since last call in seconds
			 */
			void update( float dt );

		private:

			/**
			 * List of listeners
			 */
			ListenerItem* _listeners = 0;

			/**
			 * List of timers
			 */
			TimerItem* _timers = 0;

	};
	
} // namespace

#endif