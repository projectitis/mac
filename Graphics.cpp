 /**
 * Display library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/
 
#include "Graphics.h"
#include "GraphicsBitmapExtn16.h"
#include "GraphicsVectorExtn16.h"
#include "GraphicsVectorExtn32.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{
	
	/**
	 * Constructor. Must pass in a display adapter for the hardware display being used.
	 * @param	display			A Display adapter instance for the hardware display being used
	 * @param	extensionFlags 	The extensions to load (@see mac::GraphicsExtensions enum type)
	 **/
	Graphics::Graphics( Display* display ){
		_init( display, 255 ); // All extensions
	}

	/**
	 * Constructor. Must pass in a display adapter for the hardware display being used.
	 * @param	display			A Display adapter instance for the hardware display being used
	 * @param	extensionFlags 	The extensions to load (@see mac::GraphicsExtensions enum type)
	 **/
	Graphics::Graphics( Display* display, int extensionFlags ){
		_init( display, extensionFlags );
	}

	/**
	 * Initialise the graphics object. Called by the constructor.
	 */
	void Graphics::_init( Display* display, int extensionFlags ){
		// Display
		this->display = display;
		this->framebuffer = this->display->framebuffer;
		
		// Extensions
		if (extensionFlags & extension_bitmap){
			/*
			if (framebuffer->pixelFormat == PF_RGB565){
				bitmap = new GraphicsBitmapExtn16();
			}
			else if (framebuffer->pixelFormat == PF_RGB888){
				bitmap = new GraphicsBitmapExtn32();
			}
			*/
			bitmap = new GraphicsBitmapExtn16();
			if (bitmap) bitmap->init( this->display->framebuffer );
		}
		if (extensionFlags & extension_vector){
			if (framebuffer->pixelFormat == PF_565){
				vector = new GraphicsVectorExtn16();
			}
			else if (framebuffer->pixelFormat == PF_888){
				vector = new GraphicsVectorExtn32();
			}
			if (vector) vector->init( this->display->framebuffer );
		}
		if (extensionFlags & extension_text){
			text = new GraphicsTextExtn();
			text->init( this->display->framebuffer );
		}
		/*
		if (extensionFlags & extension_text){
			if (framebuffer->pixelFormat == PF_RGB565){
				text = new GraphicsTextExtn16();
			}
			else if (framebuffer->pixelFormat == PF_RGB888){
				text = new GraphicsTextExtn32();
			}
			text->init( this->display->framebuffer );
		}
		*/
	}
	
	/**
	 * Destructor
	 **/
	Graphics::~Graphics(){
		if (bitmap) delete bitmap;
		if (vector) delete vector;
		if (text) delete text;
	}
	
	/**
	 * Update the framebuffer to the display
	 **/
	void Graphics::update(){
		this->display->update( false );
	}
	
	/**
	 * Fill the framebuffer with a color
	 **/
	void Graphics::clear( color888 color ){
		this->display->framebuffer->clear( color );
	}

	/**
	 * Blend a pixel to the display
	 * @param x     Display X coordinate
	 * @param y     Display Y coordinate
	 * @param color The color
	 * @param alpha The alpha (0.0 - 1.0)
	 */
	void Graphics::pixel(
		int16_t x,
		int16_t y,
		color888 color,
		alpha alpha
	){
		if (alpha>=1.0f) framebuffer->pixel( color, x, y );
		else framebuffer->blend( color, alpha, x, y );
	}

	/**
	 * get a pixel from the display
	 * @param x     Display X coordinate
	 * @param y     Display Y coordinate
	 * @return		The pixel color
	 */
	color888 Graphics::pixel(
		int16_t x,
		int16_t y
	){
		return framebuffer->pixel( x, y );
	}

	/**
	 * Draw a horizontal line.
	 * @param x     	Start x coord
	 * @param y     	Start y coord
	 * @param length 	Length of line
	 * @param color 	Color of line (RGB888)
	 * @param alpha 	Alpha of the line (0 to 1)
	 */
	void Graphics::lineH(
		int16_t x,
		int16_t y,
		int16_t length,
		color888 color,
		alpha alpha
	){
		// Bounds checks
		if (length<0){
			x += (length-1);
			length = -length;
		}
		if ((x>=framebuffer->width) || (y>=framebuffer->height) || (y<0)) return;

		// Clipping
		if (x<0){
			length += x; if (length<=0) return;
			x = 0;
		}
		if ((x+length) > framebuffer->width){
			length = framebuffer->width - x;
		}

		// Draw actual line
		if (framebuffer->pixelFormat == PF_565) _line16(x,y,1,length,color,alpha);
		else if (framebuffer->pixelFormat == PF_888) _line32(x,y,1,length,color,alpha);
	}

	/**
	 * Draw a horizontal line with gradient
	 * @param x     	Start x coord
	 * @param y     	Start y coord
	 * @param length 	Length of line
	 * @param color0 	Left color (RGB888)
	 * @param alpha0 	Left alpha (0 to 255)
	 * @param color1 	Right color (RGB888)
	 * @param alpha1 	Right alpha (0 to 255)
	 */
	void Graphics::lineHG(
		int16_t x,
		int16_t y,
		int16_t length,
		color888 color0,
		alpha alpha0,
		color888 color1,
		alpha alpha1
	){
		// Bounds checking
		if (length<0){
			x += (length-1);
			length = -length;
			swap(color0,color1);
			swap(alpha0,alpha1);
		}
		if ((x>=framebuffer->width) || (y>=framebuffer->height) || (y<0)) return;

		// Calculate gradient components
		float r,g,b,a;
		float gR, gG, gB, gA;
		r = color0 >> 16;
		g = (color0 >> 8) & 0xFF;
		b = color0 & 0xFF;
		alphaClamp( alpha0 );
		alphaClamp( alpha1 );
		a = alpha0;
		gR = ((color1 >> 16) - r) / length;
		gG = (((color1 >> 8) & 0xFF) - g) / length;
		gB = ((color1 & 0xFF) - b) / length;
		gA = (alpha1 - alpha0) / length;

		// Clipping
		if (x<0){
			length += x; if (length<=0) return;
			r -= x*gR; g -= x*gG; b -= x*gB; a -= x*gA;
			x = 0;
		}
		if ((x+length) > framebuffer->width){
			length = framebuffer->width - x;
		}

		// Address
		uint32_t i = framebuffer->index(x,y);

		// 16 bit
		if (framebuffer->pixelFormat == PF_565){

			// Solid line
			if ((alpha0>=1) && (alpha1 >= 1)){
				// Set solid pixels
				while (length>0){
					framebuffer->data.data16[i] = convertRGBto565( r, g, b );
					i++;
					length--;
					r += gR;
					g += gG;
					b += gB;
				}
			}
			// With alpha blending
			else{
				// Blend pixels
				while (length>0){
					framebuffer->data.data16[i] = alphaBlend5565(
						convertRGBto565( r, g, b ),
						framebuffer->data.data16[i],
						(uint8_t)( a * 16 )
					);
					i++;
					length--;
					r += gR;
					g += gG;
					b += gB;
					a += gA;
				}
			}

		} // 16-bit

		// 32-bit
		else if (framebuffer->pixelFormat == PF_888){

			// Solid line
			if ((alpha0>=1) && (alpha1 >= 1)){
				// Set solid pixels
				while (length>0){
					framebuffer->data.data32[i] = convertRGBto888( r, g, b );
					i++;
					length--;
					r += gR;
					g += gG;
					b += gB;
				}
			}
			// With alpha blending
			else{
				// Blend pixels
				while (length>0){
					framebuffer->data.data32[i] = alphaBlend8888(
						convertRGBto888( r, g, b ),
						framebuffer->data.data32[i],
						(uint8_t)( a * 255 )
					);
					i++;
					length--;
					r += gR;
					g += gG;
					b += gB;
					a += gA;
				}
			}

		} // 32-bit
	}

	/**
	 * Draw a vertical line.
	 * @param x     	Start x coord
	 * @param y     	Start y coord
	 * @param length 	Length of line
	 * @param color 	Color of line (RGB888)
	 * @param alpha 	Alpha of the line (0 to 255)
	 */
	void Graphics::lineV(
		int16_t x,
		int16_t y,
		int16_t length,
		color888 color,
		alpha alpha
	){
		// Bounds checks
		if (length<0){
			y += (length-1);
			length = -length;
		}
		if ((x>=framebuffer->width) || (y>=framebuffer->height) || (x<0)) return;

		// Clipping
		if (y<0){
			length += y; if (length<=0) return;
			y = 0;
		}
		if ((y+length) > framebuffer->height){
			length = framebuffer->height - y;
		}

		// Draw actual line
		if (framebuffer->pixelFormat == PF_565) _line16(x,y,framebuffer->width,length,color,alpha);
		else if (framebuffer->pixelFormat == PF_888) _line32(x,y,framebuffer->width,length,color,alpha);
	}

	/**
	 * Draw a vertical line with gradient
	 * @param x     	Start x coord
	 * @param y     	Start y coord
	 * @param length 	Length of line
	 * @param color0 	Top color (RGB888)
	 * @param alpha0 	Top alpha (0 to 255)
	 * @param color1 	Bottom color (RGB888)
	 * @param alpha1 	Bottom alpha (0 to 255)
	 */
	void Graphics::lineVG(
		int16_t x,
		int16_t y,
		int16_t length,
		color888 color0,
		alpha alpha0,
		color888 color1,
		alpha alpha1
	){
		// Bounds checking
		if (length<0){
			y += (length-1);
			length = -length;
			swap(color0,color1);
			swap(alpha0,alpha1);
		}
		if ((x>=framebuffer->width) || (y>=framebuffer->height) || (x<0)) return;

		// Calculate gradient components
		float r,g,b,a;
		float gR, gG, gB, gA;
		r = color0 >> 16;
		g = (color0 >> 8) & 0xFF;
		b = color0 & 0xFF;
		alphaClamp( alpha0 );
		alphaClamp( alpha1 );
		a = alpha0;
		gR = ((color1 >> 16) - r) / length;
		gG = (((color1 >> 8) & 0xFF) - g) / length;
		gB = ((color1 & 0xFF) - b) / length;
		gA = (alpha1 - alpha0) / length;

		// Clipping
		if (y<0){
			length += y; if (length<=0) return;
			r -= y*gR; g -= y*gG; b -= y*gB; a -= y*gA;
			y = 0;
		}
		if ((y+length) > framebuffer->height){
			length = framebuffer->height - y;
		}

		// Address
		uint32_t i = framebuffer->index(x,y);

		// 16 bit
		if (framebuffer->pixelFormat == PF_565){

			// Solid line
			if ((alpha0>=1) && (alpha1 >= 1)){
				// Set solid pixels
				while (length>0){
					framebuffer->data.data16[i] = convertRGBto565( r, g, b );
					i+=framebuffer->width;
					length--;
					r += gR;
					g += gG;
					b += gB;
				}
			}
			// With alpha blending
			else{
				// Blend pixels
				while (length>0){
					framebuffer->data.data16[i] = alphaBlend5565(
						convertRGBto565( r, g, b ),
						framebuffer->data.data16[i],
						(uint8_t)( a * 16 )
					);
					i+=framebuffer->width;
					length--;
					r += gR;
					g += gG;
					b += gB;
					a += gA;
				}
			}

		} // 16-bit

		// 32-bit
		else if (framebuffer->pixelFormat == PF_888){

			// Solid line
			if ((alpha0>=1) && (alpha1 >= 1)){
				// Set solid pixels
				while (length>0){
					framebuffer->data.data32[i] = convertRGBto888( r, g, b );
					i+=framebuffer->width;
					length--;
					r += gR;
					g += gG;
					b += gB;
				}
			}
			// With alpha blending
			else{
				// Blend pixels
				while (length>0){
					framebuffer->data.data32[i] = alphaBlend8888(
						convertRGBto888( r, g, b ),
						framebuffer->data.data32[i],
						(uint8_t)( a * 255 )
					);
					i+=framebuffer->width;
					length--;
					r += gR;
					g += gG;
					b += gB;
					a += gA;
				}
			}

		} // 32-bit
	}

	void Graphics::_line16(
		uint16_t x,
		uint16_t y,
		uint16_t step,
		uint16_t length,
		color888 color,
		alpha alpha
	){
		// Address
		uint32_t i = framebuffer->index(x,y);

		// Solid line
		if (alpha>=1){
			// Set solid pixels
			uint16_t fg = convert888to565( color );
			while (length>0){
				framebuffer->data.data16[i] = fg;
				i += step;
				length--;
			}
		}
		// With alpha blending
		else{
			// Precalculate blending values for faster blending
			uint8_t a = alpha5bit(alpha);
			uint32_t fg = colorPrepare565( color );
			while (length>0){
				framebuffer->data.data16[i] = alphaBlendPrepared5565( fg, framebuffer->data.data16[i], a );
				i += step;
				length--;
			}
		}

	}

	void Graphics::_line32(
		uint16_t x,
		uint16_t y,
		uint16_t step,
		uint16_t length,
		color888 color,
		alpha alpha
	){
		// Address
		uint32_t i = framebuffer->index(x,y);

		// Solid line
		if (alpha>=1){
			// Set solid pixels
			while (length>0){
				framebuffer->data.data32[i] = color;
				i += step;
				length--;
			}
		}
		// With alpha blending
		else{
			// Precalculate blending values for faster blending
			uint8_t a = alpha8bit(alpha);
			uint32_t rb,g;
			colorPrepare888( color, rb, g );
			while (length>0){
				framebuffer->data.data32[i] = alphaBlendPrepared8888( rb, g, framebuffer->data.data32[i], a );
				i += step;
				length--;
			}
		}

	}

	/**
	 * Draw a basic rectangle with alpha. If you require sub-pixel accuracy,
	 * use the vector extension (graphics->vector->rectangle).
	 * @param x     	Start x coord
	 * @param y     	Start y coord
	 * @param width  	The width of the rectangle
	 * @param height 	The height of the rectangle
	 * @param color 	Color of rectangle (RGB888)
	 * @param alpha 	Alpha of the rectangle (0 to 255)
	 */
	void Graphics::rectangle(
		int16_t x,
		int16_t y,
		int16_t width,
		int16_t height,
		color888 color,
		alpha alpha
	){
		// Bounds checking
		if (width<0){
			x += (width-1);
			width = -width;
		}
		if (height<0){
			y += (height-1);
			height = -height;
		}
		if ((x>=framebuffer->width) || (y>=framebuffer->height)) return;

		// Clipping
		if (x<0){
			width += x; if (width<=0) return;
			x = 0;
		}
		if ((x+width) > framebuffer->width){
			width = framebuffer->width - x;
		}
		if (y<0){
			height += y; if (height<=0) return;
			y = 0;
		}
		if ((y+height) > framebuffer->height){
			height = framebuffer->height - y;
		}

		// Address
		uint32_t i = framebuffer->index(x,y);
		uint32_t ii = i;
		int16_t w;

		// 16-bit
		if (framebuffer->pixelFormat == PF_565){

			// Solid rect
			if (alpha>=1){
				// Set solid pixels
				uint16_t fg = convert888to565( color );
				while (height>0){
					w = width;
					while (w>0){
						framebuffer->data.data16[i] = fg;
						i++;
						w--;
					}
					ii += framebuffer->width;
					i = ii;
					height--;
				}
			}
			// With alpha blending
			else{
				// Precalculate blending values for faster blending
				uint8_t a = alpha5bit(alpha);
				uint32_t fg = colorPrepare565( color );
				while (height>0){
					w = width;
					while (w>0){
						framebuffer->data.data16[i] = alphaBlendPrepared5565( fg, framebuffer->data.data16[i], a );
						i++;
						w--;
					}
					ii += framebuffer->width;
					i = ii;
					height--;
				}
			}

		} // 16 bit

		// 32-bit
		else if (framebuffer->pixelFormat == PF_888){

			// Solid rect
			if (alpha>=1){
				// Set solid pixels
				while (height>0){
					w = width;
					while (w>0){
						framebuffer->data.data32[i] = color;
						i++;
						w--;
					}
					ii += framebuffer->width;
					i = ii;
					height--;
				}
			}
			// With alpha blending
			else{
				// Precalculate blending values for faster blending
				uint8_t a = alpha8bit(alpha);
				uint32_t rb,g;
				colorPrepare888( color, rb, g );
				while (height>0){
					w = width;
					while (w>0){
						framebuffer->data.data32[i] = alphaBlendPrepared8888( rb, g, framebuffer->data.data32[i], a );
						i++;
						w--;
					}
					ii += framebuffer->width;
					i = ii;
					height--;
				}
			}

		}
	}

	/**
	 * Draw a basic rectangle with horizontal gradient.
	 * @param x     	Start x coord
	 * @param y     	Start y coord
	 * @param width  	The width of the rectangle
	 * @param height 	The height of the rectangle
	 * @param color0 	Left color of rectangle (RGB888)
	 * @param alpha0 	Left alpha of the rectangle (0 to 255)
	 * @param color1 	Right color of rectangle (RGB888)
	 * @param alpha1 	Right alpha of the rectangle (0 to 255)
	 **/
	void Graphics::rectangleHG(
		int16_t x,
		int16_t y,
		int16_t width,
		int16_t height,
		color888 color0,
		alpha alpha0,
		color888 color1,
		alpha alpha1
	){
		// Bounds checking
		if (width<0){
			x += (width-1);
			width = -width;
			swap(color0,color1);
			swap(alpha0,alpha1);
		}
		if (height<0){
			y += (height-1);
			height = -height;
		}
		if ((x>=framebuffer->width) || (y>=framebuffer->height)) return;

		// Calculate gradient components
		float r,g,b,a;
		float gR, gG, gB, gA;
		r = color0 >> 16;
		g = (color0 >> 8) & 0xFF;
		b = color0 & 0xFF;
		alphaClamp( alpha0 );
		alphaClamp( alpha1 );
		a = alpha0;
		gR = ((color1 >> 16) - r) / width;
		gG = (((color1 >> 8) & 0xFF) - g) / width;
		gB = ((color1 & 0xFF) - b) / width;
		gA = (alpha1 - alpha0) / width;

		// Clipping
		if (x<0){
			width += x; if (width<=0) return;
			r -= x*gR; g -= x*gG; b -= x*gB; a -= x*gA;
			x = 0;
		}
		if ((x+width) > framebuffer->width){
			width = framebuffer->width - x;
		}
		if (y<0){
			height += y; if (height<=0) return;
			y = 0;
		}
		if ((y+height) > framebuffer->height){
			height = framebuffer->height - y;
		}

		// 16-bit
		if (framebuffer->pixelFormat == PF_565){

			// Solid rect
			if ((alpha0>=1) && (alpha1>=1)){
				// Set solid pixels
				// We move across the screen and draw lines downward
				while (width>0){
					_line16(
						x,y,
						framebuffer->width,
						height,
						convertRGBto888((uint8_t)r,(uint8_t)g,(uint8_t)b)
					);
					x++;
					width--;
					r += gR;
					g += gG;
					b += gB;
				}
			}
			// With alpha blending
			else{
				// Precalculate blending values for faster blending
				while (width>0){
					_line16(
						x,y,
						framebuffer->width,
						height,
						convertRGBto888((uint8_t)r,(uint8_t)g,(uint8_t)b),
						(uint8_t)( a * 31 )
					);
					x++;
					width--;
					r += gR;
					g += gG;
					b += gB;
					a += gA;
				}
			}

		} // 16 bit

		// 32-bit
		else if (framebuffer->pixelFormat == PF_888){

			// Solid rect
			if ((alpha0>=1) && (alpha1>=1)){
				while (width>0){
					_line32(
						x,y,
						framebuffer->width,
						height,
						convertRGBto888((uint8_t)r,(uint8_t)g,(uint8_t)b)
					);
					x++;
					width--;
					r += gR;
					g += gG;
					b += gB;
				}
			}
			// With alpha blending
			else{
				while (width>0){
					_line32(
						x,y,
						framebuffer->width,
						height,
						convertRGBto888((uint8_t)r,(uint8_t)g,(uint8_t)b),
						(uint8_t)( a * 255 )
					);
					x++;
					width--;
					r += gR;
					g += gG;
					b += gB;
					a += gA;
				}
			}

		} // 32 bit
	}

	/**
	 * Draw a basic rectangle with vertical gradient.
	 * @param x     	Start x coord
	 * @param y     	Start y coord
	 * @param width  	The width of the rectangle
	 * @param height 	The height of the rectangle
	 * @param color0 	Top color of rectangle (RGB888)
	 * @param alpha0 	Top alpha of the rectangle (0 to 255)
	 * @param color1 	Bottom color of rectangle (RGB888)
	 * @param alpha1 	Bottom alpha of the rectangle (0 to 255)
	 **/
	void Graphics::rectangleVG(
		int16_t x,
		int16_t y,
		int16_t width,
		int16_t height,
		color888 color0,
		alpha alpha0,
		color888 color1,
		alpha alpha1
	){
		// Bounds checking
		if (width<0){
			x += (width-1);
			width = -width;
		}
		if (height<0){
			y += (height-1);
			height = -height;
			swap(color0,color1);
			swap(alpha0,alpha1);
		}
		if ((x>=framebuffer->width) || (y>=framebuffer->height)) return;

		// Calculate gradient components
		float r,g,b,a;
		float gR, gG, gB, gA;
		r = color0 >> 16;
		g = (color0 >> 8) & 0xFF;
		b = color0 & 0xFF;
		alphaClamp( alpha0 );
		alphaClamp( alpha1 );
		a = alpha0;
		gR = ((color1 >> 16) - r) / height;
		gG = (((color1 >> 8) & 0xFF) - g) / height;
		gB = ((color1 & 0xFF) - b) / height;
		gA = (alpha1 - alpha0) / height;

		// Clipping
		if (x<0){
			width += x; if (width<=0) return;
			x = 0;
		}
		if ((x+width) > framebuffer->width){
			width = framebuffer->width - x;
		}
		if (y<0){
			height += y; if (height<=0) return;
			r -= y*gR; g -= y*gG; b -= y*gB; a -= y*gA;
			y = 0;
		}
		if ((y+height) > framebuffer->height){
			height = framebuffer->height - y;
		}

		// 16-bit
		if (framebuffer->pixelFormat == PF_565){

			// Solid rect
			if ((alpha0>=1) && (alpha1>=1)){
				while (height>0){
					_line16(
						x,y,
						1,
						width,
						convertRGBto888((uint8_t)r,(uint8_t)g,(uint8_t)b)
					);
					y++;
					height--;
					r += gR;
					g += gG;
					b += gB;
				}
			}
			// With alpha blending
			else{
				while (height>0){
					_line16(
						x,y,
						1,
						width,
						convertRGBto888((uint8_t)r,(uint8_t)g,(uint8_t)b),
						(uint8_t)( a * 31 )
					);
					y++;
					height--;
					r += gR;
					g += gG;
					b += gB;
					a += gA;
				}
			}

		} // 16 bit

		// 32-bit
		else if (framebuffer->pixelFormat == PF_888){

			// Solid rect
			if ((alpha0>=1) && (alpha1>=1)){
				while (height>0){
					_line32(
						x,y,
						1,
						width,
						convertRGBto888((uint8_t)r,(uint8_t)g,(uint8_t)b)
					);
					y++;
					height--;
					r += gR;
					g += gG;
					b += gB;
				}
			}
			// With alpha blending
			else{
				while (height>0){
					_line32(
						x,y,
						1,
						width,
						convertRGBto888((uint8_t)r,(uint8_t)g,(uint8_t)b),
						(uint8_t)( a * 255 )
					);
					y++;
					height--;
					r += gR;
					g += gG;
					b += gB;
					a += gA;
				}
			}

		} // 32 bit
	}
	
} // namespace