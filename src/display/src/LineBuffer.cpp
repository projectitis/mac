#include "../LineBuffer.h"

namespace mac {

    LineBuffer::LineBuffer( Display* display ) {
        _display = display;
        rect.setPosAndSize( 0, 0, _display->width(), _display->height() );
        _region.set( &rect );
        _data[0].pixels = new color888[_display->width()];
        _data[1].pixels = new color888[_display->width()];
    }

    LineBuffer::~LineBuffer() {
        delete[] _data[0].pixels;
        delete[] _data[1].pixels;
    }

    void LineBuffer::setRegion( ClipRect* rect ) {
        // Ensure region is within display area
        _region.set(
            max( 0, rect->x ),
            max( 0, rect->y ),
            min( _display->width(), rect->x2 ),
            min( _display->height(), rect->y2 )
        );
        resetRegion();
    }

    void LineBuffer::resetRegion() {
        _data[_frontIndex].y = _region.y;
        _data[_frontIndex].x = _region.x;
        _data[_frontIndex].x2 = _region.x2;
    }

    void LineBuffer::flip() {
        _backIndex = _frontIndex;
        _frontIndex ^= 1;
        _data[_frontIndex].y = _data[_backIndex].y + 1;
        if ( _data[_frontIndex].y > _region.y2 ) _data[_frontIndex].y = _region.y;
        _data[_frontIndex].x = _data[_backIndex].x;
        _data[_frontIndex].x2 = _data[_backIndex].x2;
        // Push to the hardware from the backbuffer
        while ( !_display->ready ) yield();
        _display->draw( _data[_backIndex].y, _data[_backIndex].x, _data[_backIndex].x2, _data[_backIndex].pixels );
    }

} // namespace