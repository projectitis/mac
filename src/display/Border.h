#pragma once
#ifndef _MAC_BORDER_H
#define _MAC_BORDER_H 1

#include "graphics/Color8888.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project for creating beautiful and useful
 * apps on various microprocessor boards.
 *
 * mac is distributed under the MIT licence
 **/
namespace mac {

	/**
	 * Border class
	 */
	class Border {

	public:

		/**
		 * Constructor
		 */
		Border();
		Border( float_t size, color888 color, float_t alpha );

		/**
		 * @brief Set size, color and alpha in one go
		 *
		 * @param size The border size
		 * @param color The border color
		 * @param alpha The alpha
		 */
		void set( float_t size, color888 color, float_t alpha );

		/**
		 * Border size
		 */
		float_t size = 0;

		/**
		 * Border color
		 */
		color888 color = 0;

		/**
		 * Alpha
		 */
		float_t alpha = 1.0;

	};



} // ns:mac

#endif
