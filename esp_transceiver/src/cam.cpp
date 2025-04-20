/**
 * @file   cam.cpp
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

/*============< INCLUDE FILE >===================================================================*/

#include "cam.hpp"

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
/* None */

/*============< FUNCTION PROTOTYPES >============================================================*/
/* None */

/*============< PRIVATE DATA DEFINITION >========================================================*/

/**
 * @brief       To Initialize camera module
 * @param[-]    -
 * @return      ESP_OK     Operation successfully
 *              ESP_FAIL   Operation fail
 */
esp_err_t camera_init(void)
{
    esp_err_t ent_ret_sts;                                  /**< Function status */
    camera_config_t stt_cam_cfg = { COMMON_VAL_0 };         /**< Camera initialization structure */
    sensor_t *stt_cam_ctrl;                                 /**< Camera configuration structure */

    /*====< INPUT >==========================================================*/
    /* None */

    /*====< OPERATION >======================================================*/

    /* Loading camera configuration */
    stt_cam_cfg.ledc_channel = LEDC_CHANNEL_0;
    stt_cam_cfg.ledc_timer = LEDC_TIMER_0;
    stt_cam_cfg.pin_d0 = Y2_GPIO_NUM;
    stt_cam_cfg.pin_d1 = Y3_GPIO_NUM;
    stt_cam_cfg.pin_d2 = Y4_GPIO_NUM;
    stt_cam_cfg.pin_d3 = Y5_GPIO_NUM;
    stt_cam_cfg.pin_d4 = Y6_GPIO_NUM;
    stt_cam_cfg.pin_d5 = Y7_GPIO_NUM;
    stt_cam_cfg.pin_d6 = Y8_GPIO_NUM;
    stt_cam_cfg.pin_d7 = Y9_GPIO_NUM;
    stt_cam_cfg.pin_xclk = XCLK_GPIO_NUM;
    stt_cam_cfg.pin_pclk = PCLK_GPIO_NUM;
    stt_cam_cfg.pin_vsync = VSYNC_GPIO_NUM;
    stt_cam_cfg.pin_href = HREF_GPIO_NUM;
    stt_cam_cfg.pin_sccb_sda = SIOD_GPIO_NUM;
    stt_cam_cfg.pin_sccb_scl = SIOC_GPIO_NUM;
    stt_cam_cfg.pin_pwdn = PWDN_GPIO_NUM;
    stt_cam_cfg.pin_reset = RESET_GPIO_NUM;
    stt_cam_cfg.xclk_freq_hz = XCLK_FREQ_HZ;
    stt_cam_cfg.frame_size = FRAMESIZE_UXGA;
    stt_cam_cfg.pixel_format = PIXFORMAT_JPEG;
    stt_cam_cfg.grab_mode = CAMERA_GRAB_LATEST;
    stt_cam_cfg.fb_location = CAMERA_FB_IN_PSRAM;
    stt_cam_cfg.jpeg_quality = CAM_JPEG_QUALITY;
    stt_cam_cfg.fb_count = NUM_FRAME_BUFF_ALLOC;

    /* Initialize the camera driver */
    ent_ret_sts = esp_camera_init(&stt_cam_cfg);

    if (ent_ret_sts != ESP_OK)
    {
        /* @TODO add error handle */
        log_e("Camera Init fail");
    }
    else
    {
        /* Get a pointer to the image sensor control structure */
        stt_cam_ctrl = esp_camera_sensor_get();

        if (stt_cam_ctrl != NULL)
        {
            /* Modify carmera setting */
            stt_cam_ctrl->set_framesize(stt_cam_ctrl, FRAMESIZE_QVGA);
            // stt_cam_ctrl->set_vflip(stt_cam_ctrl, 1);
            // stt_cam_ctrl->set_hmirror(stt_cam_ctrl, 1);
            ent_ret_sts = ESP_OK;
        }
        else
        {
            /* @TODO add error handle */
            log_e("Camera configuration fail");
            ent_ret_sts = ESP_FAIL;
        }
    }

    /*====< OUTPUT >=========================================================*/

    return ent_ret_sts;
}