#include <stdio.h>
#include <WinSock2.h>
#include <string.h>
#include <unistd.h>
#include "cc.h"
#include "cchttpd.h"


#pragma comment(lib, "ws2_32.lib")


// 处理用户请求的线程函数
DWORD WINAPI handle_request(LPVOID arg){
    char buf[4096];
    int client_sock = (SOCKET) arg;
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
    if (stricmp(method, "GET") == 0){
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
    return 0;
}


int main(void){
    // 使用utf-8编码，避免命令行中文乱码
    system("chcp 65001");
    // 切换工作目录为上层目录
    chdir("..");
    unsigned short port_80 = 80, port_443 = 443;
    int server_sock_80 = start_server(&port_80);
    printf("Server started on port %d...\n", port_80);
    struct sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);
    while (1) {
        // 阻塞式等待客户端连接
        int client_sock_80 = accept(
                server_sock_80,
                (struct sockaddr *) &client_addr,
                &client_addr_len
                );
        if (client_sock_80 == INVALID_SOCKET) {
            printf("accept 接受失败 %d\n", WSAGetLastError());
            error_die("接受失败");
        }

        // 使用client_sock与客户端通信,使用多线程处理多个服务请求
        // 创建一个新的线程，用于监听80端口（http）
        DWORD thread_id = 0;
        HANDLE thread = CreateThread(
                0,   // 默认安全属性
                0,      // 默认堆栈大小
                handle_request, // 线程函数
                (void *) client_sock_80, // 线程函数参数
                0,      // 默认创建后立即执行
                &thread_id  // 线程id
                );

        // 创建另一个线程，用于监听443端口（https）
//        DWORD thread_id2 = 1;
//        HANDLE thread2 = CreateThread(
//                0,   // 默认安全属性
//                0,      // 默认堆栈大小
//                handle_request, // 线程函数
//                (void *) client_sock_80, // 线程函数参数
//                0,      // 默认创建后立即执行
//                &thread_id2  // 线程id
//                );

    }
    closesocket(server_sock_80);
    return 0;
}