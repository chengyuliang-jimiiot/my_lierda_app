/**
 * @File Name: my_http.h
 * @brief  
 * @Author : chengyuliang email:chengyuliang@jimiiot.com
 * @Version : 1.0
 * @Update : 2024-12-12
 **
 */

/* My header */
#include "my_http.h"
#include "my_os_ctrl.h"
#include "my_pdata.h"
#include "my_log.h"
#include "my_network_main.h"
#include "my_file.h"

static liot_sem_t http_semp;
static liot_sem_t http_semp_end;

/* 
 * 请求事件回调处理函数
 *      client：[Out] http客户端句柄。
 *      evt：[In] http请求事件类型。？利尔达提供的说明文档可能存在问题,参数类型并不是很匹配
 *          typedef enum
            {
                LIOT_HTTP_EVENT_SESSION_ESTABLISH  = 0,HTTP请求连接建立成功
                LIOT_HTTP_EVENT_RESPONE_STATE_LINE = 1,HTTP请求开始响应
                LIOT_HTTP_EVENT_SESSION_DISCONNECT = 2,HTTP请求连接断开连接
            } liot_httpc_event_id_e;

 *      evt_code:[In] http请求事件结果类型。？利尔达提供的说明文档可能存在问题，参数类型并不是很匹配
            typedef enum
            {
                LIOT_HTTPC_SESSION_OPEN      = 1,HTTP客户端连接建立成功事件状态
                LIOT_HTTPC_UPLOAD_START      = 2,HTTP客户端开始上传文件事件状态，暂不支持
                LIOT_HTTPC_UPLOAD_END        = 3,HTTP客户端完成上传文件事件，暂不支持
                LIOT_HTTPC_RESPONSE_STATUS   = 4,HTTP客户端开始接收响应数据事件
                LIOT_HTTPC_RESPONSE_COMPLETE = 5,HTTP客户端完成接收响应数据事件
                LIOT_HTTPC_RESPONSE_TIMEOUT  = 6,HTTP客户端接收响应数据超时事件
                LIOT_HTTPC_SESSION_CLOSE     = 7,HTTP客户端链路已关闭事件
                LIOT_HTTPC_RAW_REQ_START     = 8,HTTP客户端开始上传RAW文件事件，暂不支持
                LIOT_HTTPC_RAW_REQ_END       = 9,HTTP客户端上传RAW文件已完成事件，暂不支持
            } liot_httpc_event_type_e;
 *      arg：[In] http请求事件回调处理函数的参数
*/
static void http_event_cb(liot_http_client_t *client, int evt, int evt_code, void *arg)
{
    liot_trace("===http_event_cb===  evt:%d,evt_code:%d,%p", evt, evt_code, client);
    switch (evt)//请求事件类型分支
    {
        case LIOT_HTTPC_SESSION_OPEN://HTTP客户端连接建立成功
        {
            liot_trace("LIOT_HTTPC_SESSION_OPEN");
            if (evt_code != LIOT_HTTPC_SUCCESS)
            {
                liot_trace("http session open ERROR");
                liot_rtos_semaphore_release(http_semp);
            }
        }
        break;

        case LIOT_HTTPC_UPLOAD_START://HTTP客户端开始上传文件
        {
            liot_trace("LIOT_HTTPC_UPLOAD_START");
        }
        break;

        case LIOT_HTTPC_UPLOAD_END://HTTP客户端完成上传文件
        {
            liot_trace("LIOT_HTTPC_UPLOAD_END");
        }
        break;

        case LIOT_HTTPC_RESPONSE_STATUS://HTTP客户端开始接收响应数据
        {
            liot_trace("LIOT_HTTPC_RESPONSE_STATUS");

            int resp_code      = 0;
            int content_length = 0;
            int chunk_encode   = 0;
            char *location     = NULL;
            char *date         = NULL;

            /* 获取HTTP请求响应头部相关信息 */
            liot_httpc_getinfo(client, LIOT_HTTPC_STATUS_CODE, &resp_code); //HTTP请求响应码
            liot_httpc_getinfo(client, LIOT_HTTPC_CHUNK_ENCODE, &chunk_encode); //获取响应体数据是否为 chunk 编码格式,0否，1是
            liot_httpc_getinfo(client, LIOT_HTTPC_DATE, &date); //获取时间

            if (date != NULL)
            {
                liot_trace("===http_event_cb=== Date:%s", date);
                liot_rtos_free(date);
            }

            liot_trace("===http_event_cb=== resp_code:%d,chunk_encode:%d", resp_code, chunk_encode);
            if (chunk_encode == 0)//如果非chunk 编码格式
            {
                liot_httpc_getinfo(client, LIOT_HTTPC_CONTENT_LEN, &content_length); //获取内容长度
                liot_trace("===http_event_cb=== content_length:%d", content_length);
            }
            else
            {
                liot_trace("http chunk encode");
            }

            if (resp_code >= 300 && resp_code < 400)
            {
                liot_httpc_getinfo(client, LIOT_HTTPC_LOCATION, &location); //获取重定向的 URL。该值仅当响应码为 3xx 时方可获取。
                liot_trace("===http_event_cb=== location:%s", location);
                liot_rtos_free(location);
            }
        }
        break;

        case LIOT_HTTPC_RESPONSE_COMPLETE://HTTP客户端完成接收响应数据
        {
            if (evt_code == LIOT_HTTPC_SUCCESS)
            {
                liot_trace("http transfer success");
            }
            else
            {
                liot_trace("http transfer fail");
            }
            liot_trace("LIOT_HTTPC_RESPONSE_COMPLETE");
            liot_rtos_semaphore_release(http_semp);
        }
        break;

        case LIOT_HTTPC_RESPONSE_TIMEOUT://HTTP客户端接收响应数据超时
        {
            liot_trace("LIOT_HTTPC_RESPONSE_TIMEOUT");
            liot_rtos_semaphore_release(http_semp);
        }
        break;

        case LIOT_HTTPC_SESSION_CLOSE://HTTP客户端链路已关闭
        {
            liot_trace("LIOT_HTTPC_SESSION_CLOSE");
            liot_rtos_semaphore_release(http_semp_end);
        }
        break;

    }

}


/*
 *处理 HTTP 响应数据的回调函数，类型http_client_write_data_cb_t
 *      client：    [In] HTTP客户端句柄。
 *      arg：    [In] HTTP回调处理函数的参数。
 *      data：    [In] HTTP请求响应的数据。
 *      size：    [In] HTTP请求响应数据的长度。
 *      end：       [In] HTTP请求响应结束符。
 *          false:数据接收未结束。
            true:数据接收完毕。
*/
static int http_response_write_data_cb(liot_http_client_t *client, void *arg, char *data, int size, unsigned char end)
{
    liot_trace("http_response_write_data_cb");
    liot_trace(">>>>>client:%p", client);
    liot_trace("===http_response_write_data_cb=== size:%d", size);
    unsigned char *recv_buf = NULL;
    recv_buf = malloc(size + 1);
    if (recv_buf != NULL)
    {
        memset(recv_buf, 0, size + 1);
        memcpy(recv_buf, data, size);
        liot_trace("===http_response_write_data_cb=== recv_buf:%s", recv_buf);
        my_save_file(IP_LOC_INFO_FILE, recv_buf, size);
        liot_rtos_free(recv_buf);
        recv_buf = NULL;
    }
    return size;
}

void my_http_task_thread(void *arg)
{
#ifdef LIOT_HTTP_CLIENT_H 
    int http_method = LIOT_HTTPC_METHOD_GET; 
    liot_httpc_url_s *url = NULL; 
    liot_http_client_t http_client = 0; //客户端句柄

    liot_rtos_semaphore_create(&http_semp, 0);
    liot_rtos_semaphore_create(&http_semp_end, 0);

    /* 创建一个新的 HTTP 客户端句柄并初始化 HTTP 客户端资源 */
    if (liot_httpc_new(&http_client, http_event_cb, NULL) != LIOT_HTTPC_SUCCESS)//返回值类型：liot_httpc_result_code_e
    {
        liot_trace("http client create failed!!!!");
        if (LIOT_OSI_SUCCESS != liot_rtos_task_delete(NULL))
        {
            liot_trace("task deleted failed");
        }
    }

    /* url结构体初始化 */
    url = liot_rtos_malloc(sizeof(liot_httpc_url_s));
    if(NULL == url)
    {
        liot_trace("url init failed!!!!");
        if (LIOT_OSI_SUCCESS != liot_rtos_task_delete(NULL))
        {
            liot_trace("task deleted failed");
        }
    }

    liot_trace(">>>>>>http_client:%p,%p", &http_client, http_client);

    /* 解析url */
    if(http_method == LIOT_HTTPC_METHOD_GET)
    {
        char* httpUrl = get_pUrl();
        liot_trace("LIOT_HTTPC_METHOD_GET");

        if(liot_httpc_url_parse(httpUrl, url) == false)
        {
            liot_trace("liot_httpc_url_parse failed!!!!");
            liot_rtos_free(url);
            if (LIOT_OSI_SUCCESS != liot_rtos_task_delete(NULL))
            {
                liot_trace("task deleted failed");
            }
        }
        liot_trace(">>>>>>url:%s,port:%d,path:*hidden*,scheme:%d", url->host, url->port, url->scheme);
    }

    /* 设置HTTP相关属性 */
    liot_httpc_setopt(&http_client, LIOT_HTTP_CLIENT_OPT_SIM_ID, nSim);//执行数据拨号操作时使用的(U)SIM 卡
    liot_httpc_setopt(&http_client, LIOT_HTTP_CLIENT_OPT_PDPCID, cid);//设置 HTTP 客户端使用的数据通道号，即执行数据拨号操作时使用的PDP上下文ID。
    liot_httpc_setopt(&http_client, LIOT_HTTP_CLIENT_OPT_METHOD, http_method);//设置 HTTP 请求方式
    liot_httpc_setopt(&http_client, LIOT_HTTP_CLIENT_OPT_URL, url);//设置HTTP请求的URL
    liot_httpc_setopt(&http_client, LIOT_HTTP_CLIENT_OPT_WRITE_FUNC, http_response_write_data_cb);//设置处理 HTTP 响应数据的回调函数。参数类型：http_client_write_data_cb_t

    /* 发送 HTTP 请求 */
    if(liot_httpc_perform(&http_client) == LIOT_HTTPC_SUCCESS)
    {
        liot_trace("liot_httpc_perform success!");
        liot_trace("http_semp_wait");
        liot_rtos_semaphore_wait(http_semp, LIOT_WAIT_FOREVER);
    }
    else
    {
        liot_trace("http perform client fail");
    }
    liot_trace("liot_rtos_semaphore_wait!!!!");
    liot_httpc_stop(&http_client);
    liot_rtos_semaphore_wait(http_semp_end, LIOT_WAIT_FOREVER);
    liot_httpc_release(&http_client);

    liot_rtos_free(url);

    //Loop insertion point
    
    liot_rtos_semaphore_delete(http_semp);
    liot_rtos_semaphore_delete(http_semp_end);

    liot_trace("===http task end===");

    while (1)
    {
        liot_rtos_task_sleep_ms(5000);     
        liot_trace("my_http_task_thread_stack_space = %d", liot_rtos_task_get_stack_space(g_http_taskRef));
    }

    liot_trace("===my_http_task_thread===");
    liot_rtos_task_delete(NULL);
#endif // Lierda
}
