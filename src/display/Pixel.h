#pragma once
#ifndef _MAC_PIXELH_
#define _MAC_PIXELH_ 1

#include "Common.h"
#include "graphics/Color.h"
#include "utils/LinkedList.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project for creating beautiful and useful
 * apps on various microprocessor boards.
 *
 * mac is distributed under the MIT licence
 **/
namespace mac {

    /**
     * Stacked pixel class for blending and masking pixels during rendering (sued by Stage)
     */
    class Pixel : public LinkedList<Pixel> {
    public:
        color888 c = 0;
        float_t a = 0.0;
        boolean m = false;

        /**
         * Memory pool of recycled objects
         */
        static Pixel* pool;

        /**
         * Create a new object or take one from the pool
         * @return The new or recycled object
         */
        static Pixel* Create( color888 color, float_t alpha, boolean mask );
    };

} // namespace

#endif