/** 
 * GUI library for "mac/μac"
 * Modified from TinyPngOut by Project Nayuki. This version is adapted to
 * work on Teensy microprocessor, and allows PNGs to be created directly from
 * the mac::FrameBuffer class to the SD File class. Other changes for microprocessors
 * include removal of exceptions. This version:
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 *
 * Original header as follows:
 * 
 * Tiny PNG Output (C++)
 * 
 * Copyright (c) 2018 Project Nayuki
 * https://www.nayuki.io/page/tiny-png-output
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program (see COPYING.txt and COPYING.LESSER.txt).
 * If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#ifndef _MAC_TINYPNGOUTH_
#define _MAC_TINYPNGOUTH_ 1

#include <SD.h>
#include "FrameBuffer.h"
#include "ClipRect.h"

namespace mac{

	/* 
	 * Takes image pixel data from a framebuffer and writes a PNG file to a byte output stream.
	 */
	class TinyPngOut{
	    
	    public:
		
			/**
			 * Write the framebuffer to a file as a PNG
			 * @param  out         	The open file to write to
			 * @param  framebuffer  The framebuffer
			 * @return         		True if successful         
			 */
			static boolean write( File &out, FrameBuffer* framebuffer );

			/**
			 * Write a portion of the framebuffer to a file as a PNG
			 * @param  out         	The open file to write to
			 * @param  FrameBuffer 	The framebuffer
			 * @param  area 		The area of the framebuffer to save
			 * @return         		True if successful
			 */
			static boolean write( File &out, FrameBuffer* framebuffer, ClipRect* area);
		
		private:
			static uint32_t crc;    // Primarily for IDAT chunk
			static uint32_t adler;  // For DEFLATE data within IDAT

			/**
			 * Reads the 'adler' field and updates its value based on the given array of new data.
			 * @param data Data source
			 * @param len  Length of data to process
			 */
			static void adler32(const uint8_t data[], size_t len);

			/**
			 * Reads the 'crc' field and updates its value based on the given array of new data.
			 * @param data Data source
			 * @param len  Length of data to process
			 */
			static void crc32(const uint8_t data[], size_t len);
	
			static void putBigUint32(uint32_t val, uint8_t array[4]);
			static constexpr uint16_t DEFLATE_MAX_BLOCK_SIZE = 65535;
		
	};

} //ns

#endif