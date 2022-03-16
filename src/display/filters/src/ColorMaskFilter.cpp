#include "../ColorMaskFilter.h"
#include "graphics/color.h"

namespace mac {

	ColorMaskFilter::ColorMaskFilter( color888 color ) {
		this->color = color & 0xffffff;
	}

	void ColorMaskFilter::filterPixel( int16_t rx, int16_t ry, float_t& a, color888& c ) {
		if ( ( c & 0xffffff ) == this->color ) a = 0;
	}

} //ns:mac
