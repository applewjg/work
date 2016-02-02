/***************************************************************************
 * 
 * Copyright (c) 2015 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file main.cpp
 * @author wangjingui(com@baidu.com)
 * @date 2015/10/09 14:37:55
 * @brief 
 *  
 **/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "ConfManager.h"

/* version */
#ifdef __VERSION_ID__
    static const char *version_id = __VERSION_ID__;
#else
    static const char *version_id = "unknown";
#endif

/* date of building */
#if defined(__DATE__) && defined(__TIME__)
    static const char server_built[] = __DATE__ " " __TIME__;
#else
    static const char server_built[] = "unknown";
#endif


confmgr::ConfManager g_conf_manager;

void get_version(void)
{
    printf(" Version\t: %s\n", version_id);
    printf(" Built date\t: %s\n", server_built);
}

//extern int Main(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    for (unsigned int i = 0; i < argc; ++i)
    {
        if(0 == strcmp("-v",argv[i]))
        {
            get_version();
            return 0;
        }
    }
    const char * confpath = "./conf/";  // 配置目录
    const char * confname = "bc.conf";  // 配置文件
    const char * rangename = NULL;  // 配置约束文件，默认为空
    const char * dictconf = "bc_dicts.conf";  // 词典配置文件
    //静态配置初始化
    if (g_conf_manager.reload(confpath, confname, rangename) != 0) 
    {
        fprintf(stderr, "Read configure file error! BC Init failed.\n");
        return -1;
    }
    return 0;
}




















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
