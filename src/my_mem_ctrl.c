/**
 * @File Name: my_mem_ctrl.c
 * @brief  
 * @Author : chengyuliang email:chengyuliang@jimiiot.com
 * @Version : 1.0
 * @Update : 2024-12-11
 **
 */

#include <stdio.h>
#include <string.h>
#include "stdlib.h"

/* My header */
#include "my_log.h"
#include "my_mem_ctrl.h"

void my_free_buffer(char *buffer)
{
    if (buffer != NULL)
    {
        free(buffer);
        buffer = NULL;
    }
}

void* my_create_buffer(size_t buffer_size)
{
    void *buffer = malloc(buffer_size);
    if (buffer == NULL) 
    {
        my_simple_str_log("heap allocation failure");
        return NULL;
    }
    memset(buffer, 0, buffer_size);
    return buffer;
}