/**
 * Vehicle physics for "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/

#include <math.h>
#include <Arduino.h>
#include "Vehicle.h"

namespace mac{

	#define M_PI_180 M_PI/180

	Vehicle::Vehicle(){
		this->x = 0;
		this->y = 0;
		this->bearing = new Vector2F();
	}

	Vehicle::Vehicle( float x, float y ){
		this->x = x;
		this->y = y;
		this->bearing = new Vector2F();
	}

	void Vehicle::update( float dt ){
		this->bearing->m *= (1.0-(this->f*dt));
		this->bearing->rotate( this->s * dt );

		this->x += this->bearing->x * dt;
		this->y += this->bearing->y * dt;
	}

	void Vehicle::setSpeed( float s ){
		this->bearing->m = s;
		this->bearing->calcXY();
	}

	void Vehicle::setHeading( float a ){
		this->bearing->a = 0;
		this->bearing->rotate( a * M_PI_180 );
	}

	void Vehicle::steer( float s ){
		this->s = min( this->maxSteer, max( -this->maxSteer, s )) * M_PI_180;
	}

} //ns:mac
