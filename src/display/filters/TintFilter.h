#pragma once
#ifndef _MAC_TINTFILTER_H
#define _MAC_TINTFILTER_H 1

#include "Filter.h"

namespace mac{
	
	/**
	 * Tint filter
	 */
	class TintFilter: public Filter {
		
		public:
			/**
			 * @brief Construct a new Tint Filter object
			 * 
			 * @param color The color to tint with
			 * @param amount The amount to tint by
			 */
			TintFilter( color888 color, float_t amount );

			/**
			 * @brief Apply the filter to the specified pixel
			 * 
			 * @param rx The current X position in local coordinates
		 	 * @param ry The current Y position in local coordinates
			 * @param a (in/out) The alpha value of the pixel being filtered
			 * @param c (in/out) The color value of the pixel being filtered
			 */
			void filterPixel( int16_t rx, int16_t ry, float_t& a, color888& c );

		protected:
			color888 _c;
			float_t _a;

	};



} // ns:mac

#endif
