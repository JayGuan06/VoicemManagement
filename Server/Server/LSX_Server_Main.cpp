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
	LSX_message_control message("abcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefgh",800,0);
	message.test(num);
	cout << "numtest=====" << num << endl;
	
	
	message.socketControl.LSX_WSA_Init();

	message.socketControl.LSX_Socket();
	message.socketControl.LSX_AddressServer();
	message.socketControl.LSX_Bind();
	message.socketControl.LSX_Listen();

	

	//ѭ����������
	
	message.socketControl.nAddrlen = sizeof(message.socketControl.remoteAddr);
	
	while (true)
	{
		printf("�ȴ�����...\n");
		message.socketControl.LSX_Accept();
		printf("���ܵ�һ�����ӣ�%s \r\n", inet_ntoa(message.socketControl.remoteAddr.sin_addr));

		//��������
		while (true)
		{
		
			message.LSX_ReceiveData();
			message.LSX_SendCmd();

		}
		message.socketControl.LSX_Close();
	}
	message.socketControl.LSX_Close();
	message.socketControl.LSX_WSA_Clean();
	return 0;
}
