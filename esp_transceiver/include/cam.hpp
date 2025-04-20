/**
 * @file   cam.hpp
 * @brief  Camera driver module
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

#ifndef _CAM_HPP_
#define _CAM_HPP_

/*============< INCLUDE FILE >===================================================================*/

#include "common.h"

/*============< DATA TYPE DEFINITION >===========================================================*/
/* None */

/*============< ENUM DEFINITION >================================================================*/
/* None */

/*============< MACRO CONSTANT DEFINITION >======================================================*/

/* Define camera hardware connection */
#define PWDN_GPIO_NUM           (32)
#define RESET_GPIO_NUM          (-1)
#define XCLK_GPIO_NUM           (0)
#define SIOD_GPIO_NUM           (26)
#define SIOC_GPIO_NUM           (27)
#define Y9_GPIO_NUM             (35)
#define Y8_GPIO_NUM             (34)
#define Y7_GPIO_NUM             (39)
#define Y6_GPIO_NUM             (36)
#define Y5_GPIO_NUM             (21)
#define Y4_GPIO_NUM             (19)
#define Y3_GPIO_NUM             (18)
#define Y2_GPIO_NUM             (5)
#define VSYNC_GPIO_NUM          (25)
#define HREF_GPIO_NUM           (23)
#define PCLK_GPIO_NUM           (22)
#define LED_GPIO_NUM            (4)

#define XCLK_FREQ_HZ            (20000000)
#define CAM_JPEG_QUALITY        (12)
#define NUM_FRAME_BUFF_ALLOC    (1)

#define JPEG_CONVERT_QUALITY    (80)

/*============< MACRO FUNCTION DEFINITION >======================================================*/
/* None */

/*============< DATA STRUCTURE >=================================================================*/
/* None */

/*============< PUBLIC DATA DECLARATION >========================================================*/
/* None */

/*============< FUNCTION PROTOTYPES >============================================================*/

extern esp_err_t camera_init(void);

/*============< PRIVATE DATA DEFINITION >========================================================*/
/* None */

#endif