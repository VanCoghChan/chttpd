//
// Created by Nakano Yotsuba on 2023/12/20.
//

#include <time.h>
#include <stdio.h>
#include <WinSock2.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "cc.h"

#ifndef CCHAN_CCHTTPD_H
#define CCHAN_CCHTTPD_H

//初始化服务器
int start_server(unsigned short *port) {
    WSADATA wsaData;
    int ret = WSAStartup(
            MAKEWORD(1, 1),
            &wsaData
    );
    if (ret != 0) {
        printf("WSAStartup 初始化失败 %d\n", ret);
        error_die("WSAStartup初始化失败");
    }
    // 创建套接字
    int server_sock = socket(
            AF_INET, // IPv4
            SOCK_STREAM,
            IPPROTO_TCP
    );
    if (server_sock == INVALID_SOCKET) {
        printf("socket 创建失败 %d\n", WSAGetLastError());
        error_die("套接字创建失败");
    }
    // 设置端口复用
    int opt = 1;
    ret = setsockopt(
            server_sock,
            SOL_SOCKET,
            SO_REUSEADDR,
            (const char *) &opt,
            sizeof(opt)
    );
    if (ret == SOCKET_ERROR) {
        printf("setsockopt 设置失败 %d\n", WSAGetLastError());
        error_die("setsockopt失败");
    }
    // 绑定网络地址
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr)); // 初始化为0, 防止出现未知错误
    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_port = htons(*port); // 端口,必须使用htons将主机字节序转换为网络字节序
    server_addr.sin_addr.S_un.S_addr = INADDR_ANY; // 设置为任意地址
    // 绑定套接字
    ret = bind(
            server_sock,    // 套接字
            (struct sockaddr *) &server_addr,   // 服务器地址
            sizeof(server_addr)
    );
    if (ret == SOCKET_ERROR) {
        printf("bind 绑定失败 %d\n", WSAGetLastError());
        error_die("套接字绑定失败");
    }
    // 如果端口为0,则随机分配一个端口
    if (*port == 0) {
        int namelen = sizeof(server_addr);
        ret = getsockname(
                server_sock,
                (struct sockaddr *) &server_addr,
                &namelen
        );
        if (ret == SOCKET_ERROR) {
            printf("getsockname 获取端口失败 %d\n", WSAGetLastError());
            error_die("获取端口失败");
        }
        *port = ntohs(server_addr.sin_port);
    }
    // 设置监听队列
    ret = listen(
            server_sock,
            5
    );
    if (ret == SOCKET_ERROR) {
        printf("listen 监听失败 %d\n", WSAGetLastError());
        error_die("监听失败");
    }
    // 返回初始化成功的套接字
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

// 正常业务路程时，向客户端返回响应头信息
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

#endif //CCHAN_CCHTTPD_H
