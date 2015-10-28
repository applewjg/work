/***************************************************************************
 * 
 * Copyright (c) 2015 Baidu.com, Inc. All Rights Reserved
 * 
 **************************************************************************/
 
 
 
/**
 * @file server.2.cpp
 * @author wangjingui(com@baidu.com)
 * @date 2015/09/07 22:14:53
 * @brief 
 *  
 **/

#include <stdio.h>
#include <stdlib.h>
#include "hb_server.h"

heartbeat_server _hbserver;
hb_server_conf_t _hb_svr_conf;



typedef enum {
    ONLINELOG_CHN_SCHED = 0,
    ONLINELOG_CHN_HBCLIENT,
    ONLINELOG_CHN_HBSERVER,
    ONLINELOG_TYPE_MAX
}onlinelog_chn_t;

//#define MAX_SELF_DEF_LOG 8        /**< 单个线中自定义日志的日志数上限      */
#define ONLINELOG_MAX_FILE_NAME     512

int log_channel[MAX_SELF_DEF_LOG] = {-1, -1, -1, -1, -1, -1, -1, -1};
char log_file[MAX_SELF_DEF_LOG][ONLINELOG_MAX_FILE_NAME] = {"1","2","3","4","5","6","7","8"};

ul_logstat_t log_stat;
Ul_log_self log_self;

static Ul_log_self* get_log_self() { return &log_self; }
static ul_logstat_t* get_log_stat() { return &log_stat; }

int get_loghandle(int chn)
{
    if (chn < MAX_SELF_DEF_LOG && chn >= 0) {
        return log_channel[chn];
    } else {
        return UL_LOG_WARNING;
    }
}


static void hb_unavailable(void *key)
{
    if (NULL == key)
    {
        return;
    }

    int inst_id = *(int *)key;

    printf("unavailable: inst = %d\n", inst_id);

    return;
}

static void hb_available(void *key)
{
    if (NULL == key)
    {
        return;
    }

    int inst_id = *(int *)key;
    printf("available: inst = %d\n", inst_id);

    return;
}


static void hb_report(void *key, char *msg, int msg_size)
{
    if (key != key || msg != msg || msg_size != msg_size)
    {
        return;
    }
}




int main() {

    for (int k = 0; k < 8; k++)
    {
        // 初始化日志名
        snprintf(log_self.name[k], 256, "self");
        // 是否打开自定义日志。如果设置为0，则不打印对应的日志
        log_self.flags[k] = 1;
        log_channel[k] = UL_LOG_SELF_BEGIN + k;
    }

    // 初始化其他日志资源
    log_stat.events =  UL_LOG_ALL;
    log_stat.to_syslog = UL_LOG_NONE;
    log_stat.spec = 1;
    log_self.log_number = 8;

    // 主线程打开日志
    ul_openlog("", "log/test.", &log_stat, 16, &log_self);


    int log_hb_id = get_loghandle(ONLINELOG_CHN_HBSERVER);
    _hb_svr_conf.log_stat = get_log_stat();
    _hb_svr_conf.log_self = get_log_self();
    _hb_svr_conf.log_self_main = log_hb_id;
    _hb_svr_conf.log_self_listen = log_hb_id;


    int inst_id = 100;
    _hb_svr_conf.client_tmo = 600;
    _hb_svr_conf.udp_listen_port = 4400;
    _hb_svr_conf.detect_interval = 5;
    _hbserver.client_unavailable = hb_unavailable;
    _hbserver.client_available = hb_available;
    _hbserver.client_report = hb_report;
    _hbserver.init(_hb_svr_conf);
    _hbserver.add_client("127.0.0.1", 4405, &inst_id, sizeof(inst_id));
    if (!_hbserver.start()) {
        printf("start heartbeat failed.\n");
    }
    _hbserver.add_client("127.0.0.1", 4406, &inst_id, sizeof(inst_id));
    _hbserver.add_client("127.0.0.1", 4407, &inst_id, sizeof(inst_id));
    printf("hello world\n");
    sleep(2000);
    return 0;
}





















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
