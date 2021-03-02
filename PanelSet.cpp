/**
 * GUI library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/
 
#include "PanelSet.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	Widget* PanelSet::pool = 0;

	/**
	 * Constructor
	 */
	PanelSet::PanelSet( Style* aStyle ){
		_activePanel = 0;
		_menuOpen = false;
	}

	/**
	 * Pool getter
	 */
	Widget** PanelSet::_getPool(){
		return &PanelSet::pool;
	}

	/**
	 * Create a new widget or take one from the pool
	 * @return The new or recycled widget
	 */
	PanelSet* PanelSet::Create( Style* aStyle ){
		return (PanelSet*)Widget::Create<PanelSet>(aStyle);
	}

	/**
	 * Update and render the display object
	 */
	void PanelSet::update( float dt ){
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
	boolean PanelSet::render( Graphics* graphics, boolean force ){
		// Render self
		boolean selfRendered = false;
		if (_dirty || force){
			if (_menuOpen) _menuWidth = 80.0;
			else _menuWidth = 40.0;
			_drawBackground( graphics );
			this->_drawArrowRight( graphics, 20 );
			this->_drawArrowLeft( graphics, 60 );
			_dirty = false;
			selfRendered = true;
		}
		// Render children
		boolean childRendered = Widget::render( graphics, force );

		return selfRendered || childRendered;
	}

	/**
	 * Add a new panel and return it
	 * @param  title   	The full title of the panel (max 32 chars)
	 * @param  acronym 	The acronym to display when collapsed (max 3 chars)
	 * @return          The new panel
	 */
	Panel* PanelSet::addPanel( char* title, char* acronym ){
		Panel* panel = Panel::Create( this->style );
		panel->setTitle( title, acronym );
		panel->id = _getId( panel->acronym );
		panel->messenger = this->messenger;
		panel->tweens = this->tweens;
		addChild(panel);
		if (!_activePanel) _activePanel = panel;
		return panel;
	}

	/**
	 * Return a panel by acronym. Will return the first match
	 * @param  acronym 	The acronym of the panel
	 * @return         	The panel, or 0
	 */
	Panel* PanelSet::panel( const char* acronym ){
		uint32_t id = _getId( (char*)acronym );
		return (Panel*)getChild( id );
	}

	/**
	 * Calculate an ID based on up to 4 characters of a string
	 */
	uint32_t PanelSet::_getId( char* str ){
		// Calculate id
		uint32_t id = 0;
		char c = 0;
		uint8_t l = 0;
		while (l<4){ // Max of 4 chars
			c = *(str + l);
			if (c == '\0') break; // Exit if we reach null before 4 characters
			id |= (((uint8_t)c) << (l*8));
			l++;
		}
		return id;
	}

	void PanelSet::_drawBackground( Graphics* graphics ){
		uint16_t w = (uint16_t)_menuWidth;
		// Title area
		graphics->rectangle( 0,0, w-2,40, style->screenColor );
		graphics->lineV( w-2,0, 40, style->panelBorderColor[0] );
		graphics->lineV( w-1,0, 40, style->panelHighlightColor[0] );
		graphics->rectangle( w,0, graphics->display->width-w,40, style->panelBackgroundColor[0] );
		// Title
		graphics->text->setFont( style->labelFont );
		graphics->text->setColor( style->featureColor[0] );
		graphics->text->moveCursorTo( w+12,24 );
		//graphics->text->print( (char*)F("Sprite") );
		graphics->text->moveCursor( 6, 0 );
		graphics->text->setColor( style->labelColor );
		graphics->text->print( _activePanel->title );

		// Menu area
		graphics->lineH( 0,40, w, style->panelBorderColor[0]);
		graphics->rectangle( 0,41, w-2,graphics->display->height-41, style->panelBackgroundColor[0] );
		graphics->lineV( w-2,40, graphics->display->height-40, style->panelBorderColor[1]);
		graphics->lineV( w-1,40, graphics->display->height-40, style->panelHighlightColor[1]);
		// Panel area
		graphics->lineH( w,40, graphics->display->width-w, style->panelBorderColor[1]);
		graphics->rectangle( w,41, graphics->display->width-w,graphics->display->height-41, style->panelBackgroundColor[1] );

		Panel* p = (Panel*)firstChild();
		uint16_t i = 0;
		while (p) {
			if ( p == _activePanel ) graphics->text->setColor( style->labelColor );
			else graphics->text->setColor( style->labelInactiveColor );
			graphics->text->setTextArea( 12,56+(i*20), w-12,20, mac::ALIGN_LEFT );
			if (_menuOpen) graphics->text->printTextArea( p->title );
			else graphics->text->printTextArea( p->acronym ); // p->acronym 
			p = (Panel*)p->next();
			i++;
		}
	}

	/**
	 * Draw the arrow for title
	 **/
	void PanelSet::_drawArrowRight( Graphics* graphics, int16_t y ){
		uint16_t x = (uint16_t)_menuWidth-2;
		int16_t px, dx;
		int16_t py = y - 3;
		for (px=-3; px<4; px++){
			dx = 4-abs(px);
			graphics->lineH( x,py, dx, style->screenColor );
			graphics->pixel( x+dx, py, style->panelBorderColor[0] );
			graphics->pixel( x+dx+1, py, style->panelHighlightColor[0] );
			py++;
		}
	}

	/**
	 * Draw the arrow for the active menu
	 **/
	void PanelSet::_drawArrowLeft( Graphics* graphics, int16_t y ){
		uint16_t x = (uint16_t)_menuWidth - 1;
		int16_t px, dx;
		int16_t py = y - 3;
		for (px=-3; px<4; px++){
			dx = 4-abs(px);
			graphics->pixel( x-dx-1, py, style->panelBorderColor[1] );
			graphics->pixel( x-dx, py, style->panelHighlightColor[1] );
			graphics->lineH( x-dx+1, py, dx, style->panelBackgroundColor[1] );
			py++;
		}
	}
	
} // namespace