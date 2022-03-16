#include "../HSVFilter.h"
#include "graphics/color8888.h"

namespace mac {

	HSVFilter::HSVFilter( float_t h, float_t s, float_t v ) {
		this->h = h;
		this->s = s;
		this->v = v;
	}

	void HSVFilter::filterPixel( int16_t rx, int16_t ry, float_t& a, color888& c ) {
		float_t h, s, v;
		toHSV( c, h, s, v );
		h += this->h;
		s += this->s;
		v += this->v;
		c = to8888( h, s, v );
	}

} //ns:mac
