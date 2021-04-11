/**
 * GUI library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/
 
#include "Icon.h"
#include "Arduino.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	Widget* Icon::pool = 0;

	/**
	 * Constructor
	 */
	Icon::Icon( Style* aStyle ) : Widget( aStyle ){}

	/**
	 * Pool getter
	 */
	Widget** Icon::_getPool(){
		return &Icon::pool;
	}

	/**
	 * Create a new widget or take one from the pool
	 * @return The new or recycled widget
	 */
	Icon* Icon::Create( Style* aStyle ){
		return (Icon*)Widget::Create<Icon>(aStyle);
	}

	/**
	 * Reset the widget back to default settings
	 */
	void Icon::reset(){
		_iconTilemap = NULL;
		_iconIndex = 0;
		_badgeCount = 0;
		_iconAlpha = 1.0;
		_iconRotation = BitmapRotation::none;
		if (style) {
			_badgeColor = style->badgeColor;
			_textColor = style->badgeTextColor;
		} 
		Widget::reset();
	}

	/**
	 * Update and render the display object
	 */
	void Icon::update( float dt ){
		// Update self
		
		// Update children
		Widget::update( dt );
	}

	/**
	 * Render the display object. Usually you would call update which internally calls render.
	 * @param  	graphics 	The graphics object to use for rendering
	 * @param  	force 		If true, will render even if not dirty (force redraw)
	 * @return          Return true if current widget rendered (_dirty or forced)
	 */
	boolean Icon::render( Graphics* graphics, boolean force ){
		// Render children
		boolean childRendered = Widget::render( graphics, force );
		// Render self
		if (_dirty || childRendered || force){

			// if there is no tilemap, use the default
			if (!_iconTilemap) _iconTilemap = style->icons;

			// Draw the icon
			graphics->bitmap->stamp( _iconColor, _iconTilemap, _iconIndex, x, y, _iconAlpha, _iconRotation );

			_dirty = false;
			return true;
		}
		else{
			return false;
		}
	}

	/**
	 * @brief Set icon index
	 * 
	 * @param tilemap	The tilemap that contains the icons
	 * @param index 	The index of the icon in the tilemap
	 */
	void Icon::setIcon( Tilemap* tilemap, uint32_t index ) {
		_iconTilemap = tilemap;
		_iconIndex = index;
		_dirty = true;
	}

	/**
	 * @brief Set icon index
	 * 
	 * @param tilemap	The tilemap that contains the icons
	 * @param index 	The index of the icon in the tilemap
	 * @param color 	The color of the icon
	 */
	void Icon::setIcon( Tilemap* tilemap, uint32_t index, color888 color ) {
		setIcon( tilemap, index );
		_iconColor = color;
	}

	/**
	 * @brief Set icon index
	 * 
	 * @param index 	The index of the icon in the tilemap
	 */
	void Icon::setIcon( uint32_t index ) {
		_iconIndex = index;
		_dirty = true;
	}

	/**
	 * @brief Set icon color
	 * 
	 * @param color 	The color of the icon
	 */
	void Icon::setColor( color888 color ) {
		_iconColor = color;
		_dirty = true;
	}

	/**
	 * @brief Set icon index and color
	 * 
	 * @param index 	The index of the icon in the tilemap
	 * @param color 	The color of the icon
	 */
	void Icon::setIcon( uint32_t index, color888 color ) {
		_iconIndex = index;
		_iconColor = color;
	}

	/**
	 * Set a badge
	 * @param  count   		The number in the badge (1-99). If 0, the badge will be hidden
	 */
	void Icon::setBadge( uint8_t count ) {
		if ( _badgeCount == count ) return;
		_badgeCount = count;
		_dirty = true;
	}

	/**
	 * Set a badge
	 * @param  count   		The number in the badge (1-99). If 0, the badge will be hidden
	 * @param  badgeColor 	The color of the badge background circle
	 * @param  textColor 	The color of the badge text (count)
	 */
	void Icon::setBadge( uint8_t count, color888 badgeColor, color888 textColor ) {
		if ( _badgeCount == count ) return;
		_badgeCount = count;
		_badgeColor = badgeColor;
		_textColor = textColor;
		_dirty = true;
	}
	
} // namespace