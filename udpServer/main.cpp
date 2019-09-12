
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
	int strLen = 0;												//�ַ�������
	WSADATA wsaData;

	//��ʼ��winsock
	WORD wsVersion = MAKEWORD(2, 2);
	if(WSAStartup(wsVersion,&wsaData) !=0 )
	{
		printf("WSAStartup failed\n");
		return -1;
	}

	//�����׽���
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);	
	//�������ʧ��
	if(sock == INVALID_SOCKET)
	{
		perror("socket error \n");
		return -1;
	}

	sockaddr_in serverAddr;										//�����ͻ��˵�ַ��Ϣ�ṹ��
	memset(&serverAddr, 0, sizeof(serverAddr));					//���

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(CLIENT_PORT);					//�˿�
	//serverAddr.sin_addr.s_addr =htonl(INADDR_ANY);			//��ַ
	serverAddr.sin_addr.s_addr =  inet_addr("192.168.234.1");	//����IP
		
	sockaddr_in clntAddr;										//�ͻ��˵�ַ��Ϣ
	clntAddr.sin_port = htons(CLIENT_PORT);						//�˿�
	//clntAddr.sin_addr.s_addr =  inet_addr("192.168.234.129");		//�ͻ���IP
	clntAddr.sin_addr.s_addr =htonl(INADDR_ANY);			//��ַ
	int nSize = sizeof(clntAddr);


	//���׽��ֺ͵�ַ
	if(bind(sock, (SOCKADDR*)&serverAddr, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		perror("bind error\n");
		return -1;
	}

	//���տͻ�������
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
			printf(" received from %s   :  %s\n", inet_ntoa(clntAddr.sin_addr),revBuffer);	//��ӡ���յ�����

			if(strcmp(revBuffer,"quit") == 0)			//���յ�"quit"�ַ����˳�����
				break;
		}

	}

	closesocket(sock);
	WSACleanup();


	getchar();
	return 0;
}








