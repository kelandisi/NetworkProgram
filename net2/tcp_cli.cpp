/*****************************************************
FileName:tcp_cli.cpp
Author: MrAi
Mail:1191054822@qq.com
Created Time: 2020年04月06日 星期一 16时46分08秒
Notes:通过封装的tcpsocket类实例化客户端对象，实现tcp客户端程序
***************************************************/
#include <iostream>
#include <stdlib.h>
#include "tcp_socket.hpp"
int main(int argc,char* argv[])
{
    if(argc!=3)
    {
        printf("em:./tcp_cli  192.168.132.122 9000-----服务端绑定的地址\n");
        return -1;
    }
    std::string ip=argv[1];
    uint16_t port = atoi(argv[2]);
    tcp_socket cli_sock;
    //创建套接字
    CHECK_RET(cli_sock.Socket());
    //绑定地址信息（不推荐）
    //向服务端发起请求
    CHECK_RET(cli_sock.Connect(ip,port));
    //循环收发数据
    int fd=cli_sock.GetFD();
    while(1)
    {
        cal_t tmp; //定义一个结构体变量，申请了一块空间
        tmp.num1=10;
        tmp.num2=20;
        tmp.op='+';//成员变量赋值的过程就是数据对象在内存中组织排布的过程

        send(fd, (void*)&tmp,sizeof(cal_t),0);//将这块空尽力的数据发送给服务端
        sleep(100);
    }
    cli_sock.Close();
    return 0;
}

