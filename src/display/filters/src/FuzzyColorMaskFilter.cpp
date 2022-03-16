#include "../FuzzyColorMaskFilter.h"
#include "graphics/color.h"

#define INV_255 1.0/255.0;

namespace mac {

	FuzzyColorMaskFilter::FuzzyColorMaskFilter( color888 color, float_t threshhold ) {
		_r = red( color );
		_g = green( color );
		_b = blue( color );
		_t = alphaClamp( threshhold );
	}

	void FuzzyColorMaskFilter::filterPixel( int16_t rx, int16_t ry, float_t& a, color888& c ) {
		if ( ( diff( red( c ), _r ) <= _t ) && ( diff( green( c ), _g ) <= _t ) && ( diff( blue( c ), _b ) <= _t ) ) a = 0;
	}

} //ns:mac
