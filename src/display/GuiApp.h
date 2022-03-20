#pragma once
#ifndef _MAC_GUIAPPH_
#define _MAC_GUIAPPH_ 1

#include "display/App.h"
#include "display/Style.h"

/**
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project for creating beautiful and useful
 * apps on various microprocessor boards.
 *
 * mac is distributed under the MIT licence
 **/
namespace mac {

    /**
     * Intention:
     *
     * Panel:
     *      Icon
     *      Name
     *      Acronym
     *      Content (children)
     *          Child panels are added an item to the menu automatically
     *          Child content displays in the content area
     */
    class GuiApp : public App {
    public:
        /**
         * Constructor with display adapter object
         * @param	display			A Display instance for the hardware display being used.
         * @param	bufferHeight	The height of the line buffer, in lines. Default = 1. Full framebuffer = 0.
         **/
        GuiApp( Display* display, Style* style, int bufferHeight = 1 ) : App( display, bufferHeight ) {
            this->style = style;
        }

        /**
         * Destructor
         **/
        ~GuiApp() {}

        /**
         * @brief The style used by the GUI
         */
        Style* style = nullptr;

    };

} // namespace

#endif