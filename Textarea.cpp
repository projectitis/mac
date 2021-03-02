/**
 * GUI library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/
 
#include "Textarea.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	/**
	 * Create a new Textarea or take one from the pool
	 * @return The new or recycled text area
	 */
	Textarea* Textarea::create(){
		Textarea* widget;
		if (Textarea::_pool){
			widget = Textarea::_pool;
			Textarea::_pool = widget->_poolNext;
		}
		else{
			widget = new Textarea();
		}
		return widget;
	}

	/**
	 * Return this widget to the pool
	 * @param widgt [description]
	 */
	void Textarea::recycle(){
		this->reset();
		if (!Textarea::_pool){
			Textarea::_pool = this;
		}
		else{
			Textarea* widget = Textarea::_pool;
			while (widget->_poolNext) widget = widget->_poolNext;
			widget->_poolNext = this;
		}
	}

	/**
	 * Reset the widget back to default settings
	 */
	void Textarea::reset(){
		_rect.setPosAndSize(0,0,0,0);
		_font = style->font;
		_lineHeight = 1.5;
		_color = RGB565_White;
		_alpha = 1;
		_align = ALIGN_LEFT;
		_poolNext = 0;
	}
	
} // namespace