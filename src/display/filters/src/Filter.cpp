#include "../Filter.h"

namespace mac{

	Filter* Filter::next(){
		return (Filter*)LinkedList::next();
	}

	void Filter::add( Filter* filter ) {
		LinkedList::add( (LinkedList*)filter );
	}

} //ns:mac
