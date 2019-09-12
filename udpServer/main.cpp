
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <WinSock2.h>
#include "udpServer.h"


#pragma comment(lib, "ws2_32.lib")
#define BUF_SIZE 100
#define CLIENT_PORT 69

int main(void)
{
	char revBuffer[BUF_SIZE] = {0};
	char sendBuffer[BUF_SIZE] = {0};
	int strLen = 0;												//字符串长度
	WSADATA wsaData;

	//初始化winsock
	WORD wsVersion = MAKEWORD(2, 2);
	if(WSAStartup(wsVersion,&wsaData) !=0 )
	{
		printf("WSAStartup failed\n");
		return -1;
	}

	//创建套接字
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);	
	//如果创建失败
	if(sock == INVALID_SOCKET)
	{
		perror("socket error \n");
		return -1;
	}

	sockaddr_in serverAddr;										//声明客户端地址信息结构体
	memset(&serverAddr, 0, sizeof(serverAddr));					//清空

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(CLIENT_PORT);					//端口
	//serverAddr.sin_addr.s_addr =htonl(INADDR_ANY);			//地址
	serverAddr.sin_addr.s_addr =  inet_addr("192.168.234.1");	//本机IP
		
	sockaddr_in clntAddr;										//客户端地址信息
	clntAddr.sin_port = htons(CLIENT_PORT);						//端口
	//clntAddr.sin_addr.s_addr =  inet_addr("192.168.234.129");		//客户端IP
	clntAddr.sin_addr.s_addr =htonl(INADDR_ANY);			//地址
	int nSize = sizeof(clntAddr);


	//绑定套接字和地址
	if(bind(sock, (SOCKADDR*)&serverAddr, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		perror("bind error\n");
		return -1;
	}

	//接收客户端请求
	while(1)
	{
		strLen = recvfrom(sock, revBuffer,  BUF_SIZE, 0,(struct sockaddr*) &clntAddr, &nSize);
		if(strLen<0)
		{
			perror("recvfrom");
			return 3;
		}
		else if(strLen == 0)
		{
			printf("server closed\n");
		}
		else
		{
			revBuffer[strLen] = 0;
			printf(" received from %s   :  %s\n", inet_ntoa(clntAddr.sin_addr),revBuffer);	//打印接收的内容

			if(strcmp(revBuffer,"quit") == 0)			//接收到"quit"字符串退出程序
				break;
		}

	}

	closesocket(sock);
	WSACleanup();


	getchar();
	return 0;
}








