/**
 * Border class "mac/μac"
 * Author: Peter "Projectitis" Vullings <peter@projectitis.com>
 * Distributed under the MIT licence
 **/

#include "../Filter.h"

namespace mac{

	Filter* Filter::next(){
		return (Filter*)LinkedList::next();
	}

} //ns:mac
