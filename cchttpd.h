//
// Created by 燦 on 2023/12/20.
//

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "cc.h"

#ifndef CCHAN_CCHTTPD_H
#define CCHAN_CCHTTPD_H

// 初始化服务器，返回服务器套接字
int start_server(unsigned short *port) {
    int server_sock;
    // 创建socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);

    // 端口复用
    int opt = 1;
    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 绑定地址
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if(*port == 0) {
        server_addr.sin_port = 0; // 让系统自动分配端口
    } else {
        server_addr.sin_port = htons(*port);
    }

    bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // 获取分配的端口
    if(*port == 0) {
        socklen_t addr_len = sizeof(server_addr);
        getsockname(server_sock, (struct sockaddr*)&server_addr, &addr_len);
        *port = ntohs(server_addr.sin_port);
    }

    // 开始监听
    listen(server_sock, 5);
    return server_sock;
}

// 从指定的客户端套接字，读取一行数据，保存到buf中，最多读取size个字节,返回值：读取的字节数
int get_line(int sock, char buf[1024], int size) {
    char c = '\0';
    int i = 0;
    while(c!='\n' && i<size-1) {
        int n = recv(sock, &c, 1, 0);
        if (n>0){
            if (c == '\r'){
                n = recv(sock, &c, 1, MSG_PEEK);
                if (n>0 && c == '\n'){
                    recv(sock, &c, 1, 0);
                }
                else{
                    c = '\n';
                }
            }
            buf[i++] = c;
        }
        else{
            c = '\n';
        }
    }
    buf[i] = '\0';
    return i;
}

// 向客户端返回404错误的响应头以及响应体
void send_404(int client_sock){
    // 发送响应头
    char buff[1024];
    strcpy(buff, "HTTP/1.1 404 NOT FOUND\r\n");
    send(client_sock, buff, strlen(buff), 0);
    strcpy(buff, "Server: CChttpd/0.1.0\r\n");
    send(client_sock, buff, strlen(buff), 0);
    strcpy(buff, "Date: ");
    strcat(buff, get_time_str());
    strcat(buff, "\r\n");
    send(client_sock, buff, strlen(buff), 0);
    send(client_sock, "\r\n", 2, 0);
    // 发送响应体
    strcpy(buff, "<html><head><title>404 Not Found</title></head><body><h1>404 Not Found</h1></body></html>");
    send(client_sock, buff, strlen(buff), 0);
    PRINTF("HTTP/1.1 404 NOT FOUND", get_time_str());
    printf("\n\n");
}

// 向客户端返回501错误的响应头以及响应体
void send_501(int client_sock){
    // 发送响应头
    char buff[1024];
    strcpy(buff, "HTTP/1.1 501 Method Not Implemented\r\n");
    send(client_sock, buff, strlen(buff), 0);
    strcpy(buff, "Server: CChttpd/0.1.0\r\n");
    send(client_sock, buff, strlen(buff), 0);
    strcpy(buff, "Date: ");
    strcat(buff, get_time_str());
    strcat(buff, "\r\n");
    send(client_sock, buff, strlen(buff), 0);
    send(client_sock, "\r\n", 2, 0);
    // 发送响应体
    strcpy(buff, "<html><head><title>501 Method Not Implemented</title></head><body><h1>Post Method Not Implemented</h1></body></html>");
    send(client_sock, buff, strlen(buff), 0);
    PRINTF("HTTP/1.1 501 Method Not Implemented", get_time_str());
    printf("\n\n");
}

// 正常业务流程时，向客户端返回响应头信息
void send_response_header(int client_sock, char *content_type){
    char buff[1024];
    strcpy(buff, "HTTP/1.1 200 OK\r\n");
    send(client_sock, buff, strlen(buff), 0);
    strcpy(buff, "Server: CChttpd/0.1.0\r\n");
    send(client_sock, buff, strlen(buff), 0);
    strcpy(buff, "Date: ");
    strcat(buff, get_time_str());
    strcat(buff, "\r\n");
    send(client_sock, buff, strlen(buff), 0);
    strcpy(buff, "Content-Type: ");
    strcat(buff, content_type);
    strcat(buff, "\r\n");
    send(client_sock, buff, strlen(buff), 0);
    send(client_sock, "\r\n", 2, 0);
    PRINTF("HTTP/1.1 200 OK", get_time_str());
    printf("\n\n");
}

// 处理http请求的线程函数
void* handle_request_80(void* arg){
    printf("handle_request_80\n");
    // to do
    return NULL;
}

// 处理https请求的线程函数
void* handle_request_443(void* arg){
    printf("handle_request_443\n");
    char buf[4096];
    int client_sock = *(int*) arg;
    //读第一行数据
    get_line(client_sock, buf, sizeof(buf));
    //获取当前时间
    char *current_time = get_time_str();
    PRINTF(buf, current_time);
    // 判断请求类型并获取url
    char method[255];
    char url[255];
    char path[512];
    sscanf(buf, "%[^ ] %[^ ]", method, url);
    // 判断请求类型
    if (strcmp(method, "GET") == 0){
        // 判断url是否为根目录
        if (strcmp(url, "/") == 0){
            strcpy(path, "./resources/index.html");
        }
        else{
            strcpy(path, "./resources");
            strcat(path, url);
        }
        // 打开文件
        FILE *fp = fopen(path, "rb");
        if (fp == NULL){
            // 如果文件不存在，返回404信息
            send_404(client_sock);
        }
        else{
            // 如果文件存在
            // 先判断path指向的文件类型
            char *file_type = get_file_type(path);
            // 发送文件内容前，先发送确认相应头
            send_response_header(client_sock, file_type);
            // 最后再读取文件内容并发送
            int bytees_read = 0;
            while (1){
                // 读取文件内容,n>0则成功读取到文件内容
                int n = fread(buf, sizeof(char), sizeof(buf), fp);
                if (n > 0){
                    send(client_sock, buf, n, 0);
                    bytees_read += n;
                }
                else{
                    break;
                }
            }
            fclose(fp);
        }
    }
    else{
        // 不支持的请求类型 POST
        send_501(client_sock);

    }
    pthread_exit(NULL);
}

#endif //CCHAN_CCHTTPD_H
