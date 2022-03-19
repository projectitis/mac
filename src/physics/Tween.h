#pragma once
#ifndef _MAC_TWEENH_
#define _MAC_TWEENH_ 1

#include "Common.h"
#include "utils/LinkedList.h"
#include "physics/Easing.h"

/**
 * Property tweening
 *
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project for creating beautiful and useful
 * apps on various microprocessor boards.
 *
 * mac is distributed under the MIT licence
 **/
namespace mac {

	/**
	 * Listener callback. If the callback returns true, the listener remains
	 * in place and continues to listen for future events. If it returns false
	 * the listener will be removed from the list.
	 * @param  void*		Pointer to user data associated with the listener
	 */
	typedef void ( *TweenCallback )( void* );

	/**
	 * Tween item. Holds details of a single tween in a linked list.
	 */
	class TweenItem : public LinkedList<TweenItem> {
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