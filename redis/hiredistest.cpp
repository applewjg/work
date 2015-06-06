#include <stdio.h>  
#include <stdlib.h>  
#include <stddef.h>  
#include <stdarg.h>  
#include <string.h>  
#include <assert.h> 
#include <sys/time.h>
#include <string>
#include "../hiredis/hiredis.h"
#include "../utils/conf.h"
#include "../rapidjson/document.h"

int64_t get_current_time_milli() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int64_t val = tv.tv_sec;
    val = val * 1000 + tv.tv_usec / 1000;
    return val;
}

static long long usec(void) {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return (((long long)tv.tv_sec)*1000000)+tv.tv_usec;
}


int64_t chars_to_int64(const char* str, int64_t def) {
    char * end;
    long long int v = strtoll(str, &end, 0);
    if (*end != '\0') {
        return def;
    }
    return static_cast<int64_t>(v);
}

int64_t string_to_int64(const std::string& str, int64_t def) {
    return chars_to_int64(str.c_str(), def);
}

double chars_to_double(const char* str, double def) {
    char * end;
    double v = strtod(str, &end);
    if (*end != '\0') {
        return def;
    }
    return v;
}

double string_to_double(const std::string& str, double def) {
    return chars_to_double(str.c_str(), def);
}

int64_t get_json_int(const rapidjson::Document& doc, const std::string& key, int64_t def) {
    if (!doc.IsObject()) {
        return def;
    }

    if (doc.HasMember(key.c_str())) {
        auto& d = doc[key.c_str()];
        if (d.IsInt()) {
            return d.GetInt();
        }
        else if (d.IsInt64()) {
            return d.GetInt64();
        }
        else if (d.IsString()) {
            return chars_to_int64(d.GetString(), def);
        }
    }
    return def;
}


double get_json_double(const rapidjson::Document& doc, const std::string& key, double def) {
    if (!doc.IsObject()) {
        return def;
    }

    if (doc.HasMember(key.c_str())) {
        auto& d = doc[key.c_str()];
        if (d.IsNumber()) {
            return d.GetDouble();
        }
        else if (d.IsString()) {
            return chars_to_double(d.GetString(), def);
        }
    }

    return def;
}

bool hgetall_from_redis() {
    int64_t t0 = usec();
    redisContext* ctx = redisConnect("127.0.0.1", 6379);  
    if ( NULL == ctx || ctx->err)  {       
        // redis为NULL与redis->err是两种不同的错误，若redis->err为true，可使用redis->errstr查看错误信息  
        redisFree(ctx);
        printf("Connect to redisServer fail\n");  
        return false;  
    }
    printf("Connect to redisServer Success\n");
    int64_t connecttime = get_current_time_milli();
    redisReply *reply;
    struct timeval tv = { 1, 0 };
    redisSetTimeout(ctx,tv);
    const char* cmdtext = "EXISTS ga:123456:1"; 
    int num = 1;
    redisReply **replies = (redisReply**)malloc(sizeof(redisReply*)*num);
    int64_t t1 = usec();
    for (int i = 0; i < num; ++i) {
        redisAppendCommand(ctx, cmdtext);
    }
    printf("finish append\n");
    for (int i = 0; i < num; i++) {
        assert(redisGetReply(ctx, (void**)&replies[i]) == REDIS_OK);
        if (replies[i] == NULL) {
            printf("reply is null");
        }
    }
    printf("finish get reply\n");
    for (int i = 0; i < num; ++i) {
        reply = replies[i];
        printf("type = %d\n", reply->type);
        if( NULL == reply || reply->type != REDIS_REPLY_INTEGER) {
            redisFree(ctx);     // 命令执行失败，释放内存  
            printf("Failed\n");
            return false;  
        }
        printf("get string: %lld\n", reply->integer);
        /*
        size_t size = reply->elements;
        redisReply ** element = reply->element;
        printf("size = %d\n", size);
        for (int j = 0; j < size; j+=2) {
            redisReply * key = element[j];
            redisReply * val = element[j+1];
            int k = key->type;
            int v = atoi(val->str);
            printf("type = %d, value = %s\n", key->type, val->str);
        }*/
    }
    printf("finish processing\n");
    free(replies);
    int64_t t2 = usec();
    printf("connecttime = %lld, gettime = %lld\n", t1-t0,t2-t1);
    redisFree(ctx);     // 命令执行失败，释放内存  
    return true;  
}

bool doTest(int64_t group_id, int comment_type)  
{  
    int64_t t0 = usec();
    redisContext* ctx = redisConnect("127.0.0.1", 6379);  
    if ( NULL == ctx || ctx->err)  {       
        // redis为NULL与redis->err是两种不同的错误，若redis->err为true，可使用redis->errstr查看错误信息  
        redisFree(ctx);
        printf("Connect to redisServer fail\n");  
        return false;  
    }
    printf("Connect to redisServer Success\n");
    int64_t connecttime = get_current_time_milli();
    struct timeval tv = { 1, 0 };
    redisSetTimeout(ctx,tv);
    const char* cmdtext = "EXISTS ga:123456:0"; 
    int num = 1;
    redisReply **replies = (redisReply**)malloc(sizeof(redisReply*)*num);
    int64_t t1 = usec();
    printf("gid=%lld, comment_type=%d\n", group_id, comment_type);
    redisReply *reply;
    reply = (redisReply*)redisCommand(ctx, "EXISTS ga:%lld:%d", group_id, comment_type);
    printf("type = %d\n", reply->type);
    printf("get status: %d\n", reply->integer);
    freeReplyObject(reply);
    reply = (redisReply*)redisCommand(ctx, "get ga:%lld:%d", group_id, comment_type);
    printf("ga= %s\n", reply->str);
    rapidjson::Document doc;
    doc.Parse<0>(reply->str);
    printf("hello=%d\n", get_json_int(doc, "hello", 0));
    printf("world=%d\n", get_json_int(doc, "world", 0));
    reply = (redisReply*)redisCommand(ctx, "SMEMBERS gc:%lld:%d", group_id, comment_type);
    printf("%lld\n", reply->type);
    size_t size = reply->elements;
    printf("size = %lld\n", size);
    redisReply **element = reply->element;
    for (size_t i = 0; i < size; i++) {
        redisReply * tmp = element[i];
        printf("#%d:\t%s\n", i, tmp->str);
    }
    printf("finish processing\n");
    free(replies);
    int64_t t2 = usec();
    printf("connecttime = %lld, gettime = %lld\n", t1-t0,t2-t1);
    redisFree(ctx);     // 命令执行失败，释放内存  
    return true;   
}  
int main()  
{  
    doTest(123456, 0); 
    //hgetall_from_redis();
    return 0;  
}  