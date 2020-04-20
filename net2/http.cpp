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
#include <sstream>

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

        std::string http_req;
        cli_sock.Recv(&http_req);
        printf("req: [%s]\n",http_req.c_str());

        //响应-----首行(版本、状态码、描述)-头部(Content-Legth)-空行-正文
        std::string body = "<html><body><h1>Hello World</h1></body></html>";
        std::string blank = "\r\n";
        std::stringstream header;
        header << "Content-Length: "<< body.size()<<"\r\n";
        header << "Content-Type: text/html\r\n";
        header << "Location: http://www.baidu.com/\r\n";
        std::string first_line = "HTTP/1.1 500 Internal Server Error\r\n";

        cli_sock.Send(first_line);
        cli_sock.Send(header.str());
        cli_sock.Send(blank);
        cli_sock.Send(body);
        cli_sock.Close();
    }
    lst_sock.Close();
    return 0;
}

