#include <DisplayILI9341.h>
#include <Graphics.h>

mac::Graphics* graphics;
uint32_t s;

void setup() {
	// Initialise the serial for debugging info
	Serial.begin( 9600 );
	delay( 1000 );

	// Create a name mac graphics object for the ILI9341 display
 	graphics = new mac::Graphics(
 		new mac::DisplayILI9341( 10, 15, 4, 11, 13, 12, 6 )
 	);

	// Clear the display to black
 	graphics->clear( mac::RGB565_BLACK );

 	// Start timing
 	s = micros();

	// Set the fill style to orange 80% transparency
	graphics->vector->fillStyle( 0xff9900, 0.8 );

	// Draw a couple of overlapping triangles
	graphics->vector->triangle( 100,20, 150,100,  103,150 );
	graphics->vector->triangle( 50,80, 220,90,  130,200 );

	// Add a line style, and draw another
	graphics->vector->lineStyle( 0xffffff, 1 );
	graphics->vector->triangle( 100,180, 300,190,  200,230 );

	// Finish timing and output debugging into
	Serial.print( micros()-s );
	Serial.println(F(" micro secs"));

	// Update graohics to display
	graphics->update();
}

void loop() {
	// Wait forever
	delay( 200 );
}
