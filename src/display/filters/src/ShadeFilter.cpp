#include "../ShadeFilter.h"
#include "graphics/color.h"

namespace mac {

	ShadeFilter::ShadeFilter( float_t amount ) {
		_a = (amount < -1.0)? -1.0 : (amount > 1.0)? 1.0 : amount;
	}

	void ShadeFilter::filterPixel( int16_t rx, int16_t ry, float_t& a, color888& c ) {
		if (_a < 0) c = darken(c, -_a);
		else c = lighten(c, _a);
	}

} //ns:mac
