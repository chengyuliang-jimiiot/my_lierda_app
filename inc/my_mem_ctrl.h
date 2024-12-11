/**
 * @File Name: my_mem_ctrl.h
 * @brief  
 * @Author : chengyuliang email:chengyuliang@jimiiot.com
 * @Version : 1.0
 * @Update : 2024-12-11
 **
 */

#ifndef MY_MEM_CTRL_H
#define MY_MEM_CTRL_H

/* function declaration */
void my_free_buffer(char *buffer);
void* my_create_buffer(size_t buffer_size);

#endif