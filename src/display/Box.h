#pragma once
#ifndef _MAC_RECTANGLEH_
#define _MAC_RECTANGLEH_ 1

#include "display/DisplayObject.h"
#include "display/Gradient.h"
#include "display/LinearGradient.h"
#include "display/Borders.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project for creating beautiful and useful
 * apps on various microprocessor boards.
 *
 * mac is distributed under the MIT licence
 **/
namespace mac {

	/**
	 * A rectangle
	 */
	class Box : public DisplayObject, public MemoryPool<Box> {
	public:

		/**
		 * Create a new object or take one from the pool
		 * @return The new or recycled object
		 */
		static Box* Create( int16_t x, int16_t y, int16_t w, int16_t h );
		static Box* Create( ClipRect* rect );

		/**
		 * @brief Construct a new Box object
		 */
		Box();

		/**
		 * @brief Destroy the Box object
		 */
		~Box();

		/**
		 * Reset the object back to default settings
		 */
		void reset();

		/**
		 * @brief Provide the points that define the shape
		 * If the first and last points are different, the are automatically joined
		 *
		 * @param points An array of points
		 * @param len The number of points
		 */
		void set( int16_t x, int16_t y, int16_t w, int16_t h );

		/**
		 * Color. Note that if a gradient is set, it will be used instead
		 */
		color888 color;

		/**
		 * Gradient. If set, will be used instead of color
		 */
		Gradient* gradient;

		/**
		 * @brief Borders of the box
		 */
		Borders* borders;

		/**
		 * @brief Begin the render sweep for the current frame
		 * @param updateArea The area of the display being updated
		 */
		void beginRender( ClipRect* updateArea );

		/**
		 * prepare to render the next line
		 * @param ry The y position in local coordinates
		 */
		void beginLine( int16_t ry );

		/**
		 * Read a pixel
		 * @param rx The x position in local coordinates
		 * @param ry The y position in local coordinates
		 */
		void calcPixel( int16_t rx, int16_t ry );

		/**
		 * Skip a pixel
		 * @param rx The x position in local coordinates
		 * @param ry The y position in local coordinates
		 */
		void skipPixel( int16_t rx, int16_t ry );

	};

} // namespace

#endif