/**
 * @File Name: my_file.c
 * @brief  
 * @Author : chengyuliang email:chengyuliang@jimiiot.com
 * @Version : 1.0
 * @Update : 2024-12-12
 **
 */

#include "my_file.h"
#include "my_log.h"

#define FSEEK_LENGTH 0 //偏移的长度，正数向后偏移，复数向前偏移

bool my_save_file(const char *file_name, void *buffer, size_t data_length)
{
#ifdef LIERDA_FS_API_H
    int ret = -1;
    LFILE fd = -1;

    ret = liot_file_exist(file_name);
    liot_trace("check file exist. ret === (%d)", ret);
    if (ret == 0)
    {
        liot_trace("=== remove file. ret === (%d)", liot_remove(file_name));
        liot_trace("=== remove file, check file exist. ret(%d) ===", liot_file_exist(file_name));
    }

    liot_trace("=== create file ===");
    fd = liot_fopen(file_name, "wb+");
    if (fd < LIOT_FS_OK)
    {
        liot_trace("=== create file fail. fd === (%d)", fd);
    }

    liot_trace("=== write data to file ====");
    ret = liot_fwrite(buffer, 1, data_length, fd);
    if (ret != data_length)
    {
        liot_trace("=== write data fail. ret === (%d)", ret);
    }

    liot_trace("=== close file ===");
    ret = liot_fclose(fd);
    if (ret != LIOT_FS_OK)
    {
        liot_trace("=== close file fail. ret === (%d)", ret);
    }

    if (ret == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
#endif // Lierda
}

void my_open_file(const char *file_name, void *read_buffer)
{
#ifdef LIERDA_FS_API_H
    size_t data_length;
    int ret = -1;
    LFILE fd = -1;
    fd = liot_fopen(file_name, "r");
    if (fd < LIOT_FS_OK)
    {
        liot_trace("=== open file fail. fd === (%d)", fd);
    }

    /* 获取文件大小 */
    data_length = liot_fsize(fd);
    liot_trace("=== file size, ret === (%d)", data_length);
    /* 设置文件指针位置 */
    liot_fseek(fd, FSEEK_LENGTH, LIOT_SEEK_SET);
    
    /* 读取文件 */
    liot_trace("=== read data from file ====");
    ret = liot_fread(read_buffer, 1, data_length - FSEEK_LENGTH, fd);
    if (ret != (data_length - FSEEK_LENGTH))
    {
        liot_trace("=== read data fail. ret === (%d)", ret);
    }
    else
    {
        liot_trace("=== read data success === (%s)", read_buffer);
    }

#endif // Lierda
}