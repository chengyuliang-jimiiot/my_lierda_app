/**
 * @File Name: my_mem_ctrl.c
 * @brief  
 * @Author : chengyuliang email:chengyuliang@jimiiot.com
 * @Version : 1.0
 * @Update : 2024-12-09
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

char* my_create_buffer(unsigned short size)
{
    char *buffer = (char *)malloc(size * sizeof(char));
    if (buffer == NULL) 
    {
        my_simple_str_log("heap allocation failure");
        return NULL;
    }
    memset(buffer, 0, size);
    return buffer;
}