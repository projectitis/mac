#include "../GlyphInfo.h"

namespace mac {

	GlyphInfo::GlyphInfo() {
		bounds = new ClipRect();
	}

	GlyphInfo::~GlyphInfo() {
		delete bounds;
	}

	void GlyphInfo::reset() {
		bounds->clear();
		_next = 0;
		_prev = 0;
		lineRepeat = 0;
		dataOffset = 0;
		renderStarted = false;
		code = 0;
	}

	void GlyphInfo::set( char c ) {
		code = c;
	}

} // namespace