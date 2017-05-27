// Server.cpp : �������̨Ӧ�ó������ڵ㡣
//


#include "stdafx.h"
#include <stdio.h>
#include <winsock2.h>
#include "LSX_Socket.h"

#pragma comment(lib,"ws2_32.lib")
using namespace std;

int main(int argc, char* argv[])
{	
	int num = 111;
	LSX_socket_control socketControl;
	socketControl.message.InitMsgBody();
	socketControl.message.InitMsgHead();
	
	socketControl.message.AllMsg();
	socketControl.message.test(num);
	cout << "=====" << num << endl;
	
	
	//��ʼ��WSA
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}

	//�����׽���
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		printf("socket error !");
		return 0;
	}


	//��IP�Ͷ˿�
	/*
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}
	*/
	socketControl.LSX_AddressServer(slisten);


	//��ʼ����
	if (listen(slisten, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
		return 0;
	}

	//ѭ����������
	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	char revData[255];
	while (true)
	{
		printf("�ȴ�����...\n");
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
		if (sClient == INVALID_SOCKET)
		{
			printf("accept error !");
			continue;
		}
		printf("���ܵ�һ�����ӣ�%s \r\n", inet_ntoa(remoteAddr.sin_addr));

		//��������
		while (true)
		{
		
			int ret = recv(sClient, revData, 255, 0);
			if (ret > 0)
			{
				revData[ret] = 0x00;
				printf(revData);
			}

			//��������
			char * sendData = "��ã�TCP�ͻ��ˣ�\n";
			const char *p = socketControl.message.allMsg.data();
			send(sClient, p, strlen(sendData), 0);
		}
			closesocket(sClient);
	}

	closesocket(slisten);
	WSACleanup();
	return 0;
}
