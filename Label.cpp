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
	Label::Label( Style* aStyle ) : Widget( aStyle ){}

	/**
	 * Pool getter
	 */
	Widget** Label::_getPool(){
		return &Label::pool;
	}

	/**
	 * Create a new widget or take one from the pool
	 * @return The new or recycled widget
	 */
	Label* Label::Create( Style* aStyle ){
		return (Label*)Widget::Create<Label>( aStyle );
	}

	/**
	 * Reset the widget back to default settings
	 */
	void Label::reset(){
		this->_font = 0;
		this->_text = 0;
		this->_color = 0;
		this->_alpha = 1.0;
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
		// Render self
		boolean selfRendered = false;
		if (_dirty || force){
			if (!this->_font && style) this->_font = style->labelFont;
			if (this->_font && this->_text) {
				graphics->text->setFont( this->_font );
				graphics->text->setColor( this->_color );
				graphics->text->setAlpha( this->_alpha );
				graphics->text->setTextArea( x, y, 100, 20 ); // XXX: Proper W and H		
				graphics->text->printTextArea( this->_text );
				selfRendered = true;
			}
			_dirty = false;
		}
		// Render children
		return Widget::render( graphics, force ) || selfRendered;
	}

	/**
	 * Set the font to use for the label
	 * @param  f   	The font
	 */
	void Label::setFont( packedbdf_t* f ) {
		this->_font = f;
		_dirty = true;
	}

	/**
	 * Set the text of the label
	 * @param  text   	The text for the label
	 */
	void Label::setText( const char* text ){
		this->_text = (char*)text;
		_dirty = true;
	}

	/**
	 * @brief Set the Color of the label
	 * @param c 	The color
	 */
	void Label::setColor( color888 c ) {
		this->_color = c;
		_dirty = true;
	}

	/**
	 * @brief Set the Alpha of the label
	 * @param a 	The alpha
	 */
	void Label::setAlpha( alpha a ) {
		this->_alpha = alphaClamp( a );
		_dirty = true;
	}
	
} // namespace