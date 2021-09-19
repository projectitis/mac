/**
 * GUI library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/
 
#include "../Tween.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	/**
	 * Create the Tween item
	 */
	TweenItem::TweenItem( float_t* property, float_t from, float_t to, float_t seconds, EasingFunction ease, TweenCallback callback, void* callbackData ){
		this->property = property;

		this->from = from;
		this->change = to-from;
		this->current = 0;
		this->seconds = seconds;
		this->seconds_inv = 1/seconds;
		this->ease = ease;

		this->callback = callback;
		this->callbackData = callbackData;

		*property = from;
	}

	/**
	 * Add an Tween
	 */
	void Tween::addTween( float_t* property, float_t from, float_t to, float_t seconds, EasingFunction ease, TweenCallback callback, void* callbackData ){
		TweenItem* item = new TweenItem( property, from, to, seconds, ease, callback, callbackData );
		if (_tweens) _tweens->before( item );
		_tweens = item;
	}

	/**
	 * Remove all tweens for a property
	 */
	void Tween::removeTween( float_t* property ){
		TweenItem* item = _tweens;
		TweenItem* removedItem;
		while (item){
			// Remove item if property matches
			if (item->property == property){
				removedItem = item;
				item = (TweenItem*)item->remove();
				delete removedItem;
			}
			// Move to next
			else {
				item = (TweenItem*)item->next();
			}
		}
	}

	/**
	 * Trigger an event
	 */
	void Tween::update( float_t dt ){
		TweenItem* item = _tweens;
		TweenItem* removedItem;
		while (item){
			// Progress the tween
			item->current += dt;
			if (item->current > item->seconds) item->current = item->seconds;
			// Calculate value
			*item->property = item->from + item->change * item->ease( item->current * item->seconds_inv );
			// If tween is done, trigger callback then remove it
			if (item->current == item->seconds){
				if (item->callback) item->callback( item->callbackData );
				removedItem = item;
				item = (TweenItem*)item->remove();
				delete removedItem;
			}
			// Move to next
			else {
				item = (TweenItem*)item->next();
			}
		}
	}
	
} // namespace