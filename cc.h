//
// Created by Nakano Yotsuba on 2023/12/20.
//
#include <time.h>
#include <stdio.h>

#ifndef CCHAN_CC_H
#define CCHAN_CC_H

//定义打印宏，打印时间，函数名，行号，传入的字符串
#define PRINTF(str, time) printf("%s [%s-%d] %s", time, __func__ , __LINE__, str)

// 错误处理
void error_die(const char *string) {
    perror(string);
    exit(-1);
}

// 获取当前时间并格式化，返回值为格式化后的字符串
char *get_time_str(){
    time_t rawtime;
    struct tm *info;
    static char time_str[80];
    time( &rawtime );
    info = localtime( &rawtime );
    strftime(time_str,80,"%Y-%m-%d %H:%M:%S", info);
    return time_str;
}

// 获取path中的文件类型
char *get_file_type(char *path){
    char *file_type = NULL;
    char *p = strrchr(path, '.');
    if (p == NULL){
        file_type = "text/plain";
    }
    else if (strcmp(p, ".html") == 0){
        file_type = "text/html";
    }
    else if (strcmp(p, ".jpg") == 0){
        file_type = "image/jpeg";
    }
    else if (strcmp(p, ".png") == 0){
        file_type = "image/png";
    }
    else if (strcmp(p, ".gif") == 0){
        file_type = "image/gif";
    }
    else{
        file_type = "text/plain";
    }
    return file_type;
}

// 打印当前工作目录
void print_cwd(){
    char buf[512];
    getcwd(buf, sizeof(buf));
    printf("cwd: %s\n", buf);
}



#endif //CCHAN_CC_H
