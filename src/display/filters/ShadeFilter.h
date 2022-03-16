#pragma once
#ifndef _MAC_SHADEFILTER_H
#define _MAC_SHADEFILTER_H 1

#include "Filter.h"

namespace mac {

	/**
	 * Shade (lighten and darken) filter
	 */
	class ShadeFilter : public Filter {
	public:
		/**
		 * @brief Construct a new Shade Filter object
		 *
		 * @param shade The amount to shade by (-1.0 is fully dark, 1.0 is fully light)
		 */
		ShadeFilter( float_t amount );

		/**
		 * @brief Apply the filter to the specified pixel
		 *
		 * @param rx The current X position in local coordinates
		 * @param ry The current Y position in local coordinates
		 * @param a (in/out) The alpha value of the pixel being filtered
		 * @param c (in/out) The color value of the pixel being filtered
		 */
		void filterPixel( int16_t rx, int16_t ry, float_t& a, color888& c );

		/**
		 * @brief Set the amount of tint (0.0 - 1,.0)
		 *
		 * @param value The amount
		 */
		void amount( float_t value ) {
			_a = (value < -1.0)? -1.0 : (value > 1.0)? 1.0 : value;
		}

		/**
		 * @brief Get the amount
		 *
		 * @return float_t
		 */
		float_t amount() { return _a; }

	protected:

		float_t _a;

	};



} // ns:mac

#endif
