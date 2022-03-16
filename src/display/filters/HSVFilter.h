#pragma once
#ifndef _MAC_HSVFILTER_H
#define _MAC_HSVFILTER_H 1

#include "Filter.h"

namespace mac {

	/**
	 * Hue Saturation Value filter
	 */
	class HSVFilter : public Filter {

	public:
		/**
		 * @brief Construct a new Hue Saturation Value Filter object
		 *
		 * @param h The hue shift (is in the range 0 to 360 after the shift)
		 * @param s The saturation shift (is in the range 0.0 to 1.0 after the shift)
		 * @param v The value shift (is in the range 0.0 to 1.0 after the shift)
		 */
		HSVFilter( float_t h = 0, float_t s = 0, float_t v = 0 );

		/**
		 * @brief Apply the filter to the specified pixel
		 *
		 * @param rx The current X position in local coordinates
		 * @param ry The current Y position in local coordinates
		 * @param a (in/out) The alpha value of the pixel being filtered
		 * @param c (in/out) The color value of the pixel being filtered
		 */
		void filterPixel( int16_t rx, int16_t ry, float_t& a, color888& c );

		float_t h, s, v;

	};



} // ns:mac

#endif
