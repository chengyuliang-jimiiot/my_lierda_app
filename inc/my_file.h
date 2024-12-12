/**
 * @File Name: my_file.h
 * @brief  
 * @Author : chengyuliang email:chengyuliang@jimiiot.com
 * @Version : 1.0
 * @Update : 2024-12-12
 **
 */

#ifndef MY_FILE_H
#define MY_FILE_H

/* Lierda Header */
#include "liot_fs_api.h"
bool my_save_file(const char *file_name, void *buffer, size_t data_length);
void my_open_file(const char *file_name, void *read_buffer);
#endif // !MY_FILE_H