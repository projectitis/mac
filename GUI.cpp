/**
 * GUI library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/
 
#include "GUI.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{
	
	/**
	 * Constructor with graphics object
	 * @param	graphics		A Graphics instance to render to
	 **/
	GUI::GUI( Graphics* graphics, Style* style ) : PanelSet( style ){
		// Graphics
		this->graphics = graphics;
		_init( style );
	}

	/**
	 * Constructor with display adapter object
	 * @param	display			A Display instance for the hardware display being used
	 **/
	GUI::GUI( Display* display, Style* style ) : PanelSet( style ){
		// Graphics
		this->graphics = new mac::Graphics( display, extension_text );
		_manageGraphics = true; // means we also destroy it
		_init( style );
	}

	/**
	 * Initialise the GUI
	 */
	void GUI::_init( Style* style ){
		this->_lastMicros = micros();
		if (style) this->style = style;
		else{
			this->style = new Style();
			_manageStyle = true;
		}
		this->messenger = new Messenger();
		this->tweens = new Tween();
		this->input = new Input( this->messenger );
		this->input->setMode( mac::InputType::button1, 35 );

		// Test input
		this->messenger->addListener( input_next, this, 0 );
		this->messenger->addListener( input_action, this, 0 );
		this->messenger->addListener( input_back, this, 0 );
	}
	
	/**
	 * Destructor
	 **/
	GUI::~GUI(){
		if (_manageGraphics) delete this->graphics;
		if (_manageStyle) delete this->style;
		delete this->input;
		delete this->tweens;
		delete this->messenger;
	}

	/**
	 * Limit the update time to this speed or slower
	 * @param fps  		Maximum FPS for update
	 */
	void GUI::maxUpdateFrequency( uint8_t fps ){
		if (fps==0) _minDeltaMicros = 0;
		else _minDeltaMicros = 1/fps * 1000000;
	}

	/**
	 * Update the GUI
	 */
	void GUI::update(){
		// Update timer
		_thisMicros = micros();
		_deltaMicros = _thisMicros - _lastMicros;
		_deltaSecs = _deltaMicros * 0.000001;

		// Update input as often as possible
		input->update( _deltaSecs );

		// Exit early if we haven't reached fps limit yet
		// for remaining updates and render
		if (_deltaMicros < _minDeltaMicros) return;

		// Update tweens
		tweens->update( _deltaSecs );

		// Update self
		// ...here...

		// Update children
		Widget::update( _deltaSecs );

		// Update and render display objects
		this->render();

		// Update timer. We do this last because someone might want to know
		// value of lastMicros and thisMicros during update.
		_lastMicros = _thisMicros;
	}

	/**
	 * Render the GUI
	 */
	void GUI::render(){
		// Render parent
		boolean childRendered = PanelSet::render( graphics );
		// Update the display if there have been changes
		if (childRendered) graphics->update();
	}

	/**
	 * If this widget is listening for any events, this method is called when those
	 * events are triggered. If the method returns false, the event listener is removed.
	 * If it returns true, the listener is kept in place.
	 * @param  event  	The event that was fired
	 * @param  data  	Any user data registered with the listener
	 * @return       flase to remove the listener. True otherwise
	 */
	boolean GUI::listen( uint32_t event, void* data ){
		if (event == mac::Event::input_next) Serial.print("Event NEXT\n");
		else if (event == mac::Event::input_action) Serial.print("Event ACTION\n");
		else if (event == mac::Event::input_back) Serial.print("Event BACK\n");
		else Serial.printf("Event %d\n", event);
		return true;
	}
	
} // namespace