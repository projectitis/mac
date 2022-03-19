#pragma once
#ifndef _MAC_BORDERS_H
#define _MAC_BORDERS_H 1

#include "display/Border.h"

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
	class Borders {
	public:

		/**
		 * Constructor
		 */
		Borders();
		Borders( float_t size, color888 color, float_t alpha );
		~Borders();

		Border* top;
		Border* bottom;
		Border* left;
		Border* right;

		boolean inset = false;

	};

} // ns:mac

#endif
