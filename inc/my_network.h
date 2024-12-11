/**
 * @File Name: my_network.h
 * @brief  
 * @Author : chengyuliang email:chengyuliang@jimiiot.com
 * @Version : 1.0
 * @Update : 2024-12-11
 **
 */

#ifndef MY_NETWORK_H
#define MY_NETWORK_H

/* My header */
#ifndef MY_TYPE_H
#include "my_type.h"
#endif

/* Lierda Header */
#include "liot_datacall.h"
#include "liot_fs_api.h"
#include "liot_http.h"
#include "liot_nw.h"
#include "liot_sockets.h"

/* function declaration */
bool my_network_init(UINT8 nSim, INT32 cid, INT32 ip_version, CHAR *apn_name, CHAR *username, CHAR *password, INT32 auth_type);

#endif