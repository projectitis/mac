#pragma once
#ifndef _MAC_FUZZYCOLORMASKFILTER_H
#define _MAC_FUZZYCOLORMASKFILTER_H 1

#include "Filter.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project for creating beautiful and useful
 * apps on various microprocessor boards.
 *
 * mac is distributed under the MIT licence
 **/
namespace mac {

	/**
	 * Fuzzy color-mask filter
	 */
	class FuzzyColorMaskFilter : public Filter {
	public:
		/**
		 * @brief Construct a new Fuzzy Color Mask Filter object
		 *
		 * @param color The color to tint with
		 * @param threshhold The threshhold for matching thecolor (0.0 = exact, 1.0 = maximum variation)
		 */
		FuzzyColorMaskFilter( color8888 color, float_t threshhold );

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
		 * @brief Change the mask color
		 *
		 * @param color The mask color
		 */
		void color( color8888 color ) {
			_r = red( color );
			_g = green( color );
			_b = blue( color );
		}

		/**
		 * @brief Get the mask color
		 *
		 * @return color888 The mask color
		 */
		color8888 color() {
			return to8888( _r, _g, _b );
		}

		/**
		 * @brief Set the threshhold (0.0 - 1,.0)
		 *
		 * @param value The threshhold
		 */
		void threshhold( float_t value ) {
			_t = alphaClamp( value );
		}

		/**
		 * @brief Get the threshhold
		 *
		 * @return float_t
		 */
		float_t threshhold() { return _t; }

	protected:
		uint8_t _r;
		uint8_t _g;
		uint8_t _b;
		float_t _t;

		/**
		 * @brief Calculate the different between two components
		 *
		 * @param v1 The first component
		 * @param v2 The second component
		 * @return float The difference between 0.0 and 1.0
		 */
		inline float diff( uint8_t v1, uint8_t v2 ) {
			return abs( v1 - v2 ) / 255.0f;
		}
	};



} // ns:mac

#endif
