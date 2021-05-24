/**
 * GUI library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/
 
#include "GlyphInfo.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	/**
	 * @brief Instantiate the pool for object re-use
	 */
	GlyphInfo* GlyphInfo::pool = 0;

	/**
	 * @brief Construct the glyph with a character and position
	 * 
	 * @param font 	The font
	 * @param c 	The character code
	 * @param x 	The x position
	 * @param baseline The y position of the baseline
	 */
	GlyphInfo::GlyphInfo( char c ) {
		code = c;
		bounds = new ClipRect();
	}

	GlyphInfo::~GlyphInfo() {
		delete bounds;
	}

	/**
	 * Return this object to the pool
	 */
	void GlyphInfo::recycle(){
#ifdef MAC_OBJECT_REUSE
		// Reset vars
		bounds->clear();
		_next = 0;
		_prev = 0;
		lineRepeat = 0;
		dataOffset = 0;
		renderStarted = false;

		// Add back to pool
		this->_poolNext = GlyphInfo::pool;
		GlyphInfo::pool = this;
#else
		delete this;
#endif
	}

	/**
	 * @brief Set the glyph to a new character and position
	 * 
	 * @param c 	The character code
	 */
	void GlyphInfo::set( char c ) {
		code = c;
	}

	/**
	 * Return the next item
	 */
	GlyphInfo* GlyphInfo::next(){
		return (GlyphInfo*)LinkedList::next();
	}

	/**
	 * Remove the current node from teh display list
	 * @return The removed node (self)
	 */
	GlyphInfo* GlyphInfo::remove() {
		return (GlyphInfo*)LinkedList::remove();
	}
	
} // namespace