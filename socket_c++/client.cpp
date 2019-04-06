#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include<iostream>
#include <winsock2.h>
#define SERVER_PORT 9210 //�����˿�
#pragma comment(lib,"ws2_32.lib")
 
struct Data
{
	int length;
	char sendMessage[2000];
	int fin;
}data;
 
int main()
{
	FILE * fp;
	errno_t err = fopen_s(&fp, "1.jpg", "rb+");
	WORD wVersionRequested;
	WSADATA wsaData;
	int ret;
	SOCKET sClient; //�����׽���
	struct sockaddr_in saServer; //��ַ��Ϣ
 
	BOOL fSuccess = TRUE;
 
	fseek(fp, 0, SEEK_END);
	int end = ftell(fp);
	fseek(fp, 0, 0);
	//WinSock��ʼ��
	wVersionRequested = MAKEWORD(2, 2); //ϣ��ʹ�õ�WinSock DLL�İ汾
	ret = WSAStartup(wVersionRequested, &wsaData);
	if (ret != 0)
	{
		printf("WSAStartup() failed!\n");
		return 0;
	}
	//ȷ��WinSock DLL֧�ְ汾2.2
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		printf("Invalid WinSock version!\n");
		return 0;
	}
	//����Socket,ʹ��TCPЭ��
	sClient = socket(AF_INET, SOCK_STREAM, 0);
	if (sClient == INVALID_SOCKET)
	{
		WSACleanup();
		printf("socket() failed!\n");
		return 0;
	}
	//������������ַ��Ϣ
	saServer.sin_family = AF_INET; //��ַ����
	saServer.sin_port = htons(SERVER_PORT); //ע��ת��Ϊ�������
	saServer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	//���ӷ�����
	ret = connect(sClient, (struct sockaddr *)&saServer, sizeof(saServer));
	if (ret == SOCKET_ERROR)
	{
		printf("connect() failed!\n");
		closesocket(sClient); //�ر��׽���
		WSACleanup();
		return 0;
	}
 
	int nZero = 10;
	setsockopt(sClient, SOL_SOCKET, SO_SNDBUF, (char *)&nZero, sizeof(nZero));
	while (end > 0)
	{
		memset(data.sendMessage, '0', sizeof(data.sendMessage));
		fread(data.sendMessage, 1024, 1, fp);
		if (end >= 1024) //ʣ��ܶ���Ϣ
		{
			data.fin = 0;
			data.length = 1024;
		}
		else
		{
			data.fin = 1;
			data.length = end;
		}
		ret = send(sClient, (char *)&data, sizeof(struct Data), 0);
		if (ret == SOCKET_ERROR)
		{
			printf("send() failed!\n");
		}
		else
		{
			end -= 1024;
		}
	}
 
	closesocket(sClient); //�ر��׽���
	WSACleanup();
	return 0;
}

