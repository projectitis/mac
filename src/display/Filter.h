#pragma once
#ifndef _MAC_FILTER_H
#define _MAC_FILTER_H 1

#include "utils/LinkedList.h"
#include "graphics/Color8888.h"
#include "IDrawable.h"

namespace mac{
	
	/**
	 * Filter base class
	 * A filter is used to modify pixels of a display object just before rendering them.
	 * 
	 * A filter is treated as a display object. All the render methods of IDrawable are
	 * applied to filter (except calcPixel) and can be used to create complex filters.
	 * Most filters will not need to use them and will only implement filterPixel.
	 */
	class Filter: public IDrawable, LinkedList {
		
		public:
			/**
			 * @brief Destroy the Filter object
			 */
			virtual ~Filter(){};

			/**
			 * @brief Apply the filter to the specified pixel
			 * 
			 * @param rx The current X position in local coordinates
		 	 * @param ry The current Y position in local coordinates
			 * @param a (in/out) The alpha value of the pixel being filtered
			 * @param c (in/out) The color value of the pixel being filtered
			 */
			virtual void filterPixel( int16_t rx, int16_t ry, float_t& a, color888& c ){}

			/**
			 * Return the next item
			 */
			Filter* next();

	};



} // ns:mac

#endif
