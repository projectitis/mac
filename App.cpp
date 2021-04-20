/**
 * App framework for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/
 
#include "App.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	/**
	 * Constructor without any graphical elements.
	 **/
	App::App(){
		this->_init();
	}

	/**
	 * Constructor with display adapter object
	 * @param	display			A Display instance for the hardware display being used
	 **/
	App::App( Display* display ){
		// Graphics
		this->display = display;
		this->_init();
	}

	/**
	 * Initialise the app
	 */
	void App::_init(){
		this->lastMicros = micros();

		this->stage = new Stage();
		this->messenger = new Messenger();
		this->tweens = new Tween();
		this->input = new Input( this->messenger );
	}
	
	/**
	 * Destructor
	 **/
	App::~App(){
		if (this->display) delete this->display;

		delete this->input;
		delete this->tweens;
		delete this->messenger;
		delete this->stage;
	}

	/**
	 * Limit the update time to this speed or slower
	 * @param fps  		Maximum FPS for update
	 */
	void App::setRenderFPS( uint16_t fps ){
		if (fps==0) _renderDeltaMicrosMax = 0;
		else _renderDeltaMicrosMax = (1/(float)fps) * 1000000;
	}

	/**
	 * Update the app
	 */
	void App::update(){
		// Update timer
		thisMicros = micros();
		deltaMicros = thisMicros - lastMicros;
		deltaSecs = deltaMicros * 0.000001;

		// Update input and messenger as often as possible
		input->update( deltaSecs );
		messenger->update( deltaSecs );

		// User update functions
		messenger->sendMessage( Event::update_all );

		// Perform the fps limited functions, which include
		// tweening and rendering.
		renderDeltaMicros += deltaMicros;
		if (renderDeltaMicros >= _renderDeltaMicrosMax){
			renderDeltaSecs = renderDeltaMicros * 0.000001;

			// Update tweens
			tweens->update( renderDeltaSecs );

			// Render the stage
			stage->render( display );

			// User render functions
			messenger->sendMessage( Event::update_render ); 

			renderDeltaMicros -= _renderDeltaMicrosMax;
		}

		// Update timer. We do this last because someone might want to know
		// value of lastMicros and thisMicros during update.
		lastMicros = thisMicros;
	}

	void App::serialBegin( boolean waitUntilReady ){
		// Initialise the serial for debugging info
		Serial.begin( 9600 );
		if (waitUntilReady){
			for (uint8_t i=0; i<20; i++){
				delay( 100 );
				if (Serial) break;
			}
		}
	}
	
} // namespace