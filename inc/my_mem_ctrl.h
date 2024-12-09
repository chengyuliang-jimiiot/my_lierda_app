/**
 * @File Name: my_mem_ctrl.h
 * @brief  
 * @Author : chengyuliang email:chengyuliang@jimiiot.com
 * @Version : 1.0
 * @Update : 2024-12-06
 **
 */

#ifndef MY_MEM_CTRL_H
#define MY_MEM_CTRL_H

/*宏定义*/
#define MY_BUFFER_SIZE 64

/*函数声明*/
void my_free_buffer(char *buffer);
char* my_create_buffer(void);

#endif