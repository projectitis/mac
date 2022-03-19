#pragma once
#ifndef _MAC_LINEARGRADIENTH_
#define _MAC_LINEARGRADIENTH_ 1

#include "Gradient.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project for creating beautiful and useful
 * apps on various microprocessor boards.
 *
 * mac is distributed under the MIT licence
 **/
namespace mac {

	/**
	 * A linear gradient
	 */
	class LinearGradient : public Gradient {

	public:
		/**
		 * Memory pool of recycled objects
		 */
		static LinearGradient* pool;

		/**
		 * Create a new object or take one from the pool
		 * @return The new or recycled object
		 */
		static LinearGradient* Create( uint8_t numStops );

		/**
		 * Return this object to the pool
		 */
		void recycle();

		/**
		 * @brief Construct a new Linear Gradient object
		 *
		 * @param numStops The number of stops. Must be at least 2
		 */
		LinearGradient( uint8_t numStops = 2 ) : Gradient( numStops ) {}

		/**
		 * @brief Begin the render sweep for the current frame
		 * @param updateArea The area of the display being updated
		 */
		void beginRender( ClipRect* updateArea ) override;

		/**
		 * prepare to render the next line
		 * @param ry The y position in local coordinates
		 */
		void beginLine( int16_t ry ) override;

		/**
		 * @brief Calulcate the next pixel in the line
		 * @param rx The x position in local coordinates
		 * @param ry The y position in local coordinates
		 */
		void calcPixel( int16_t rx, int16_t ry ) override;

		/**
		 * @brief Advance a pixel without calculating it
		 *
		 * @param rx The x position in local coordinates
		 * @param ry The y position in local coordinates
		 */
		void skipPixel( int16_t rx, int16_t ry ) override;

	protected:

		/**
		 * Pointer to next object in memory pool of recycled objects
		 */
		LinearGradient* _poolNext = 0;
	};

} // namespace

#endif