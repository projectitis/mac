/**
 * Border class "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/

#include "../Border.h"

namespace mac{

	Border::Border(){}
	Border::Border( float_t size, color888 color, float_t alpha ){
		set( size, color, alpha );
	}

	void Border::set( float_t size, color888 color, float_t alpha ){
		if (size < 0) size = 0;
		this->size = size;
		this->color = color;
		alpha = alphaClamp( alpha );
		this->alpha = alpha;
	}

} //ns:mac
