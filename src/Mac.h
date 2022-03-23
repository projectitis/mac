/**
 * @file mac.h
 * @author Peter "Projectitis" Vullings <peter@projectitis.com>
 * @brief GUI library for "mac/μac"
 * @version 0.2
 * @date 2021-05-18
 * @copyright Copyright (c) 2021
 * @license https://choosealicense.com/licenses/mit/
 *
 * mac (or μac) stands for "Microprocessor App Creator"
 * mac is a project for creating beautiful and useful
 * apps on various microprocessor boards.
 *
 * Version 0.2 supports
 *      Teensy
 *
 * Planned support for
 *      ESP32 (via Arduino)
 */

#pragma once
#ifndef _MAC_H_
#define _MAC_H_ 1

#define MAC_VERSION 0.2

 /**
  * Include all mac headers
  */
#include "Common.h"
#include "display/GuiApp.h"
#include "display/hardware/DisplayILI9341.h"
#include "physics/Tween.h"

#endif