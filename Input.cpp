/**
 * GUI library for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/
 
#include "Input.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project that enables creating beautiful and useful apps on the
 * Teensy microprocessor, but hopefully is generic enough to be ported to other
 * microprocessor boards. The various libraries that make up mac might also
 * be useful in other projects.
 **/
namespace mac{

	/**
	 * Constructor
	 * 
	 */
	Input::Input( Messenger* messenger ){
		_messenger = messenger;
	}

	/**
	 * Sets the input mode to one of the preset modes. Additional mappings can be added
	 * as well, or instead of, setting the mode. See the definition for InputType for the
	 * pins that should be specified when calling this method.
	 * @param mode 		The input mode
	 */
	void Input::setMode( InputType mode, pin p1, pin p2, pin p3, pin p4, pin p5, pin p6 ){
		unmapAll();
		_mode = mode;
		switch (mode){
			case mac::InputType::button1:
				map( p1, mac::PinType::digital, mac::ClickType::click, mac::Event::input_next );
				map( p1, mac::PinType::digital, mac::ClickType::dbl_click, mac::Event::input_action );
				map( p1, mac::PinType::digital, mac::ClickType::hold, mac::Event::input_back );
				break;
			default:
				break;
		}
	}

	/**
	 * Sets analogue joystick pins. These are mapped automatically to the events for
	 * input_up, input_down, input_left and input_right with a threshhold of 50%.
	 * @param up    Pin to use for up
	 * @param down  Pin to use for down
	 * @param left  Pin to use for left
	 * @param right Pin to use for right
	 */
	void Input::setJoyPins( pin up, pin down, pin left, pin right ){

	}

	/**
	 * Sets D-Pad pins. These are mapped automatically to the events for
	 * input_up, input_down, input_left and input_right.
	 * @param up    Pin to use for up
	 * @param down  Pin to use for down
	 * @param left  Pin to use for left
	 * @param right Pin to use for right
	 */
	void Input::setDPadPins( pin up, pin down, pin left, pin right ){

	}

	/**
	 * Map a specific pin to an input event. Multiple events can be mapped to the same pin, as long
	 * as a different clickType is used for each. Examples:
	 * 		map( 12, digital, click, input_select ); // Map a click on pin 12 to the 'select' event
	 * 		map( 7, analog_mid, move, input_vaxis, 0.2 ); // Map analogue movement on pin 7 to the 'vertical axis' event after moving 20% from neutral
	 * 		map( 16, digital, click, input_next );			// Map a click on pin 16 to 'next', AND
	 * 		map( 16, digital, dbl_click, input_previous );	// Map a dbl-click on the same pin to 'previous'
	 * @param  pin       	The pin to map
	 * @param  pinType   	Digital or analogue pin type
	 * @param  clickType 	The type of click
	 * @param  event     	The event to fire
	 * @param  threshhold   If the pin is analogue, the associated threshhold
	 * @return           The input for chaining
	 */
	Input* Input::map( pin pin, PinType pinType, ClickType clickType, Event event, uint16_t threshhold ){

		// First unmap the pin/clickType if it exists already
		unmap( pin, clickType );
		 
		// First find pin
		InputPin* ip = _pins;
		while (ip){
			if (ip->pin == pin) break;
			ip = ip->next;
		}
		// if pin doesn't exist, create it and add it
		if (!ip || (ip->pin!=pin)){
			ip = new InputPin();
			ip->pin = pin;
			ip->pinType = pinType;
			ip->state = mac::PinState::psRest;
			pinMode(pin, INPUT);

			ip->next = _pins;
			_pins = ip;
		}

		// Create and add the mapping
		InputMap* im = new InputMap();
		im->clickType = clickType;
		im->event = event;
		im->threshhold = threshhold;
		im->next = ip->mappings;
		ip->mappings = im;

		// Chaining
		return this;
	}

	/**
	 * Remove a pin mapping from a pin.
	 * @param  	pin 		The pin to unmap
	 * @param 	ClickType 	The click type to unmap. If 0 will unmap all clickTypes for pin
	 * @return     The input for chaining
	 */
	Input* Input::unmap( pin pin, ClickType clickType ){
		InputPin* ip = _pins;
		while (ip){
			if (ip->pin == pin){
				InputMap* im = ip->mappings;
				InputMap* im_prev = 0;
				while (im){
					InputMap* im_next = im->next;
					if (( clickType == ClickType::none ) || ( im->clickType == clickType )){
						if (im_prev) im_prev->next = im->next;
						delete im;
					}
					else{
						im_prev = im;
					}
					im = im_next;
				}
				if ( clickType == ClickType::none ){
					delete ip;
				}
				break;
			}
			ip = ip->next;
		}
		return this;
	}

	/**
	 * Remove all pin mappings
	 * @return     The input for chaining
	 */
	Input* Input::unmapAll(){
		InputPin* ip = _pins;
		while (ip){
			InputPin* ip_next = ip->next;
			InputMap* im = ip->mappings;
Serial.printf("Unmapping pin %d\n", ip->pin);
			while (im){
				InputMap* im_next = im->next;
Serial.printf("  Click type %d\n", im->clickType);
				delete im;
				im = im_next;
			}
			delete ip;
			ip = ip_next;
		}
		return this;
	}

	/**
	 * Set the active state of the Input object. If false will not fire input events, but will
	 * still process pins when update() is called. 
	 * @param 	state 	true if active, false if not active
	 */
	void Input::setActive( boolean state ){
		_active = state;
	}

	/**
	 * Update inputs
	 */
	void Input::update( float dt ){
		InputPin* ip = _pins;
		int buttonState;
		ClickType buttonClickType;
		while (ip){
			buttonState = digitalRead(ip->pin);
			buttonClickType = mac::ClickType::none;

			// Process digital pin
			if (ip->pinType == mac::PinType::digital){
				// State based
				switch (ip->state){

					// Button is resting
					case mac::PinState::psRest:
						if (buttonState==HIGH){
							ip->state = mac::PinState::psDown1DB;
							ip->time = 0;
						}
						break;

					// Button debouncing for first push
					case mac::PinState::psDown1DB:
						ip->time += dt;
						if (ip->time >= Input::TIME_DEBOUNCE){
							if (buttonState==HIGH){
								buttonClickType = mac::ClickType::down;
								ip->state = mac::PinState::psDown1;
								ip->time = 0;
							}
							else{
								ip->state = mac::PinState::psRest; // cancel
							}
						}
						break;

					// Button down for first click
					case mac::PinState::psDown1:
						ip->time += dt;
						if (ip->time >= Input::TIME_HOLD){
							ip->state = mac::PinState::psHold;
							buttonClickType = mac::ClickType::hold;
						}
						else if (buttonState==LOW){
							if (ip->time < TIME_PRESS){
								ip->state = mac::PinState::psUp1DB;
								ip->time = 0;
							}
							else{
								ip->state = mac::PinState::psPress1DB;
								ip->time = 0;
							}
						}
						break;

					// Button held on first push
					case mac::PinState::psHold:
						if (buttonState==LOW){
							ip->state = mac::PinState::psLastDB;
							ip->time = 0;
						}
						break;

					// Button debouncing for first release
					case mac::PinState::psUp1DB:
						ip->time += dt;
						if (ip->time >= Input::TIME_DEBOUNCE){
							if (buttonState==LOW){
								buttonClickType = mac::ClickType::up;
								ip->state = mac::PinState::psUp1;
								ip->time = 0;
							}
							else{
								ip->state = mac::PinState::psRest; // cancel
							}
						}
						break;

					// Button debouncing for first release
					case mac::PinState::psUp1:
						ip->time += dt;
						if (buttonState==HIGH){
							ip->state = mac::PinState::psDown2DB;
							ip->time = 0;
						}
						else if (ip->time >= Input::TIME_DBLCLICK){
							ip->state = mac::PinState::psRest;
							buttonClickType = mac::ClickType::click;
						}
						break;

					// Button debouncing for first press release
					case mac::PinState::psPress1DB:
						ip->time += dt;
						if (ip->time >= Input::TIME_DEBOUNCE){
							if (buttonState==LOW){
								buttonClickType = mac::ClickType::press;
								ip->state = mac::PinState::psRest;
							}
							else{
								// XXX: Maybe go back to down1 as if release never happened?
								ip->state = mac::PinState::psRest; // cancel
							}
						}
						break;

					// Button debouncing for second push
					case mac::PinState::psDown2DB:
						ip->time += dt;
						if (ip->time >= Input::TIME_DEBOUNCE){
							if (buttonState==HIGH){
								buttonClickType = mac::ClickType::down;
								ip->state = mac::PinState::psDown2;
								ip->time = 0;
							}
							else{
								ip->state = mac::PinState::psRest; // cancel
							}
						}
						break;

					// Button down for second click
					case mac::PinState::psDown2:
						ip->time += dt;
						if (ip->time >= Input::TIME_HOLD){
							ip->state = mac::PinState::psHold;
							buttonClickType = mac::ClickType::click_hold;
						}
						else if (buttonState==LOW){
							if (ip->time < TIME_PRESS){
								ip->state = mac::PinState::psUp2DB;
								ip->time = 0;
							}
							else{
								ip->state = mac::PinState::psPress2DB;
								ip->time = 0;
							}
						}
						break;

					// Button debouncing for first release
					case mac::PinState::psUp2DB:
						ip->time += dt;
						if (ip->time >= Input::TIME_DEBOUNCE){
							if (buttonState==LOW){
								buttonClickType = mac::ClickType::dbl_click;
								ip->state = mac::PinState::psRest;
							}
							else{
								// XXX: Maybe return to down2 as if LOW was never detected?
								ip->state = mac::PinState::psRest; // cancel
							}
						}
						break;

					// Button debouncing for second press release
					case mac::PinState::psPress2DB:
						ip->time += dt;
						if (ip->time >= Input::TIME_DEBOUNCE){
							if (buttonState==LOW){
								buttonClickType = mac::ClickType::click_press;
								ip->state = mac::PinState::psRest;
							}
							else{
								// XXX: Maybe go back to down2 as if release never happened?
								ip->state = mac::PinState::psRest; // cancel
							}
						}
						break;

					// Button debouncing for return to rest
					case mac::PinState::psLastDB:
						ip->time += dt;
						if (ip->time >= Input::TIME_DEBOUNCE){
							if (buttonState==LOW){
								buttonClickType = mac::ClickType::up;
								ip->state = mac::PinState::psRest;
								ip->time = 0;
							}
						}
						break;
				}

			}

			// Process analogue pin
			else{


			}

			// If we have triggered a ClickType, see if it is mapped (only if active)
			if ( ( buttonClickType != ClickType::none ) && _active){
				InputMap* im = ip->mappings;
				// If the click type is press or click_press, we also need to fire the 'up' events
				boolean alsoUp = ( buttonClickType == ClickType::press ) || ( buttonClickType == ClickType::click_press );
				while (im){
					// Trigger event for click type
					if (im->clickType == buttonClickType){
						_messenger->sendMessage( im->event );
					}
					// Trigger event for alternative click type
					if (alsoUp && (im->clickType == mac::ClickType::up)){
						_messenger->sendMessage( im->event );
					}
					im = im->next;
				}
			}
			ip = ip->next;
		}
	}
	
} // namespace