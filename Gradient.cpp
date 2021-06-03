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
	 *
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
	void GradientStop::reset( float pos, float gStep ) {
		r = (float)((this->color >> 16) & 0xff);
		g = (float)((this->color >> 8) & 0xff);
		b = (float)((this->color >> 0) & 0xff);
		a = alpha;
		if (next && (pos > position) && (pos < next->position)) {
			float dp = (pos - position) / distance;
			r += dr * dp;
			g += dg * dp;
			b += db * dp;
			a += da * dp;
			dStep = gStep / distance;
		}
	}
	void GradientStop::calc() {
		if (!next) return;
		distance = next->position - position;
		dr = (next->r - r);
		dg = (next->g - g);
		db = (next->b - b);
		da = (next->a - a);
	}
	void GradientStop::step() {
		r += dr * dStep;
		g += dg * dStep;
		b += db * dStep;
		a += da * dStep;
	}
	*/

	/**
	 * @brief The resuse pool
	 */
	Gradient* Gradient::pool = 0;

	/**
	 * Create a new object or take one from the pool
	 * @return The new or recycled object
	 */
	Gradient* Gradient::Create( uint8_t numStops ){
		Gradient* g;
		if (Gradient::pool){
			g = Gradient::pool;
			Gradient::pool = g->_poolNext;
		}
		else{
			g = new Gradient();
		}
		g->numStops = numStops;
		g->stops = new GradientStop[numStops];
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
		position(0,0,1.0,0);
		// Remove stops
		if (stops) delete stops[];
		stops = 0;
	}

	/**
	 * @brief Specify the start and end points
	 */
	void Gradient::position( float x, float y, float x2, float y2 ) {
		_x = x;
		_x2 = x2;
		_y = y;
		_y2 = y2;
	}

	/**
	 * @brief Set or change a gradient stop
	 * 
	 * @param color The color of the stop
	 * @param alpha The alpha at the stop
	 * @param position The position along the gradient (0.0 - 1.0)
	 */
	Gradient* Gradient::stop( uint8_t index, color888 color, float alpha, float position ) {
		_needsCalc = true;
		stops[ index ].color = color;
		stops[ index ].alpha = alpha;
		stops[ index ].position = position;
		stops[ index ].r = (float)((color >> 16) & 0xff);
		stops[ index ].g = (float)((color >> 8) & 0xff);
		stops[ index ].b = (float)((color >> 0) & 0xff);
		stops[ index ].a = alpha;
		return this;
	}

	/**
	 * @brief Begin the render sweep for the current frame
	 * @param updateArea The area of the display being updated. Unlike display objects, this must be in LOCAL coordinates
	 */
	void Gradient::beginRender( ClipRect* updateArea ) {
		// Calculate all stop values
		if (_needsCalc) {
			_needsCalc = false;
			uint8_t i = 0;
			uint8_t n = 1;
			while (n < numStops) {
				n = i + 1;
				stops[i].distance = stops[n].position - stops[i].position;
				stops[i].dr = (stops[n].r - stops[i].r);
				stops[i].dg = (stops[n].g - stops[i].g);
				stops[i].db = (stops[n].b - stops[i].b);
				stops[i].da = (stops[n].a - stops[i].a);
				i = n;
			}
		}

		// Calculate gradient, magnitude and angles
		float dx = _x2 - _x;
		float dy = _y2 - _y;

		_len = sqrtf( dx*dx + dy*dy );
		float a = atan2f( -dy, -dx ) - PI/2;
		_cos = -cosf( a );
		_sin = -sinf( a );
		
		// Store edge
		_x0 = updateArea->x;
		// Calculate steps
		_dx = (dx==0)?0:1.0 / (float)dx;
		_dy = (dy==0)?0:1.0 / (float)dy;
		// Calculate the gradient y value at the update edge
		_steep = abs(dy) > abs(dx);
		if (_x == _x2){
			_y0 = updateArea->y;
			_pos0 = (_y0 - _y) * _dy;
		}
		else {
			float m = dy/dx;
			_y0 = _y + m * (_x0 - _x);
			_pos0 = (_x0 - _x) * _dx;
		}
Serial.printf(" dx=%f dy=%f\n", _dx, _dy);
	}

	/**
	 * Prepare to render the next line
	 * @param ry The y position in local coordinates
	 */
	void Gradient::beginLine( int16_t ry ) {
//Serial.print("\nbeginLine");
		if (_x == _x2) {
			_pos = (ry - _y0) / _m + _pos0;
		}
		else{
			float i = _x0 + _a * (ry - _y0);
			_pos = i / _m + _pos0;
		}
Serial.printf(" %d: pos:%f\n", ry, _pos);

		// Calculate the first stop on this line
		activeStop = stops;
		int n = 0;
		while (activeStop->next) {
			if (activeStop->next->position > _pos) break;
			activeStop = activeStop->next;
			n++;
		}
		activeStop->reset( _pos, _steep?_dy:_dx );
//Serial.printf(" stop:%d\n",n);
	}

	/**
	 * Read a pixel from the sprite and advance position
	 * @param c (out) color
	 * @param a (out) alpha
	 */
	void Gradient::calcPixel( int16_t rx, int16_t ry ) {
//Serial.printf("  %d:", rx);
		_rc = (((uint8_t)activeStop->r) << 16) | (((uint8_t)activeStop->g) << 8) | (uint8_t)activeStop->b;
		_ra = activeStop->a;
		if (activeStop->next) {
			_pos += _steep?_dy:_dx;
//Serial.printf(" %f", _pos);
			if (_pos >= activeStop->next->position) {
				while (activeStop->next && (_pos >= activeStop->next->position)){
					activeStop = activeStop->next;
//Serial.print(" stop");
				}
				activeStop->reset( _pos, _steep?_dy:_dx );
			}
			else if (_pos >= 0.0) {
				activeStop->step();
			}
		}
//Serial.println();
	}

	void Gradient::skipPixel( int16_t rx, int16_t ry ){
		_pos += _steep?_dy:_dx;
	}
	
} // namespace