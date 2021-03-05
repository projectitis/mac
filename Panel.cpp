/**
 * GUI library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/
 
#include "Panel.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	Widget* Panel::pool = 0;

	/**
	 * Constructor
	 */
	Panel::Panel( Style* aStyle ){}

	/**
	 * Pool getter
	 */
	Widget** Panel::_getPool(){
		return &Panel::pool;
	}

	/**
	 * Create a new widget or take one from the pool
	 * @return The new or recycled widget
	 */
	Panel* Panel::Create( Style* aStyle ){
		return (Panel*)Widget::Create<Panel>(aStyle);
	}

	/**
	 * Reset the widget back to default settings
	 */
	void Panel::reset(){
		this->title = 0;
		this->acronym = 0;
		Widget::reset();
	}

	/**
	 * Update and render the display object
	 */
	void Panel::update( float dt ){
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
	boolean Panel::render( Graphics* graphics, boolean force ){
		// Render children
		boolean childRendered = Widget::render( graphics, force );
		// Render self
		if (_dirty || childRendered || force){
			
			// Icon


			_dirty = false;
			return true;
		}
		else{
			return false;
		}
	}

	/**
	 * Set the title and nickname of the panel
	 * @param  title   	The full title of the panel (max 32 chars)
	 * @param  acronym 	The acronym to display when collapsed (max 3 chars)
	 * @return 	Return self for chaining
	 */
	void Panel::setTitle( char* title, char* acronym ){
		this->title = (char*)malloc(sizeof(char) * strlen(title));
		strcpy(this->title, title );
		if (!acronym) acronym = title;
		this->acronym = (char*)malloc(sizeof(char) * strlen(acronym));
		strcpy(this->acronym, acronym );
	}
	
} // namespace