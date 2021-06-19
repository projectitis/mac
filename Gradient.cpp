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
	 * @brief The resuse pool
	 */
	GradientStop* GradientStop::pool = 0;

	/**
	 * Create a new object or take one from the pool
	 * @return The new or recycled object
	 */
	GradientStop* GradientStop::Create(){
		GradientStop* g;
		if (GradientStop::pool){
			g = GradientStop::pool;
			GradientStop::pool = g->_poolNext;
		}
		else{
			g = new GradientStop();
		}
		return g;
	}

	/**
	 * Return this object to the pool
	 */
	void GradientStop::recycle(){
#ifdef MAC_OBJECT_REUSE
		this->reset();
		this->_poolNext = GradientStop::pool;
		GradientStop::pool = this;
#else
		delete this;
#endif
	}

	/**
	 * Reset the object back to default settings
	 */
	void GradientStop::reset(){
		color = 0;
		alpha = 1.0;
		position = 0.0;
		distance = 0.0;
		step = 0.0;
		r = 0.0;
		g = 0.0;
		b = 0.0;
		a = 0.0;
		dr = 0.0;
		dg = 0.0;
		db = 0.0;
		da = 0.0;
	}

	/**
	 * @brief Start the stop :)
	 */
	void GradientStop::start( float pos ) {
		r = (float)((color >> 16) & 0xff);
		g = (float)((color >> 8) & 0xff);
		b = (float)((color >> 0) & 0xff);
		a = this->alpha;
		if ((pos > position) && (pos< (position+distance))) {
			float s = (pos-position) * step;
			r += dr * s;
			g += dg * s;
			b += db * s;
			a += da * s;
		}
	}

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
		numStops = max(2, numStops); // Ensure at least 2 stops
		g->numStops = numStops;
		g->stops = new GradientStop*[numStops];
		while (numStops--) g->stops[numStops] = GradientStop::Create();
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
		if (stops) {
			while (numStops--) stops[numStops]->recycle();
			delete[] stops;
		}
		stops = 0;
	}

	/**
	 * @brief Specify the start and end points
	 */
	void Gradient::position( float x, float y, float x2, float y2 ) {
		_needsCalc = true;
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
		stops[index]->color = color;
		stops[index]->alpha = alpha;
		stops[index]->position = position;
		return this;
	}

	/**
	 * @brief Begin the render sweep for the current frame
	 * @param updateArea The area of the display being updated. Unlike display objects, this must be in LOCAL coordinates
	 */
	void Gradient::beginRender( ClipRect* updateArea ) {
//Serial.printf("beginRender %d,%d %dx%d\n", updateArea->x,updateArea->y, updateArea->width, updateArea->height );
		// Calculate all stop values
		if (_needsCalc) {
			_needsCalc = false;

			dx = _x2 - _x;
			dy = _y2 - _y;
			steep = abs(dy) > abs(dx);

			// Reverse stops if required
			uint8_t n = 0;
			if (dx<0) {
				swap( _x, _x2 );
				swap( _y, _y2 );
				dx = -dx;
				dy = -dy;

				if (!reverse) {
					reverse = true;
					GradientStop* t;
					while (n < (numStops >> 1)) {
						t = stops[n];
						stops[n] = stops[numStops-n-1];
						stops[numStops-n-1] = t;
						n++;
					}
					n = 0;
				}
			}

			// Calculate stops
			uint8_t i = 0;
			n = 1;
			stops[0]->start( 0 );
			while (i < numStops-1) {
				stops[i]->distance = stops[n]->position - stops[i]->position;
				stops[i]->step = 1.0/stops[i]->distance;
				stops[n]->start( 0 );
				stops[i]->dr = (stops[n]->r - stops[i]->r);
				stops[i]->dg = (stops[n]->g - stops[i]->g);
				stops[i]->db = (stops[n]->b - stops[i]->b);
				stops[i]->da = (stops[n]->a - stops[i]->a);
				i++; n++;
			}

			// Screen area
			y0 = updateArea->y;
			x0 = updateArea->x;
Serial.printf(" dx:%f dy:%f", dx,dy);

			// Calculate gradient angle
			float a = atan2f( -dy, dx );
			float m = dy/dx;
			float ms = dx/dy;
			float ilen = 1.0 / sqrtf(dx*dx + dy*dy);
Serial.printf(" a:%fdeg", a);

			// Calculate pos at origin (of updateRect)
			if (steep) {


			}
			else {
				pos0 = (x0 - _x) / cosf(a) * ilen;
Serial.printf(" x0:%f _x:%d pos0:%f", x0, _x, pos0);
				float py = (_y + (x0 - _x) * m);
				float y = py - y0;
Serial.printf(" y:%f", y);
				float d = sinf(a)*y*ilen;
				pos0 += d;
Serial.printf(" d:%f", d);
				dx = cosf(a) / dx;
				dy = -tanf(a) * dx;
Serial.printf(" dx:%f dy:%f", dx);
			}
			
		}
Serial.println();
	}

	/**
	 * Prepare to render the next line
	 * @param ry The y position in local coordinates
	 */
	void Gradient::beginLine( int16_t ry ) {
		// Calculate pos offset based on pos0 and dy
		pos = pos0 + (ry - y0) * dy;

		// Calculate the first stop on this line
		activeStop = 0;
		while (activeStop < numStops) {
			if (activeStop == numStops-1) break;
			if (stops[activeStop+1]->position >= pos){
				stops[activeStop]->start( pos );
				break;
			}
			activeStop++;
		}
	}

	/**
	 * Read a pixel from the sprite and advance position
	 * @param c (out) color
	 * @param a (out) alpha
	 */
	void Gradient::calcPixel( int16_t rx, int16_t ry ) {
		if ((rx>=(_x-1)) && (rx<=(_x+1)) && (ry==_y)) rc = 0x0099ff;
		else if ((ry>=(_y-1)) && (ry<=(_y+1)) && (rx==_x)) rc = 0x0099ff;
		else if ((rx>=(_x2-1)) && (rx<=(_x2+1)) && (ry==_y2)) rc = 0x0099ff;
		else if ((ry>=(_y2-1)) && (ry<=(_y2+1)) && (rx==_x2)) rc = 0x0099ff;
		else if ((pos>=0) && (pos<0.01)) rc = 0xff0000;
		else if ((pos>0.495) && (pos<0.505)) rc = 0xff0000;
		else if ((pos>0.99) && (pos<=1.0)) rc = 0xff0000;
		else rc = (((uint8_t)stops[activeStop]->r) << 16) | (((uint8_t)stops[activeStop]->g) << 8) | (uint8_t)stops[activeStop]->b;
		ra = stops[activeStop]->a;
		if (activeStop < (numStops-1)) {
			pos += dx;
			while ((activeStop < (numStops-1)) && (pos >= stops[activeStop+1]->position)){
				activeStop++;
			}
			stops[activeStop]->start( pos );
		}
	}

	void Gradient::skipPixel( int16_t rx, int16_t ry ){
		
	}
	
} // namespace