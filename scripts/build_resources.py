from glob import glob
from os import path
from PIL import Image

###
### Compile resources to c header files
###

# This code is available under the MIT license
# Copyright (c) 2018 Peter Vullings (Projectitis)
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
# BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
# ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

# Step through all files in the 'resources' folder and prepare
# a header file for each so that the resource is compiled into the
# program.

# The filename will determine the header file name and variable name.
# Optional parameters may be included in the filename of the resource.

# Will process only:
#
# IMAGES
#		.bmp
#				Will create a uint16_t array of RGB565 pixel values.
#		.png
#				Will create a uint8_t array of 24bit pixels - one
#				uint16_t RGB565 followed by a uint8_t alpha value.
#				If the PNG does not contain any transparency, the
#				image will be processed as a bmp
#
#		Options:
#				t-__x__
#						will process the image as individual tiles.
#						instead of all pixels top to bottom, the array will
#						contain the pixels of the first tile, followed by the
#						second etc. Examples:
#						t-32x32		(tileset.t-32x32.bmp)
#						t-8x16		(character_sprites.t-8x16.png)
#				f-___
#						Specify how to format the output data in the header.
#						Currently the options are:
#						f-mac		Format the data as a TileMap struct as
#									used by the mac library.
#						f-min		Include only the data and no additional
#									variables such as size or tile details.
#						f-max		As well as the data, include variables for
#									data size, tile size, tile counts etc.
#									This is also the default.
#						
#
# MUSIC
#		.xm
#		.mod
#		.stm
#		.it
#				Will create a byte array of raw mod data
#
#		Options:
#				f-pmf (unsupported)
#						will convert the file to PMF format
#				f-libxmize (unsupported)
#						will libxmize the file to internal format for the
#						libxm player
# Get list of files in resources folder
resources = glob('../resources/*.*')
for file in list(resources):
	# Split the filename by dot to get the parts, including options
	filename = path.basename(file)
	fileparts = filename.split('.')
	if len(fileparts) < 2:
		continue
	
	# first and last parts of filename are name and extn
	name = fileparts.pop(0)
	extn = fileparts.pop().lower();
	
	# any remaining parts are options
	options = {}
	for opstr in fileparts:
		options[opstr[:1]] = opstr[2:]
	
	# Start the output
	outstr = '';
	
	### IMAGES
	if extn in ['png','bmp','jpg','jpg']:
		# load the image
		im = Image.open(file)
		
		# Check if the image has alpha
		alpha = im.mode == 'RGBA'
		
		# Some useful messages to console
		print()
		print('Processing',filename,'(image)')
		print('Image mode:',im.mode)
		
		# Output format
		ofmt = '';
		if 'f' in options:
			ofmt = options['f'];
			
		
		# Get image dimensions
		width, height = im.size
		tilewidth, tileheight = width, height
		rows, cols = 1, 1
		tilesize = tilewidth*tileheight
		
		# Option: t-__x__
		# Are we splitting the image in to tiles?
		if 't' in options:
			tilesize = [int(x) for x in options['t'].split('x')]
			if len(tilesize)==2:
				tilewidth, tileheight = tilesize[:2]
				if (width%tilewidth==0) and (height%tileheight==0):
					rows = int(height/tileheight)
					cols = int(width/tilewidth)
					print('Slicing',(cols*rows),'tiles at',tilewidth,'x',tileheight)
					
		# Holds pixel data
		p = [];
					
		# steps tiles
		for row in range(rows):
			for col in range(cols):
				# step pixels in tile
				for y in range(tileheight):
					for x in range(tilewidth):
						if alpha:
							# get pixels including alpha
							r,g,b,a = im.getpixel((col*tilewidth+x,row*tileheight+y))
							# combine to 5658 24bit as 3 bytes
							p.append( # RRRRRGGG
								(r & 0b11111000)
								| ((g >> 5) & 0b00000111)
								)
							p.append( # GGGBBBBB
								((g << 3) & 0b11100000)
								| ((b & 0b11111000) >> 3)
								)
							p.append(a) # AAAAAAAA
						else:
							# get pixels without alpha
							r,g,b = im.getpixel((col*tilewidth+x,row*tileheight+y))
							# combine to 565 16bit
							p.append(
								int((r << 8) & 0b1111100000000000)
								| int((g << 3) & 0b0000011111100000)
								| int((b >> 3) & 0b0000000000011111)
								)
				
		# start output
		if  not ((ofmt=='min') or (ofmt=='mac')):
			outstr += 'const uint32_t '+name+'_twidth = '+str(tilewidth)+';\n'
			outstr += 'const uint32_t '+name+'_theight = '+str(tileheight)+';\n'
			outstr += 'const uint32_t '+name+'_tcount = '+str(rows*cols)+';\n'
			if alpha:
				outstr += 'const uint32_t '+name+'_tstride = '+str(tilewidth*tileheight*3)+';\n'
			else:
				outstr += 'const uint32_t '+name+'_tstride = '+str(tilewidth*tileheight)+';\n'
			outstr += 'const uint32_t '+name+'_size = '+str(len(p))+';\n'
		
		# Output pixels
		if alpha:
			outstr += '__attribute__((aligned(4))) const uint8_t '+name+'_data[] = {\n'
		else:
			outstr += 'const uint16_t '+name+'_data[] = {\n'	
		c = 0
		f = True
		if alpha:
			print(len(p),'bytes in output as 3-byte words');
			# Step pixels and output in groups of 48
			for pc in p:
				if f:
					outstr += ' '
					f = False
				else:
					outstr += ','
				outstr += '0x{:02x}'.format(pc)
				c += 1
				if c == 48:
					outstr += '\n'
					c = 0
		else:
			print((len(p)*2),'bytes in output as 16bit words');
			# Step pixels and output in groups of 32
			for pc in p:
				if f:
					outstr += ' '
					f = False
				else:
					outstr += ','
				outstr += '0x{:04x}'.format(pc)
				c += 1
				if c == 32:
					outstr += '\n'
					c = 0
		outstr += '};\n'
		
		# output footer
		if ofmt=='mac':
			# typedef struct {
			# 	uint32_t tileWidth;
			# 	uint32_t tileHeight;
			# 	uint32_t tileCount;
			# 	uint32_t tileStride;
			# 	uint8_t pixelFormat;
			# 	uint32_t dataSize;
			# 	union {
			# 		uint8_t* data8;
			# 		uint16_t* data16;
			# 	} __attribute__ ((aligned (4)));;
			# } TileMap;
			outstr += 'const mac::TileMap '+name+' = {\n'
			outstr += '\t'+str(tilewidth)+',\n'
			outstr += '\t'+str(tileheight)+',\n'
			outstr += '\t'+str(rows*cols)+',\n'
			outstr += '\t'+str(tilewidth*tileheight)+',\n'
			if alpha:
				outstr += '\tmac::PF_RGBA5658,\n'
			else:
				outstr += '\tmac::PF_RGB565,\n'
			outstr += '\t'+str(len(p))+',\n'
			if alpha:
				outstr += '\t{ .data16=(uint16_t*)'+name+'_data }\n'
			else
				outstr += '\t{ .data8=(uint8_t*)'+name+'_data }\n'
			outstr += '};\n'
			
	### MUSIC
	elif extn in ['xm','mod','stm','it']:
		# Generate file header
		outstr += '// Tracker module as byte array\n'
		outstr += 'const char '+name+'[] = {\n'
		
		# Some useful messages to console
		print()
		print('Processing',filename,'(mod)')
		
		# Open raw file and step bytes
		c = 1
		b = 1
		with open(file, "rb") as f:
			byte = f.read(1)
			outstr += ' {:3d}'.format(byte[0])
			while byte:
				byte = f.read(1)
				if byte:
					outstr += ',{:3d}'.format(byte[0])
					c += 1
					b += 1
					if c == 64:
						outstr += '\n'
						c = 0
		
		# End file
		outstr += '};\n'
		outstr += 'const uint32_t '+name+'_size = '+str(b)+';\n'
		
		# console
		print(b,'bytes in output');
				
	### Write to file
	if outstr:
		outfile = open('../resources/'+name+'.h', 'w')
		outfile.write(outstr)
		outfile.close()			
				
# Wait before exit
print()
input("Press enter to exit")