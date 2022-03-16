#pragma once
#ifndef _MAC_COLORMASKFILTER_H
#define _MAC_COLORMASKFILTER_H 1

#include "Filter.h"

namespace mac {

	/**
	 * Color-mask filter
	 * Makes a specifc color transparent
	 */
	class ColorMaskFilter : public Filter {
	public:
		/**
		 * @brief Construct a new Color Mask Filter object
		 *
		 * @param color The color to mask
		 */
		ColorMaskFilter( color888 color );

		/**
		 * @brief Apply the filter to the specified pixel
		 *
		 * @param rx The current X position in local coordinates
		 * @param ry The current Y position in local coordinates
		 * @param a (in/out) The alpha value of the pixel being filtered
		 * @param c (in/out) The color value of the pixel being filtered
		 */
		void filterPixel( int16_t rx, int16_t ry, float_t& a, color888& c ) override;

		/**
		 * @brief The color being masked
		 */
		color888 color = 0;

	};



} // ns:mac

#endif
