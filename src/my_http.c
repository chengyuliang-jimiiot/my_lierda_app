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
 * �����¼��ص�������
 *      client��[Out] http�ͻ��˾����
 *      evt��[In] http�����¼����͡����������ṩ��˵���ĵ����ܴ�������,�������Ͳ����Ǻ�ƥ��
 *          typedef enum
            {
                LIOT_HTTP_EVENT_SESSION_ESTABLISH  = 0,HTTP�������ӽ����ɹ�
                LIOT_HTTP_EVENT_RESPONE_STATE_LINE = 1,HTTP����ʼ��Ӧ
                LIOT_HTTP_EVENT_SESSION_DISCONNECT = 2,HTTP�������ӶϿ�����
            } liot_httpc_event_id_e;

 *      evt_code:[In] http�����¼�������͡����������ṩ��˵���ĵ����ܴ������⣬�������Ͳ����Ǻ�ƥ��
            typedef enum
            {
                LIOT_HTTPC_SESSION_OPEN      = 1,HTTP�ͻ������ӽ����ɹ��¼�״̬
                LIOT_HTTPC_UPLOAD_START      = 2,HTTP�ͻ��˿�ʼ�ϴ��ļ��¼�״̬���ݲ�֧��
                LIOT_HTTPC_UPLOAD_END        = 3,HTTP�ͻ�������ϴ��ļ��¼����ݲ�֧��
                LIOT_HTTPC_RESPONSE_STATUS   = 4,HTTP�ͻ��˿�ʼ������Ӧ�����¼�
                LIOT_HTTPC_RESPONSE_COMPLETE = 5,HTTP�ͻ�����ɽ�����Ӧ�����¼�
                LIOT_HTTPC_RESPONSE_TIMEOUT  = 6,HTTP�ͻ��˽�����Ӧ���ݳ�ʱ�¼�
                LIOT_HTTPC_SESSION_CLOSE     = 7,HTTP�ͻ�����·�ѹر��¼�
                LIOT_HTTPC_RAW_REQ_START     = 8,HTTP�ͻ��˿�ʼ�ϴ�RAW�ļ��¼����ݲ�֧��
                LIOT_HTTPC_RAW_REQ_END       = 9,HTTP�ͻ����ϴ�RAW�ļ�������¼����ݲ�֧��
            } liot_httpc_event_type_e;
 *      arg��[In] http�����¼��ص��������Ĳ���
*/
static void http_event_cb(liot_http_client_t *client, int evt, int evt_code, void *arg)
{
    liot_trace("===http_event_cb===  evt:%d,evt_code:%d,%p", evt, evt_code, client);
    switch (evt)//�����¼����ͷ�֧
    {
        case LIOT_HTTPC_SESSION_OPEN://HTTP�ͻ������ӽ����ɹ�
        {
            liot_trace("LIOT_HTTPC_SESSION_OPEN");
            if (evt_code != LIOT_HTTPC_SUCCESS)
            {
                liot_trace("http session open ERROR");
                liot_rtos_semaphore_release(http_semp);
            }
        }
        break;

        case LIOT_HTTPC_UPLOAD_START://HTTP�ͻ��˿�ʼ�ϴ��ļ�
        {
            liot_trace("LIOT_HTTPC_UPLOAD_START");
        }
        break;

        case LIOT_HTTPC_UPLOAD_END://HTTP�ͻ�������ϴ��ļ�
        {
            liot_trace("LIOT_HTTPC_UPLOAD_END");
        }
        break;

        case LIOT_HTTPC_RESPONSE_STATUS://HTTP�ͻ��˿�ʼ������Ӧ����
        {
            liot_trace("LIOT_HTTPC_RESPONSE_STATUS");

            int resp_code      = 0;
            int content_length = 0;
            int chunk_encode   = 0;
            char *location     = NULL;
            char *date         = NULL;

            /* ��ȡHTTP������Ӧͷ�������Ϣ */
            liot_httpc_getinfo(client, LIOT_HTTPC_STATUS_CODE, &resp_code); //HTTP������Ӧ��
            liot_httpc_getinfo(client, LIOT_HTTPC_CHUNK_ENCODE, &chunk_encode); //��ȡ��Ӧ�������Ƿ�Ϊ chunk �����ʽ,0��1��
            liot_httpc_getinfo(client, LIOT_HTTPC_DATE, &date); //��ȡʱ��

            if (date != NULL)
            {
                liot_trace("===http_event_cb=== Date:%s", date);
                liot_rtos_free(date);
            }

            liot_trace("===http_event_cb=== resp_code:%d,chunk_encode:%d", resp_code, chunk_encode);
            if (chunk_encode == 0)//�����chunk �����ʽ
            {
                liot_httpc_getinfo(client, LIOT_HTTPC_CONTENT_LEN, &content_length); //��ȡ���ݳ���
                liot_trace("===http_event_cb=== content_length:%d", content_length);
            }
            else
            {
                liot_trace("http chunk encode");
            }

            if (resp_code >= 300 && resp_code < 400)
            {
                liot_httpc_getinfo(client, LIOT_HTTPC_LOCATION, &location); //��ȡ�ض���� URL����ֵ������Ӧ��Ϊ 3xx ʱ���ɻ�ȡ��
                liot_trace("===http_event_cb=== location:%s", location);
                liot_rtos_free(location);
            }
        }
        break;

        case LIOT_HTTPC_RESPONSE_COMPLETE://HTTP�ͻ�����ɽ�����Ӧ����
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

        case LIOT_HTTPC_RESPONSE_TIMEOUT://HTTP�ͻ��˽�����Ӧ���ݳ�ʱ
        {
            liot_trace("LIOT_HTTPC_RESPONSE_TIMEOUT");
            liot_rtos_semaphore_release(http_semp);
        }
        break;

        case LIOT_HTTPC_SESSION_CLOSE://HTTP�ͻ�����·�ѹر�
        {
            liot_trace("LIOT_HTTPC_SESSION_CLOSE");
            liot_rtos_semaphore_release(http_semp_end);
        }
        break;

    }

}


/*
 *���� HTTP ��Ӧ���ݵĻص�����������http_client_write_data_cb_t
 *      client��    [In] HTTP�ͻ��˾����
 *      arg��    [In] HTTP�ص��������Ĳ�����
 *      data��    [In] HTTP������Ӧ�����ݡ�
 *      size��    [In] HTTP������Ӧ���ݵĳ��ȡ�
 *      end��       [In] HTTP������Ӧ��������
 *          false:���ݽ���δ������
            true:���ݽ�����ϡ�
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
    liot_http_client_t http_client = 0; //�ͻ��˾��

    liot_rtos_semaphore_create(&http_semp, 0);
    liot_rtos_semaphore_create(&http_semp_end, 0);

    /* ����һ���µ� HTTP �ͻ��˾������ʼ�� HTTP �ͻ�����Դ */
    if (liot_httpc_new(&http_client, http_event_cb, NULL) != LIOT_HTTPC_SUCCESS)//����ֵ���ͣ�liot_httpc_result_code_e
    {
        liot_trace("http client create failed!!!!");
        if (LIOT_OSI_SUCCESS != liot_rtos_task_delete(NULL))
        {
            liot_trace("task deleted failed");
        }
    }

    /* url�ṹ���ʼ�� */
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

    /* ����url */
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

    /* ����HTTP������� */
    liot_httpc_setopt(&http_client, LIOT_HTTP_CLIENT_OPT_SIM_ID, nSim);//ִ�����ݲ��Ų���ʱʹ�õ�(U)SIM ��
    liot_httpc_setopt(&http_client, LIOT_HTTP_CLIENT_OPT_PDPCID, cid);//���� HTTP �ͻ���ʹ�õ�����ͨ���ţ���ִ�����ݲ��Ų���ʱʹ�õ�PDP������ID��
    liot_httpc_setopt(&http_client, LIOT_HTTP_CLIENT_OPT_METHOD, http_method);//���� HTTP ����ʽ
    liot_httpc_setopt(&http_client, LIOT_HTTP_CLIENT_OPT_URL, url);//����HTTP�����URL
    liot_httpc_setopt(&http_client, LIOT_HTTP_CLIENT_OPT_WRITE_FUNC, http_response_write_data_cb);//���ô��� HTTP ��Ӧ���ݵĻص��������������ͣ�http_client_write_data_cb_t

    /* ���� HTTP ���� */
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
