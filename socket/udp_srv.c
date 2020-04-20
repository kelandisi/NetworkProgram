/*****************************************************
FileName:udp_srv.c
Author: MrAi
Mail:1191054822@qq.com
Created Time: 2020年04月04日 星期六 11时31分08秒
Notes:编写一个udp服务端的c语言程序
    1、创建套接字
    2、为套接字绑定地址信息
    3、接受数据
    4、发送数据
    5、关闭套接字
***************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h> //sockaddr结构体 /IPPROTO_UDP
#include <arpa/inet.h> //包含一些字节序转化的接口
#include <sys/socket.h> //套接字接口头文件

int main(int argc, char *argv[])//agrc表示参数个数，通过argv向程序传递端口参数
{
    if(argc!=3)
    {
        printf("./udp_srv ip port em:./udp_srv 192.168.122.132 9000\n");
        return -1;
    }
    const char* ip_addr=argv[1];
    uint16_t port_addr=atoi(argv[2]);
    
    //socket(地址域，套接字类型，协议类型)
    int sockfd = socket(AF_INET , SOCK_DGRAM, IPPROTO_UDP);
    if(sockfd<0)
    {
        perror("socket error");
        return -1;
    }

    //bind (套接字描述符， 地址结构，地址长度)
    //struct sockaddr_in   ipv4地址结构
    //struct in_addr{ uint32_t s_addr }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    //htons-将两个字节的主机字节序整数转换成网络字节序的整数
    addr.sin_port = htons(port_addr);//不能直接赋值，这里需要的是网络字节序
                              //而port_addr是点分十进制的形式，所以要进行字节序转换
    //inet_addr 将一个点分十进制的字符串IP地址转换为网络字节序的整数IP地址
    addr.sin_addr.s_addr =inet_addr(ip_addr);
    socklen_t len=sizeof(struct sockaddr_in);
    int ret = bind(sockfd, (struct sockaddr*)&addr,len);
    if(ret<0)
    {
        perror ("bind error");
        return -1;
    }
    while(1)
    {
        char buf[1024]={0};
        struct sockaddr_in cli_addr;
        socklen_t len=sizeof(struct sockaddr_in);
        //recvfrom (描述符，缓冲区，长度，参数，客户端地址信息，地址信息长度)
        //阻塞接受数据，将数据放入buf中，将发送端的地址放入cli_addr中
        int ret = recvfrom(sockfd,buf,1023,0,(struct sockaddr*)&cli_addr,&len);
        if(ret < 0)
        {
            perror("recvfrom perror");
            close(sockfd);
            return -1;
        }

        printf("client say: %s\n",buf);

        printf("sever say:");
        fflush(stdout);//让用户输入数据，发送给客户端
        memset(buf,0x00,1024);//清空buf中的数据
        scanf("%s",buf);
        
        //通过sockfd将buf中的数据发送到cliaddr客户端中
        ret = sendto(sockfd,buf, strlen(buf),0,(struct sockaddr*)&cli_addr,len);
        if(ret<0)
        {
            perror("sendto error");
            close(sockfd);
            return -1;
        }
    }
    close(sockfd);
    return 0;
}

