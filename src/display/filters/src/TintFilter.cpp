#include "../TintFilter.h"
#include "graphics/color.h"

namespace mac{

	TintFilter::TintFilter( color888 color, float_t amount ){
		_c = color;
		_a = alphaClamp(amount);
	}

	void TintFilter::filterPixel( int16_t rx, int16_t ry, float_t& a, color888& c ){
		c = tint( c, _c, _a );
	}

} //ns:mac
