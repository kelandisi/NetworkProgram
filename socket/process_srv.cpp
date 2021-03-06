/*****************************************************
FileName:tcp_srv.cpp
Author: MrAi
Mail:1191054822@qq.com
Created Time: 2020年04月06日 星期一 15时55分51秒
Notes:使用封装的tcpsocket类实例化对象，实现tcp服务端程序
 ***************************************************/
#include <iostream>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include "tcp_socket.hpp"
void sigcb(int signo)
{
    //当子进程退出的时候就会向父进程发送SIGCHLD信号，回调这个函数
    while(waitpid(-1,0,WNOHANG)>0);//  >0表示处理了一个退出的子进程
                                    //  <0表示没有退出的子进程
                                    //  一次回调循坏将退出的子进程全部退出
}
int main(int argc,char* argv[])
{
    if(argc!=3)
    {
        printf("em:./tcp_srv 192.168.122.132 9000\n");
        return -1;
    }
    std::string ip=argv[1];
    uint16_t port = atoi(argv[2]);//stoi将字符串转化为数字

    signal(SIGCHLD,sigcb);
    tcp_socket lst_sock;//监听套接字
    CHECK_RET(lst_sock.Socket());
    CHECK_RET(lst_sock.Bind(ip,port));
    CHECK_RET(lst_sock.Listen());
    while(1)
    {
        tcp_socket cli_sock;
        std::string cli_ip;
        uint16_t cli_port;
        //Accept类成员函数，使用的私有成员_sockfd就是lst_sock的私有成员
        //cli_sock取地址传入，目的是为了获取accept接口返回的通信套接字描述
        bool ret = lst_sock.Accept(&cli_sock,&cli_ip,&cli_port);
        if(ret==false)
        {
            //获取连接失败，可以重新继续获取下一个连接
            continue;
        }
        printf("new connect:[%s:%d]\n",cli_ip.c_str(),cli_port);
        //---------------------------------------
        pid_t pid = fork();
        if(pid==0)//子进程复制父进程  --- 数据独有，代码共享
        {
            //让子进程处理与客户端通信
            while(1)
            {
                std::string buf;
                if(cli_sock.Recv(&buf) ==false)
                {
                    cli_sock.Close();//通信套接字接受数据出错，关闭的是通信套接字
                    exit(0);
                }
                printf("client:[%s:%d] say:%s\n",&cli_ip[0],cli_port,&buf[0]);

                std::cout<<"server say:";
                fflush(stdout);
                buf.clear();
                std::cin>>buf;
                if(cli_sock.Send(buf) == false)
                {
                    cli_sock.Close();
                    exit(0);
                } 
            }
            cli_sock.Close();
            exit(0);
        }
        //父子进程数据独有，都会具有cli_sock，但是父进程并不通信
        cli_sock.Close();//这个关闭对子进程没有影响，因为数据各自有一份
        //wait(NULL);//等待子进程退出，防止出现僵尸进程，但是又会出现wait()等待，程序阻塞
    }
        lst_sock.Close();
        return 0;
    }
