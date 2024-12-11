/**
 * @File Name: my_network.c
 * @brief  
 * @Author : chengyuliang email:chengyuliang@jimiiot.com
 * @Version : 1.0
 * @Update : 2024-12-11
 **
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* My header */
#include "my_log.h"
#include "my_os_ctrl.h"
#include "my_network.h"

/* 网络初始化函数
 * **************
 * nSim : sim卡槽
 * cid ：连接标识符
 * ip_version ： IP协议版本
 * apn_name ： 接入点名称
 * username
 * password
 * auth_type ：身份验证类型（APN鉴权方式）
 * ***************
*/
bool my_network_init(UINT8 nSim, INT32 cid, INT32 ip_version, CHAR *apn_name, CHAR *username, CHAR *password, INT32 auth_type)
{
    
#ifdef _LIOT_SOCKETS_H_
    liot_trace("my_network_init start");
    liot_data_call_info_t info;
    int times = 0;
    int ret = 0;

    /* 等待网络注册成功，每次等待120秒，最多等待10次 */
    while (LIOT_DATACALL_SUCCESS != (ret = liot_network_register_wait(nSim, 120)) && times < 10)
    {
        liot_trace("network_register_wait");
        times++;
        liot_rtos_task_sleep_s(1);
    }
    if (LIOT_DATACALL_SUCCESS != ret)
    {
        liot_trace("====network register failure!!!!!====");
        if (LIOT_OSI_SUCCESS != liot_rtos_task_delete(NULL))
        {
            liot_trace("task deleted failed");
        }
        return  false;
    }

    /* 启动数据拨号 */
    ret = liot_start_data_call(nSim, cid, ip_version, apn_name, username, password, auth_type);
    if (ret != 0)
    {
        liot_trace("====data call failure!!!!=====");
    }

    /* 获取数据拨号信息 */
    ret = liot_get_data_call_info(nSim, cid, &info);
    if (ret != 0)
    {
        liot_trace("liot_get_data_call_info ret: %d", ret);
        liot_stop_data_call(nSim, cid);
        return false;
    }
    liot_trace("info->profile_idx: %d", info.cid);
    liot_trace("info->ip_version: %d", info.ip_version);
    liot_trace("info->v4.state: %d", info.v4.state);
    liot_ip4addr_ntoa(&info.v4.addr.ip);
    liot_trace("info.v4.addr.ip: %s", liot_ip4addr_ntoa(&info.v4.addr.ip));

    liot_ip4addr_ntoa(&info.v4.addr.pri_dns);
    liot_trace("info.v4.addr.pri_dns: %s", liot_ip4addr_ntoa(&info.v4.addr.pri_dns));

    liot_ip4addr_ntoa(&info.v4.addr.sec_dns);
    liot_trace("info.v4.addr.sec_dns: %s", liot_ip4addr_ntoa(&info.v4.addr.sec_dns));

    return true;

#endif // Lierda
}
