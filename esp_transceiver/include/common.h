/**
 * @file   common.h
 * @brief  To provide common project data
 * @author Rewlgil
 * @date   15 Apr 2025
 * @note   -
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

#ifndef _COMMON_H_
#define _COMMON_H_

/*============< INCLUDE FILE >===================================================================*/

#include <stdint.h>
#include "esp32-hal-log.h"
#include "esp_camera.h"

/*============< DATA TYPE DEFINITION >===========================================================*/
/* None */

/*============< ENUM DEFINITION >================================================================*/
/* None */

/*============< MACRO CONSTANT DEFINITION >======================================================*/

#define NULL_CHAR               ('\0')
#define COMMON_VAL_0            (0)
#define COMMON_VAL_1            (1)
#define MIN_JOY_VAL             (-100)
#define MAX_JOY_VAL             (100)

/*============< MACRO FUNCTION DEFINITION >======================================================*/

#define LIMIT(val, min, max)    ((val < min) ? (min) : ((val > max) ? (max) : (val)))

/*============< DATA STRUCTURE >=================================================================*/

typedef struct
{
    int8_t s1_x1;
    int8_t s1_y1;
    int8_t s1_x2;
    int8_t s1_y2;
} ST_JOY_POSITION_T;                            /**< Joy poition structure */

/*============< PUBLIC DATA DECLARATION >========================================================*/
/* None */

/*============< FUNCTION PROTOTYPES >============================================================*/
/* None */

/*============< PRIVATE DATA DEFINITION >========================================================*/
/* None */

#endif