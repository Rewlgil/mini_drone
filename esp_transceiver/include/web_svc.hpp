/**
 * @file   web_svc.hpp
 * @brief  To provides a web service for remote control applications. Recieve 
 *         virtual joystick input from a client and streaming live video back in real time.
 * @author Rewlgil
 * @date   15 Apr 2025
 * @note   This code implement base on example from 
 *         example\arduino-esp32\libraries\ESP32\examples\Camera\CameraWebServer
 * @copyright
 * Copyright (c) 2025 Rewlgil
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _WEB_SVC_HPP_
#define _WEB_SVC_HPP_

/*============< INCLUDE FILE >===================================================================*/

#include "common.h"

/*============< DATA TYPE DEFINITION >===========================================================*/
/* None */

/*============< ENUM DEFINITION >================================================================*/
/* None */

/*============< MACRO CONSTANT DEFINITION >======================================================*/
/* None */

/*============< MACRO FUNCTION DEFINITION >======================================================*/
/* None */

/*============< DATA STRUCTURE >=================================================================*/
/* None */

/*============< PUBLIC DATA DECLARATION >========================================================*/

extern const uint8_t u1s_index_html_start[] asm("_binary_data_index_html_start");   /**< Binary data embedded in FLASH section */
extern const uint8_t u1s_index_html_end[]   asm("_binary_data_index_html_end");     /**< Binary data embedded in FLASH section */
extern const uint8_t u1s_script_js_start[]  asm("_binary_data_joy_js_start");       /**< Binary data embedded in FLASH section */
extern const uint8_t u1s_script_js_end[]    asm("_binary_data_joy_js_end");         /**< Binary data embedded in FLASH section */

/*============< FUNCTION PROTOTYPES >============================================================*/

extern esp_err_t start_camera_server(void);

/*============< PRIVATE DATA DEFINITION >========================================================*/
/* None */

#endif