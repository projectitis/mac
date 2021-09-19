/**
 * Borders class "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/

#include "../Borders.h"

namespace mac{

	Borders::Borders(){
		this->left = new Border();
		this->right = new Border();
		this->top = new Border();
		this->bottom = new Border();
	}
	Borders::Borders( float_t size, color888 color, float_t alpha ){
		this->left = new Border( size, color, alpha );
		this->right = new Border( size, color, alpha );
		this->top = new Border( size, color, alpha );
		this->bottom = new Border( size, color, alpha );
	}
	Borders::~Borders(){
		delete this->left;
		delete this->right;
		delete this->top;
		delete this->bottom;
	}

} //ns:mac
