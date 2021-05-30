/**
 * GUI library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/
 
#include "Gradient.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	/**
	 * @brief Construct a new Gradient Stop object
	 * @param color The color
	 * @param alpha The alpha
	 * @param position The position
	 */
	GradientStop::GradientStop( color888 color, float alpha, float position ) {
		set( color, alpha, position );
	}
	void GradientStop::set( color888 color, float alpha, float position ) {
		this->color = color;
		this->alpha = alpha;
		this->position = position;
		r = (float)((this->color >> 16) & 0xff);
		g = (float)((this->color >> 8) & 0xff);
		b = (float)((this->color >> 0) & 0xff);
		a = alpha;
	}
	void GradientStop::update() {
		if (!next) return;
		float dp = next->position - position;
		dr = (next->r - r) / dp;
		dg = (next->g - g) / dp;
		db = (next->b - b) / dp;
		da = (next->a - a) / dp;
Serial.printf("Update\n  To next %f\n  %f %f %f\n", dp, dr, dg, db);
	}

	/**
	 * @brief The resuse pool
	 */
	Gradient* Gradient::pool = 0;

	/**
	 * Create a new object or take one from the pool
	 * @return The new or recycled object
	 */
	Gradient* Gradient::Create(){
		Gradient* g;
		if (Gradient::pool){
			g = Gradient::pool;
			Gradient::pool = g->_poolNext;
		}
		else{
			g = new Gradient();
		}
		return g;
	}

	/**
	 * Return this object to the pool
	 */
	void Gradient::recycle(){
#ifdef MAC_OBJECT_REUSE
		this->reset();
		this->_poolNext = Gradient::pool;
		Gradient::pool = this;
#else
		delete this;
#endif
	}

	/**
	 * @brief Construct a new Gradient object
	 */
	Gradient::Gradient() {}

	/**
	 * @brief Destroy the Gradient object
	 * Likely never happens (recycle should be called instead)
	 */
	Gradient::~Gradient() {
		reset();
	}

	/**
	 * Reset the object back to default settings
	 */
	void Gradient::reset(){
		// Reset position
		set(0,0,1.0,0);
		// Remove stops
		GradientStop* stop = stops;
		while (stop) {
			stop = stops->next;
			delete stops;
			stops = stop;
		}
	}

	/**
	 * @brief Specify the start and end points
	 */
	void Gradient::set( float x, float y, float x2, float y2 ) {
		if (x2 < x) {
			_x = x2;
			_x2 = x;
			_y = y2;
			_y2 = y;
		}
		else {
			_x = x;
			_x2 = x2;
			_y = y;
			_y2 = y2;
		}
	}

	/**
	 * @brief Set a gradient stop
	 * 
	 * @param color The color of the stop
	 * @param alpha The alpha at the stop
	 * @param position The position along the gradient (0.0 - 1.0)
	 */
	void Gradient::setStop( color888 color, float alpha, float position ) {
		alpha = alphaClamp( alpha );
		position = alphaClamp( position );
		// Very first stop
		if (!stops) {
Serial.println("First stop");
			stops = new GradientStop( color, alpha, position );
			return;
		}
		GradientStop* nextStop = stops;
		GradientStop* prevStop = 0;
		while (nextStop) {
			// if stops are the same position, replace existing
			if (nextStop->position == position) {
Serial.println("Replace stop");
				nextStop->set( color, alpha, position );
				nextStop->update();
				return;
			}
			// If the next stop is higher, insert before
			else if (nextStop->position > position) {
				GradientStop* stop = new GradientStop( color, alpha, position );
				// Insert as first in row
				if (!prevStop) {
Serial.println("Insert at start");
					stop->next = this->stops;
					this->stops = stop;
					stop->update();
					return;
				}
				// Insert before this one
Serial.println("Insert");
				prevStop->next = stop;
				stop->next = nextStop;
				stop->update();
				return;
			}
			prevStop = nextStop;
			nextStop = nextStop->next;
		}
		// Insert at end
Serial.println("Insert at end");
		prevStop->next = new GradientStop( color, alpha, position );
		prevStop->update();
	}

	/**
	 * @brief Begin the render sweep for the current frame
	 * @param updateArea The area of the display being updated. Unlike display objects, this must be in LOCAL coordinates
	 */
	void Gradient::beginRender( ClipRect* updateArea ) {
		// Calculate gradient and magnitude
		float dx = _x2 - _x;
		float dy = _y2 - _y;
		_m = sqrtf( dx*dx + dy*dy );
		_a = atan2f( -dy, -dx );
		// Calculate x step
		// XXX: Will be based on angle
		_xStep = 1.0 / (float)dx;
Serial.printf("_xStep:%f\n", _xStep);
		_yStep = 1.0 / (float)abs(dy);
		_updateX = updateArea->x;
		_pos = -_yStep;
	}

	/**
	 * Prepare to render the next line
	 * @param ry The y position in local coordinates
	 */
	void Gradient::beginLine( int16_t ry ) {
		_pos = 0;

		// Step all stops and reset
		activeStop = stops;
		while (activeStop) {
			activeStop->set( activeStop->color, activeStop->alpha, activeStop->position );
			activeStop = activeStop->next;
		}

		// Calculate the stop to start from
		activeStop = stops;
		while (activeStop->next) {
			if (activeStop->next->position > _pos) break;
			activeStop = activeStop->next;
		}
	}

	/**
	 * Read a pixel from the sprite and advance position
	 * @param c (out) color
	 * @param a (out) alpha
	 */
	void Gradient::calcPixel( int16_t rx, int16_t ry ) {
		_rc = (((uint8_t)activeStop->r) << 16) | (((uint8_t)activeStop->g) << 8) | (uint8_t)activeStop->b;
		_ra = activeStop->a;
		if (activeStop->next) {
			_pos += _xStep;
			float dp = _pos - activeStop->position;
			activeStop->r += activeStop->dr * _xStep;
			activeStop->g += activeStop->dg * _xStep;
			activeStop->b += activeStop->db * _xStep;
			activeStop->a += activeStop->da * _xStep;
				/*
			if (_pos >= activeStop->next->position){
				while (activeStop->next && (_pos >= activeStop->next->position)){
					float dp = _pos - activeStop->next->position;
					activeStop = activeStop->next;
				}
			}
			else {
				float dp = _pos - activeStop->position;
				activeStop->r += activeStop->dr * dp;
				activeStop->g += activeStop->dg * dp;
				activeStop->b += activeStop->db * dp;
				activeStop->a += activeStop->da * dp;
			}
			*/
		}
	}

	void Gradient::skipPixel( int16_t rx, int16_t ry ){
		_pos += _xStep;
	}
	
} // namespace