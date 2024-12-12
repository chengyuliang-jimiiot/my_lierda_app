/**
 * @File Name: my_network_main.h
 * @brief  
 * @Author : chengyuliang email:chengyuliang@jimiiot.com
 * @Version : 1.0
 * @Update : 2024-12-12
 **
 */

#ifndef MY_NETWORK_MAIN_H
#define MY_NETWORK_MAIN_H

#ifndef MY_OS_CTRL_H
#include "my_os_ctrl.h"
#endif //MY_OS_CTRL_H

extern liot_task_t g_http_taskRef;
extern liot_task_t g_network_main_taskRef;
extern int cid;
extern uint8_t nSim;

#endif //MY_NETWORK_MAIN_H