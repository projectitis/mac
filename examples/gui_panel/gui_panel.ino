#include <mac.h>

mac::GUI* gui;
mac::Panel* panel;

void setup() {
	// Initialise the serial for debugging info
	Serial.begin( 9600 );
	delay( 1000 );

	// Create a GUI object and specify the ILI9341 display
 	gui = new mac::GUI(
 		new mac::DisplayILI9341( 10, 15, 4, 11, 13, 12, 6 )
 	);

 	// Create new panel and add it to the GUI
 	panel = gui->addPanel( "Example blade", "ExB" );

 	gui->update();
}

void loop() {
	// Update GUI
	//gui->update();
}