/**
 * GUI library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/
 
#include "label.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	Widget* Label::pool = 0;

	/**
	 * Constructor
	 */
	Label::Label( Style* aStyle ){}

	/**
	 * Pool getter
	 */
	Widget** _getPool(){
		return &Label::pool;
	}

	/**
	 * Create a new widget or take one from the pool
	 * @return The new or recycled widget
	 */
	Label* Label::Create( Style* aStyle ){
		return (Label*)Widget::Create<Label>(aStyle);
	}

	/**
	 * Reset the widget back to default settings
	 */
	void Label::reset(){
		this->text = 0;
		Widget::reset();
	}

	/**
	 * Update and render the display object
	 */
	void Label::update( float dt ){

	}

	/**
	 * Render the display object.
	 * @param 	graphics 	The graphics object to use for rendering
	 * @param  	force 		If true, will render even if not dirty (force redraw)
	 * @return          Return true if current widget rendered (_dirty or forced)
	 */
	boolean Label::render( Graphics* graphics, boolean force ){
		return false;
	}

	/**
	 * Set the text of the label
	 * @param  text   	The text for the label
	 */
	void Label::setText( const char* text ){
		this->text = (char*)text;
		_dirty = true;
	}
	
} // namespace