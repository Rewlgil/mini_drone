/**
 * @file   web_svc.cpp
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

/*============< INCLUDE FILE >===================================================================*/

#include "esp_http_server.h"
#include "esp_timer.h"
#include "cJSON.h"
#include "web_svc.hpp"
#include "cam.hpp"

/*============< DATA TYPE DEFINITION >===========================================================*/
/* None */

/*============< ENUM DEFINITION >================================================================*/
/* None */

/*============< MACRO CONSTANT DEFINITION >======================================================*/

/* Web server configuration */
#define INDEX_HTTPD_SERVER_PORT             (80U)
#define STREAM_HTTPD_SERVER_PORT            (81U)
#define INDEX_HTTPD_CTRL_PORT               (32768U)
#define STREAM_HTTPD_CTRL_PORT              (32769U)
#define INDEX_HTTPD_TASK_PRIORITY           (tskIDLE_PRIORITY + 6U)
#define STREAM_HTTPD_TASK_PRIORITY          (tskIDLE_PRIORITY + 5U)
#define POST_HANDLE_BUFF_LEN                (100U)
#define HTTP_REPS_HEADER_BUFF_LEN           (128U)
#define HTTP_STREAM_BOUNDARY_STR "123456789000000000000987654321"
static const char *STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" HTTP_STREAM_BOUNDARY_STR;
static const char *STREAM_BOUNDARY = "\r\n--" HTTP_STREAM_BOUNDARY_STR "\r\n";
static const char *STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\nX-Timestamp: %d.%06d\r\n\r\n";

/*============< MACRO FUNCTION DEFINITION >======================================================*/
/* None */

/*============< DATA STRUCTURE >=================================================================*/
/* None */

/*============< PUBLIC DATA DECLARATION >========================================================*/
/* None */

/*============< FUNCTION PROTOTYPES >============================================================*/

static esp_err_t index_get_handler(httpd_req_t *req);
static esp_err_t script_get_handler(httpd_req_t *req);
static esp_err_t post_handler(httpd_req_t *req);
static esp_err_t stream_handler(httpd_req_t *req);
static esp_err_t not_found_handler(httpd_req_t *req);

/*============< PRIVATE DATA DEFINITION >========================================================*/

/* Struct to hold joy position information */
static ST_JOY_POSITION_T stt_httpd_rec_joy_pos;

/*============< FUNCTION DEFINITION >============================================================*/

/**
 * @brief       To Handle HTTP GET request by sending web page html
 * @param[in]   req HTTP Request data structure
 * @return      ESP_OK     Operation successfully
 *              ESP_FAIL   Operation fail
 */
static esp_err_t index_get_handler(httpd_req_t *req) 
{
    esp_err_t ent_ret;                                      /**< Function status */
    size_t szt_cont_len;                                    /**< Content length */
    
    /*====< INPUT >==========================================================*/

    szt_cont_len = u1s_index_html_end - u1s_index_html_start;

    /*====< OPERATION >======================================================*/

    log_i("got index request");
    (void)httpd_resp_set_type(req, "text/html");
    ent_ret = httpd_resp_send(req, (const char*)u1s_index_html_start, szt_cont_len);

    if (ent_ret != ESP_OK)
    {
        log_e("HTTPD send response fail with code :%d", ent_ret);
    }
    else
    {
        /* Do nothing */
    }

    /*====< OUTPUT >=========================================================*/

    return ent_ret;
}

/**
 * @brief       To Handle HTTP GET request by sending Javascript
 * @param[in]   HTTP Request Data Structure
 * @return      ESP_OK     Operation successfully
 *              ESP_FAIL   Operation fail
 */
static esp_err_t script_get_handler(httpd_req_t *req) 
{
    esp_err_t ent_ret;                                      /**< Function status */
    size_t szt_cont_len;                                    /**< Content length */

    /*====< INPUT >==========================================================*/

    szt_cont_len = u1s_script_js_end - u1s_script_js_start;

    /*====< OPERATION >======================================================*/

    log_i("got js request");
    (void)httpd_resp_set_type(req, "application/javascript");
    ent_ret = httpd_resp_send(req, (const char*)u1s_script_js_start, szt_cont_len);

    if (ent_ret != ESP_OK)
    {
        log_e("HTTPD send response fail with code :%d", ent_ret);
    }
    else
    {
        /* Do nothing */
    }

    /*====< OUTPUT >=========================================================*/

    return ent_ret;
}

/**
 * @brief       To Handle HTTP POST by recieve and extract data
 * @param[in]   req HTTP Request data structure
 * @return      ESP_OK     Operation successfully
 *              ESP_FAIL   Operation fail
 */
static esp_err_t post_handler(httpd_req_t *req)
{
    esp_err_t ent_ret;                                      /**< Function status */
    int s2t_http_req_ret;                                   /**< Data length or error code */
    char cht_cont_buff[POST_HANDLE_BUFF_LEN];               /**< HTTP request content buffer */
    cJSON *cjsont_content = NULL;                           /**< JSON structure of HTTP content */
    cJSON *cjsont_x1 = NULL;                                /**< JSON structure of item x1 */
    cJSON *cjsont_y1 = NULL;                                /**< JSON structure of item y1 */
    cJSON *cjsont_x2 = NULL;                                /**< JSON structure of item x2 */
    cJSON *cjsont_y2 = NULL;                                /**< JSON structure of item y2 */
    size_t szt_cont_len;                                    /**< HTTP content length */

    /*====< INPUT >==========================================================*/

    szt_cont_len = req->content_len;

    /*====< OPERATION >======================================================*/

    if (szt_cont_len >= POST_HANDLE_BUFF_LEN)
    {
        (void)httpd_resp_send_err(req, HTTPD_431_REQ_HDR_FIELDS_TOO_LARGE, NULL);
        log_e("Error content too long");
        ent_ret = ESP_FAIL;
    }
    else
    {
        s2t_http_req_ret = httpd_req_recv(req, cht_cont_buff, szt_cont_len);
        if (s2t_http_req_ret <= COMMON_VAL_0) 
        {
            (void)httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, NULL);
            log_e("HTTPD recieve fail with code: %d", s2t_http_req_ret);
            ent_ret = ESP_FAIL;
        }
        else
        {
            /* Add terminator character to content string */
            cht_cont_buff[szt_cont_len] = NULL_CHAR;

            /* Create JSON object from string */
            cjsont_content = cJSON_Parse(cht_cont_buff);
            if (cjsont_content == NULL) 
            {
                log_e("Failed to parse JSON");
                (void)httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid JSON");
                ent_ret = ESP_FAIL;
            }
            else
            {
                /* Extract JSON structure */
                cjsont_x1 = cJSON_GetObjectItem(cjsont_content, "x1");
                cjsont_y1 = cJSON_GetObjectItem(cjsont_content, "y1");
                cjsont_x2 = cJSON_GetObjectItem(cjsont_content, "x2");
                cjsont_y2 = cJSON_GetObjectItem(cjsont_content, "y2");

                /* Validate the value */
                if ((cJSON_IsNumber(cjsont_x1) && (cjsont_x1 != NULL)) 
                    && (cJSON_IsNumber(cjsont_y1) && (cjsont_y1 != NULL))
                    && (cJSON_IsNumber(cjsont_x2) && (cjsont_x2 != NULL))
                    && (cJSON_IsNumber(cjsont_y2) && (cjsont_y2 != NULL)))
                {
                    /* Save joy position to program structure */
                    stt_httpd_rec_joy_pos.s1_x1 = LIMIT(cjsont_x1->valueint, MIN_JOY_VAL, MAX_JOY_VAL);
                    stt_httpd_rec_joy_pos.s1_y1 = LIMIT(cjsont_y1->valueint, MIN_JOY_VAL, MAX_JOY_VAL);
                    stt_httpd_rec_joy_pos.s1_x2 = LIMIT(cjsont_x2->valueint, MIN_JOY_VAL, MAX_JOY_VAL);
                    stt_httpd_rec_joy_pos.s1_y2 = LIMIT(cjsont_y2->valueint, MIN_JOY_VAL, MAX_JOY_VAL);

                    httpd_resp_sendstr(req, "JSON Received and Parsed");

                    log_i("User joy position x1:%d y1:%d x2:%d y2:%d", 
                        stt_httpd_rec_joy_pos.s1_x1,
                        stt_httpd_rec_joy_pos.s1_y1,
                        stt_httpd_rec_joy_pos.s1_x2,
                        stt_httpd_rec_joy_pos.s1_y2);

                    ent_ret = ESP_OK;
                }
                else
                {
                    (void)httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid JSON");
                    log_e("JSON object not found");
                    ent_ret = ESP_FAIL;
                }

                /* Free up memory */
                cJSON_Delete(cjsont_content);
            }
        }
    }

    /*====< OUTPUT >=========================================================*/

    return ent_ret;
}

/**
 * @brief       To Handle HTTP GET by sent camera frame
 * @param[in]   req HTTP Request data structure
 * @return      ESP_OK     Operation successfully
 *              ESP_FAIL   Operation fail
 */
static esp_err_t stream_handler(httpd_req_t *req)
{
    esp_err_t ent_res = ESP_OK;                             /**< Return function status */
    camera_fb_t *pst_cam_fb = NULL;                         /**< Pointer to structure of frame buffer */
    uint8_t *pu1t_jpg_buf = NULL;                           /**< Pointer to JPEG buffer */
    size_t szt_jpg_buf_len = COMMON_VAL_0;                  /**< Length of JPEG buffer */
    bool b1t_jpeg_conver_sts;                               /**< Status of JPEG conversion is OK */
    size_t szt_rep_head_len;                                /**< Length of HTTP representation headers */
    char cht_resp_header[HTTP_REPS_HEADER_BUFF_LEN];        /**< http respond header */
    struct timeval stt_frame_timestamp;                     /**< Timestamp of current frame */
    int64_t s8t_curr_frm_us;                                /**< Current time in uS */
    int64_t s8t_frm_time_us;                                /**< Time used for current frame */
    static int64_t s8s_last_frm_us = COMMON_VAL_0;          /**< Time in uS since the last frame is sent */
    uint32_t u4t_frame_rate_fps;                            /**< Frame rate (Frame per second) */

    /*====< INPUT >==========================================================*/
    /* None */

    /*====< OPERATION >======================================================*/

    /* HTTP header for stream content */
    (void)httpd_resp_set_type(req, STREAM_CONTENT_TYPE);
    (void)httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    (void)httpd_resp_set_hdr(req, "X-Framerate", "60");

    while (ent_res == ESP_OK)
    {
        /* Get pointer to camera frame buffer */
        pst_cam_fb = esp_camera_fb_get();

        if (pst_cam_fb == NULL)
        {
            log_e("Camera capture failed");
            ent_res = ESP_FAIL;
            break;
        }
        else
        {
            stt_frame_timestamp.tv_sec = pst_cam_fb->timestamp.tv_sec;
            stt_frame_timestamp.tv_usec = pst_cam_fb->timestamp.tv_usec;
            
            if (pst_cam_fb->format != PIXFORMAT_JPEG)
            {
                /* Convert frame to JPEG */
                b1t_jpeg_conver_sts = frame2jpg(pst_cam_fb, JPEG_CONVERT_QUALITY, 
                                                &pu1t_jpg_buf, &szt_jpg_buf_len);
                esp_camera_fb_return(pst_cam_fb);
                pst_cam_fb = NULL;
                if (b1t_jpeg_conver_sts == true)
                {
                    ent_res = ESP_OK;
                }
                else
                {
                    log_e("JPEG compression failed");
                    ent_res = ESP_FAIL;
                }
            }
            else
            {
                szt_jpg_buf_len = pst_cam_fb->len;
                pu1t_jpg_buf = pst_cam_fb->buf;
                ent_res = ESP_OK;
            }
        }

        if (ent_res == ESP_OK)
        {
            /* Sent HTTP boundary */
            ent_res = httpd_resp_send_chunk(req, STREAM_BOUNDARY, strlen(STREAM_BOUNDARY));
        }
        else
        {
            log_e("HTTPD send stream boudary fail with code :%d", ent_res);
        }
        
        if (ent_res == ESP_OK)
        {
            /* Sent HTTP representation headers */
            szt_rep_head_len = snprintf(cht_resp_header, HTTP_REPS_HEADER_BUFF_LEN, STREAM_PART, 
                                    szt_jpg_buf_len, stt_frame_timestamp.tv_sec, stt_frame_timestamp.tv_usec);

            ent_res = httpd_resp_send_chunk(req, (const char *)cht_resp_header, szt_rep_head_len);
        }
        else
        {
            log_e("HTTPD send stream header fail with code :%d", ent_res);
        }

        if (ent_res == ESP_OK)
        {
            /* Sent HTTP body */
            ent_res = httpd_resp_send_chunk(req, (const char *)pu1t_jpg_buf, szt_jpg_buf_len);
        }
        else
        {
            log_e("HTTPD send stream body fail with code :%d", ent_res);
        }

        if (pst_cam_fb)
        {
            /* Return the frame buffer to be reused again */
            esp_camera_fb_return(pst_cam_fb);
            pst_cam_fb = NULL;
            pu1t_jpg_buf = NULL;
        }
        else if (pu1t_jpg_buf)
        {
            /* Delete JPEG buffer to free memory */
            free(pu1t_jpg_buf);
            pu1t_jpg_buf = NULL;
        }
        else
        {
            /* do nothing */
        }

        /* Calculate time used in each frame */
        s8t_curr_frm_us = esp_timer_get_time();
        s8t_frm_time_us = s8t_curr_frm_us - s8s_last_frm_us;
        s8s_last_frm_us = s8t_curr_frm_us;

        if (s8t_frm_time_us != 0)
        {
            u4t_frame_rate_fps = (uint32_t)(1000000.0 / (float)s8t_frm_time_us);
        }
        else
        {
            /* Avoid divide by zeros */
            u4t_frame_rate_fps = COMMON_VAL_0;
        }

        log_i("MJPG: %uB %dfps", 
            (uint32_t)szt_jpg_buf_len, u4t_frame_rate_fps);
    }

    /*====< OUTPUT >=========================================================*/

    return ESP_FAIL;
}

/**
 * @brief       To Handle HTTP GET by sent camera frame
 * @param[in]   req HTTP Request data structure
 * @return      ESP_OK     Operation successfully
 *              ESP_FAIL   Operation fail
 */
static esp_err_t not_found_handler(httpd_req_t *req)
{
    esp_err_t ent_ret;                                      /**< Function status */

    /*====< INPUT >==========================================================*/
    /* None */

    /*====< OPERATION >======================================================*/

    ent_ret = httpd_resp_send_404(req);
    log_e("404 Request content not found: %s\n", req->uri);

    if (ent_ret != ESP_OK)
    {
        /* Error handle */
        log_e("HTTPD send 404 fail with code :%d", ent_ret);
    }
    else
    {
        /* Do nothing */
    }

    /*====< OUTPUT >=========================================================*/

    return ent_ret;
}

/**
 * @brief       To Handle HTTP GET by sent camera frame
 * @param[in]   HTTP Request Data Structure
 * @return      ESP_OK     Operation successfully
 *              ESP_FAIL   Operation fail
 */
esp_err_t start_camera_server(void)
{
    esp_err_t ent_ret = ESP_OK;                                     /**< Function status */
    httpd_handle_t index_httpd_hnd = NULL;                          /**< Pointer to index server handle instance */
    httpd_handle_t stream_httpd_hnd = NULL;                         /**< Pointer to stream server handle instance */
    httpd_config_t index_httpd_config = HTTPD_DEFAULT_CONFIG();     /**< Index server configuration */
    httpd_config_t stream_httpd_config = HTTPD_DEFAULT_CONFIG();    /**< Stream server configuration */
    esp_err_t ent_index_httpd_strat_ret;                            /**< Index server start status */
    esp_err_t ent_stream_httpd_strat_ret;                           /**< Stream server start status */

    /*====< INPUT >==========================================================*/

    index_httpd_config.server_port = INDEX_HTTPD_SERVER_PORT;
    index_httpd_config.ctrl_port = INDEX_HTTPD_CTRL_PORT;
    index_httpd_config.task_priority = INDEX_HTTPD_TASK_PRIORITY;
    stream_httpd_config.server_port = STREAM_HTTPD_SERVER_PORT;
    stream_httpd_config.ctrl_port = STREAM_HTTPD_CTRL_PORT;
    stream_httpd_config.task_priority = STREAM_HTTPD_TASK_PRIORITY;

    /*====< OPERATION >======================================================*/

    /* Create URI handler */
    httpd_uri_t index_uri = 
    {
        .uri = "/",
        .method = HTTP_GET,
        .handler = index_get_handler,
        .user_ctx = NULL,
        .is_websocket = true,
        .handle_ws_control_frames = false,
        .supported_subprotocol = NULL
    };

    httpd_uri_t script_uri = 
    {
        .uri = "/joy.js",
        .method = HTTP_GET,
        .handler = script_get_handler,
        .user_ctx = NULL,
        .is_websocket = true,
        .handle_ws_control_frames = false,
        .supported_subprotocol = NULL
    };

    httpd_uri_t remote_cmd = 
    {
        .uri = "/cmd",
        .method = HTTP_POST,
        .handler = post_handler,
        .user_ctx = NULL,
        .is_websocket = false,
        .handle_ws_control_frames = false,
        .supported_subprotocol = NULL
    };

    httpd_uri_t stream_uri = 
    {
        .uri = "/stream",
        .method = HTTP_GET,
        .handler = stream_handler,
        .user_ctx = NULL,
        .is_websocket = true,
        .handle_ws_control_frames = false,
        .supported_subprotocol = NULL
    };

    httpd_uri_t fallback_uri = 
    {
        .uri      = "/*",
        .method   = HTTP_GET,
        .handler  = not_found_handler,
        .user_ctx = NULL,
        .is_websocket = true,
        .handle_ws_control_frames = false,
        .supported_subprotocol = NULL
    };

    /* Start HTTP server */
    log_i("Starting web server on port: '%d'", index_httpd_config.server_port);
    ent_index_httpd_strat_ret = httpd_start(&index_httpd_hnd, &index_httpd_config);
    if (ent_index_httpd_strat_ret == ESP_OK)
    {
        (void)httpd_register_uri_handler(index_httpd_hnd, &index_uri);
        (void)httpd_register_uri_handler(index_httpd_hnd, &script_uri);
        (void)httpd_register_uri_handler(index_httpd_hnd, &remote_cmd);
        (void)httpd_register_uri_handler(index_httpd_hnd, &fallback_uri);
    }
    else
    {
        ent_ret = ESP_FAIL;
    }

    /* Start HTTP server */
    log_i("Starting stream on port: '%d'", stream_httpd_config.server_port);
    ent_stream_httpd_strat_ret = httpd_start(&stream_httpd_hnd, &stream_httpd_config);
    if (ent_stream_httpd_strat_ret == ESP_OK)
    {
        (void)httpd_register_uri_handler(stream_httpd_hnd, &stream_uri);
    }
    else
    {
        ent_ret = ESP_FAIL;
    }

    /*====< OUTPUT >=========================================================*/

    return ent_ret;
}
