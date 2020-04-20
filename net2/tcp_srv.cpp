/*=============================================================== 
*   Copyright (C) . All rights reserved.")
*   文件名称： 
*   创 建 者：zhang
*   创建日期：
*   描    述：使用封装的TcpSocket类实例化对象实现tcp服务端程序 
================================================================*/

#include <iostream>
#include <stdlib.h>
#include "tcp_socket.hpp"


int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("em:./tcp_srv 192.168.122.132 9000\n");
        return -1;
    }
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    tcp_socket lst_sock;
    CHECK_RET(lst_sock.Socket());//创建套接字
    //#define CHECK_RET(q) if((q)==false){return -1;}
    //if (lst_sock.Socket() == false) {return -1;}
    CHECK_RET(lst_sock.Bind(ip, port));//绑定地址信息
    CHECK_RET(lst_sock.Listen());//开始监听
    while(1) {
        tcp_socket cli_sock;
        std::string cli_ip;
        uint16_t cli_port;
        //Accept类成员函数，使用的私有成员_sockfd就是lst_sock的私有成员
        //cli_sock取地址传入，目的是为了获取accept接口返回的通信套接字描述符
        bool ret = lst_sock.Accept(&cli_sock);//获取新套接字
        if (ret == false) {
            //获取新连接失败，可以重新继续获取下一个
            continue;
        }
        int fd=cli_sock.GetFD();
        cal_t tmp; //定义变量申请了一块空间
        recv(fd,(void*)&tmp,sizeof(cal_t),0);
        int sum=tmp.num1+tmp.num2;
        printf("%d %c %d = %d\n",tmp.num1,tmp.op,tmp.num2,sum);

    }
    lst_sock.Close();
    return 0;
}

