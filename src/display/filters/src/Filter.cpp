#include "../Filter.h"

namespace mac{

	Filter* Filter::next(){
		return (Filter*)LinkedList::next();
	}

} //ns:mac
