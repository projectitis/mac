/**
 * GUI library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/
 
#include "ClipRect.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	/**
	 * Constructor
	 */
	ClipRect::ClipRect(){
		clear();
	}
	ClipRect::ClipRect( int16_t px, int16_t py, int16_t w, int16_t h ){
		setPosAndSize( px, py, w, h );
	}

	/**
	 * Clear back to an empty rect
	 */
	void ClipRect::clear() {
		x = 0; y = 0;
		x2 = -1; y2 = -1;
		width = 0; height = 0;
	} 

	/**
	 * Set (copy) the size of the rect from the supplied rect
	 * @param rect   	The rect to set from
	 */
	void ClipRect::set( ClipRect* rect ) {
		x = rect->x;
		y = rect->y;
		x2 = rect->x2;
		y2 = rect->y2;
		width = rect->width;
		height = rect->height;
	}

	/**
	 * Set the top-left and bottom-right corners of rect.
	 * Adjusts width and height.
	 * @param px   	X coordinate of top-left corner 
	 * @param py 	Y coordinate of top-left corner
	 * @param px2	X coordinate of bottom-right corner
	 * @param py2  	Y coordinate of bottom-right corner
	 */
	void ClipRect::set( int16_t px, int16_t py, int16_t px2, int16_t py2 ){
		if (px2<px){
			x = px2;
			x2 = px;
		}
		else{
			x = px;
			x2 = px2;
		}
		if (py2<py){
			y = py2;
			y2 = py;
		}
		else{
			y = py;
			y2 = py2;
		}
		width = x2 - x + 1;
		height = y2 - y + 1;
	}

	/**
	 * Set width of rect
	 * @param w The width
	 */
	void ClipRect::setWidth( uint16_t w ){
		if (w<0){
			x += w;
			width = -w;
		}
		else{
			width = w;
		}
		x2 = x + width - 1;
	}

	/**
	 * Set height of rect
	 * @param h The height
	 */
	void ClipRect::setHeight( uint16_t h ){
		if (h<0){
			y += h;
			height = -h;
		}
		else{
			height = h;
		}
		y2 = y + height - 1;
	}

	/**
	 * Set the top-left corner of rect.
	 * Adjusts bottom-right corner
	 * @param px   	X coordinate of top-left corner 
	 * @param py 	Y coordinate of top-left corner
	 **/
	void ClipRect::setPos( int16_t px, int16_t py ){
		x = px;
		y = py;
		x2 = x + width - 1;
		y2 = y + height - 1;
	}

	/**
	 * Set the bottom-right corner of rect.
	 * Adjusts width and height
	 * @param px   	X coordinate of bottom-right corner 
	 * @param py 	Y coordinate of bottom-right corner
	 **/
	void ClipRect::setPos2( int16_t px, int16_t py ){
		x2 = px;
		y2 = py;
		if (x2>x) x2 = x - 1;
		if (y2>y) y2 = y - 1;
		width = x2 - x + 1;
		height = y2 - y + 1;
	}

	/**
	 * Set the width and height of the rect
	 * Adjusts x2, y2
	 * @param w   	Width of rect
	 * @param h 	Height of rect
	 **/
	void ClipRect::setSize( int16_t w, int16_t h ){
		setWidth( w );
		setHeight( h );
	}

	void ClipRect::setPosAndSize( int16_t px, int16_t py, int16_t w, int16_t h ){
		if (w<0){
			px += w;
			w = -w;
		}
		if (h<0){
			py += h;
			h = -h;
		}
		x = px; y = py;
		width = w; height = h;
		x2 = x + width - 1; y2 = y + height - 1;
	}
	
	/**
	 * Return true if the clip rect contains the specified point
	 * @param  x  	X coord to test
	 * @param  y 	Y coord to test
	 * @return   True if point within rect
	 */
	boolean ClipRect::contains( int16_t px, int16_t py ){
		return ((px>=x) && (px<= x2) && (py >= y) && (py <= y2));
	}

	/**
	 * Return true if the clip rect contains the specified X coord
	 * @param  px  	X coord to test
	 * @return   True if point within rect
	 */
	boolean ClipRect::containsX( int16_t px ){
		return ((px>=x) && (px<= x2));
	}

	/**
	 * Return true if the clip rect contains the specified Y coord
	 * @param  px  	Y coord to test
	 * @return   True if point within rect
	 */
	boolean ClipRect::containsY( int16_t py ){
		return ((py >= y) && (py <= y2));
	}

	/**
	 * Check if the clip rect is empty (has a 0 width or height)
	 * @return True if the rect is empty
	 */
	boolean ClipRect::isEmpty(){
		return (width==0) || (height==0);
	}

	/**
	 * Check if any part of another rect overlaps this one
	 * @param rect The other rect
	 * @return boolean True if any part of the rects overlap
	 */
	boolean ClipRect::overlaps( ClipRect* rect ){
//Serial.println("ClipRect::overlaps");
//Serial.printf("  This: %d,%d %dx%d\n", this->x,this->y,this->width,this->height);
//Serial.printf("  Rect: %d,%d %dx%d\n", rect->x,rect->y,rect->width,rect->height);
		if (isEmpty() || rect->isEmpty()) return false;
		if (rect->x2 < x) return false;
		if (rect->x > x2) return false;
		if (rect->y2 < y) return false;
		if (rect->y > y2) return false;
		return true;
	}

	/**
	 * Clip this rect to another rect (intersection)
	 * @param  rect  	The rect to clip to
	 */
	void ClipRect::clip( ClipRect* rect ){
		if (x < rect->x) x = rect->x;
		if (x2 > rect->x2) x2 = rect->x2;
		if (y < rect->y) y = rect->y;
		if (y2 > rect->y2) y2 = rect->y2;
		if (x>x2){
			x2 = x - 1;
		}
		width = x2 - x + 1;
		if (y>y2){
			y2 = y - 1;
		}
		height = y2 - y + 1;
	}

	/**
	 * Clip this rect to another rect (intersection)
	 * @param  rect  	The rect to clip to
	 */
	void ClipRect::clipPosAndSize( int16_t px, int16_t py, int16_t w, int16_t h ){
		if (x < px) x = px;
		if (x2 >= (px+w)) x2 = px + w - 1;
		if (y < py) y = py;
		if (y2 >= (py+h)) y2 = py + h - 1;
		if (x>x2){
			x2 = x - 1;
		}
		width = x2 - x + 1;
		if (y>y2){
			y2 = y - 1;
		}
		height = y2 - y + 1;
	}

	/**
	 * Expand this rect to also encompase the specified rect (union)
	 * @param rect The rect to encompase
	 */
	void ClipRect::grow( ClipRect* rect ){
		if (isEmpty()){
			set( rect );
			return;
		}
		if (x > rect->x) x = rect->x;
		if (x2 < rect->x2) x2 = rect->x2;
		if (y > rect->y) y = rect->y;
		if (y2 < rect->y2) y2 = rect->y2;
		width = x2 - x + 1;
		height = y2 - y + 1;
	}

	/**
	 * Expand this rect to also encompase a rectangular area specified by position and size (union)
	 * @param  px  	X coord to clip
	 * @param  py 	Y coord to clip
	 * @param  w  	Width
	 * @param  h 	Height
	 */
	void ClipRect::growPosAndSize( int16_t px, int16_t py, int16_t w, int16_t h ) {
		if (isEmpty()){
			set( px, py, w, h );
			return;
		}
		if (x > px) x = px;
		if (x2 <= (px+w)) x2 = px + w - 1;
		if (y > py) y = py;
		if (y2 <= (py+h)) y2 = py + h - 1;
		width = x2 - x + 1;
		height = y2 - y + 1;
	}
	
} // namespace