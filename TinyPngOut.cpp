/* 
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

#include "TinyPngOut.h"
#include "Bitmap.h"

#define IS_BIG_ENDIAN (*(uint16_t *)"\0\xff" < 0x100)

namespace mac{

	uint32_t TinyPngOut::crc = 0;
	uint32_t TinyPngOut::adler = 1;

	/**
	 * Write entire framebuffer as PNG file
	 * @param framebuffer  	The source framebuffer
	 */
	boolean TinyPngOut::write( File &out, FrameBuffer* framebuffer ){
		ClipRect* area = new ClipRect();
		area->setPosAndSize(  0,0, framebuffer->width,framebuffer->height );
		boolean result = write( out, framebuffer, area );
		delete area;
		return result;
	}

	/**
	 * Write a section of the framebuffer as a PNG file
	 */
	boolean TinyPngOut::write( File &out, FrameBuffer* framebuffer, ClipRect* area ){
		color888 c;
		uint8_t R, G, B;
		int16_t x,y;
		uint32_t uncompRemain = 0;
		uint16_t deflateFilled = 0;
		uint32_t lineSize = 0;
		uint32_t uBytes = 0;

		crc = 0;
		adler = 1;

		// Validate area
		area->clipPosAndSize( 0,0, framebuffer->width,framebuffer->height );
		if ((area->width<1) || (area->height<1)) return false;

		// Check sizes
		uint64_t lineSz = (uint64_t)(area->width) * 3 + 1;
		if (lineSz > UINT32_MAX) return false;
		lineSize = (uint32_t)lineSz;

		uint64_t uncompRm = lineSize * area->height;
		if (uncompRm > UINT32_MAX) return false;
		uncompRemain = (uint32_t)uncompRm;

		uint32_t numBlocks = uncompRemain / DEFLATE_MAX_BLOCK_SIZE;
		if (uncompRemain % DEFLATE_MAX_BLOCK_SIZE != 0) numBlocks++;  // Round up

		// 5 bytes per DEFLATE uncompressed block header, 2 bytes for zlib header, 4 bytes for zlib Adler-32 footer
		uint64_t idatSize = (uint64_t)(numBlocks * 5 + 6);
		idatSize += uncompRemain;
		if (idatSize > (uint32_t)INT32_MAX) return false;

		// Write header (not a pure header, but a couple of things concatenated together)
		uint8_t header[] = {  // 43 bytes long
			// PNG header
			0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A,
			// IHDR chunk
			0x00, 0x00, 0x00, 0x0D,
			0x49, 0x48, 0x44, 0x52,
			0, 0, 0, 0,  // 'width' placeholder
			0, 0, 0, 0,  // 'height' placeholder
			0x08, 0x02, 0x00, 0x00, 0x00,
			0, 0, 0, 0,  // IHDR CRC-32 placeholder
			// IDAT chunk
			0, 0, 0, 0,  // 'idatSize' placeholder
			0x49, 0x44, 0x41, 0x54,
			// DEFLATE data
			0x08, 0x1D,
		};
		putBigUint32( (uint32_t)area->width, &header[16]);
		putBigUint32( (uint32_t)area->height, &header[20]);
		putBigUint32( (uint32_t)idatSize, &header[33]);
		crc = 0;
		crc32(&header[12], 17);
		putBigUint32(crc, &header[29]);
		out.write( (uint8_t*)&header, sizeof(header) );
		
		crc = 0;
		crc32(&header[37], 6);  // 0xD7245B6B

		// tBytes = total pixel bytes
		// per line:
		// 		uBytes = used bytes
		// 		nBytes = number of bytes
		uint32_t tBytes = area->width * area->height * 3;
		x = area->x;
		y = area->y;
		uBytes = 0;
		uint8_t buffer[12];
		int8_t bufferPos = -1;
Serial.printf("Expecting %d bytes\n",tBytes);
		while (tBytes > 0) {

			// Start DEFLATE block
			if (deflateFilled == 0) {
				uint16_t size = DEFLATE_MAX_BLOCK_SIZE;
				if (uncompRemain < size) size = (uint16_t)uncompRemain;
				const uint8_t header[] = {  // 5 bytes long
					(uint8_t)(uncompRemain <= DEFLATE_MAX_BLOCK_SIZE ? 1 : 0),
					(uint8_t)(size >> 0),
					(uint8_t)(size >> 8),
					(uint8_t)((size >> 0) ^ 0xFF),
					(uint8_t)((size >> 8) ^ 0xFF),
				};
				out.write( (uint8_t*)&header, sizeof(header) );
				crc32( header, sizeof(header) / sizeof(header[0]) );
			}
			// Sanity check
			if (!(x <= area->x2 && deflateFilled < DEFLATE_MAX_BLOCK_SIZE)) return false;
			
			// Beginning of line - write filter method byte
			if (uBytes == 0) {				
				uint8_t b[] = {0};
				out.write( (uint8_t*)&b, sizeof(b) );
				crc32(b, 1);
				adler32(b, 1);
				uncompRemain--;
				deflateFilled++;
				uBytes++;
			}

			// Write some pixel bytes for current line
			else {
				// Allowed up to size of block
				uint16_t nBytes = (DEFLATE_MAX_BLOCK_SIZE - deflateFilled)/3;
				nBytes = min( nBytes, lineSize - uBytes);
				nBytes = min( nBytes, tBytes );		
				if (nBytes <= 0) return false;
Serial.printf("  About to fetch %d bytes\n",nBytes);

				// Increment positions
				tBytes -= nBytes;
				uncompRemain -= nBytes;
				deflateFilled += nBytes;
				uBytes += nBytes;

				// Step through the required number of bytes
				while (nBytes){

					// Grab pixels if buffer is empty
					if (bufferPos<0){
						uint8_t numBytes = min( 12, nBytes );
						bufferPos = numBytes-1;
						while (numBytes){
							c = framebuffer->pixel( x,y );
							convert888toRGB( c, R, G, B );
							buffer[--numBytes] = R;
							buffer[--numBytes] = G;
							buffer[--numBytes] = B;

							x++;
							if (x > area->x2){
								x = 0;
								y++;
Serial.printf("    At line %d\n",y);
							}
						}
					}

					// Read bytes from buffer
					uint8_t b[] = {buffer[bufferPos--]};
					out.write( (uint8_t*)&b, 1 );
					crc32(b, 1);
					adler32(b, 1);

					nBytes--;
				}
			}
			
			// End current block
			if (deflateFilled >= DEFLATE_MAX_BLOCK_SIZE) deflateFilled = 0; 

			// End current line
			if (uBytes == lineSize) uBytes = 0;

		} // while count

		uint8_t footer[] = {  // 20 bytes long
			0, 0, 0, 0,  // DEFLATE Adler-32 placeholder
			0, 0, 0, 0,  // IDAT CRC-32 placeholder
			// IEND chunk
			0x00, 0x00, 0x00, 0x00,
			0x49, 0x45, 0x4E, 0x44,
			0xAE, 0x42, 0x60, 0x82,
		};
		putBigUint32(adler, &footer[0]);
		crc32(&footer[0], 4);
		putBigUint32(crc, &footer[4]);
		out.write( (uint8_t*)&footer, sizeof(footer) );

		return true;
	}

	void TinyPngOut::crc32(const uint8_t data[], size_t len) {
		crc = ~crc;
		for (size_t i = 0; i < len; i++) {
			for (int j = 0; j < 8; j++) {  // Inefficient bitwise implementation, instead of table-based
				uint32_t bit = (crc ^ (data[i] >> j)) & 1;
				crc = (crc >> 1) ^ ((-bit) & UINT32_C(0xEDB88320));
			}
		}
		crc = ~crc;
	}


	void TinyPngOut::adler32(const uint8_t data[], size_t len) {
		uint32_t s1 = adler & 0xFFFF;
		uint32_t s2 = adler >> 16;
		for (size_t i = 0; i < len; i++) {
			s1 = (s1 + data[i]) % 65521;
			s2 = (s2 + s1) % 65521;
		}
		adler = s2 << 16 | s1;
	}


	void TinyPngOut::putBigUint32(uint32_t val, uint8_t array[4]) {
		array[0] = (uint8_t)(val >> 24);
		array[1] = (uint8_t)(val >> 16);
		array[2] = (uint8_t)(val >> 8);
		array[3] = (uint8_t)(val);
	}

} //ns