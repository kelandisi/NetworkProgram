/*****************************************************
FileName:tcp_srv.cpp
Author: MrAi
Mail:1191054822@qq.com
Created Time: 2020年04月06日 星期一 15时55分51秒
Notes:使用封装的tcpsocket类实例化对象，实现tcp服务端程序
 ***************************************************/
#include <iostream>
#include <stdlib.h>
#include "tcp_socket.hpp"
#include <pthread.h>

void *thr_start(void *arg)
{   
    long fd=(long)arg;
    tcp_socket cli_sock ;
    cli_sock.SetFD(fd);
    while(1)
    {
        std::string buf;
        if(cli_sock.Recv(&buf) ==false)
        {
            cli_sock.Close();//通信套接字接受数据出错，关闭的是通信套接字
            pthread_exit(NULL);
        }
        printf("client say:%s\n",&buf[0]);

        std::cout<<"server say:";
        fflush(stdout);
        buf.clear();
        std::cin>>buf;
        if(cli_sock.Send(buf) == false)
        {
            cli_sock.Close();
            pthread_exit(NULL);
        } 

    }
    cli_sock.Close();
    return NULL;
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
        pthread_t tid;
        //将通信套接字当做参数传递给线程，让线程与客户端进行通信
        //cli_sock是一个局部变量--循环完了这个资源就会被释放
        pthread_create(&tid,NULL,thr_start,(void*)cli_sock.GetFD());//线程
        pthread_detach(tid);//不关心线程的返回值，分离线程，退出后自动释放资源   (不需要进行进程等待了)
        //主线程里面不能关闭cli_sock套接字，因为多线程是共用描述符的  (县城中是共用资源的)
    }
lst_sock.Close();
return 0;
}
