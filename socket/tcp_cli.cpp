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
    while(1)
    {
        printf("client say:");
        fflush(stdout);
        std::string buf;
        std::cin>>buf;

        CHECK_RET(cli_sock.Send(buf));

        buf.clear();
        CHECK_RET(cli_sock.Recv(&buf));
        printf("server say:%s\n",buf.c_str());
    }
    cli_sock.Close();
    return 0;
}

