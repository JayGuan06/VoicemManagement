// Client.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <WINSOCK2.H>
#include <STDIO.H>
#include <iostream>
#include <string.h>
#include <WinBase.h>
#include <Windows.h>
#include "message.h"
#include "md5.h"

using namespace std;
#pragma  comment(lib,"ws2_32.lib")
char dbuf[128];

byte m801C[1024];
byte byte_src[4];


void IntToHex(byte byte_src[], int value)//big  ���
{

	//byte byte_src[4];//transform 
	/*int to hex for trasform*/
	memset(byte_src, 0, 4);
	byte_src[0] = (byte)((value & 0xFF000000) >> 24);
	byte_src[1] = (byte)((value & 0x00FF0000) >> 16);
	byte_src[2] = (byte)((value & 0x0000FF00) >> 8);
	byte_src[3] = (byte)((value & 0x000000FF));

	for (int i = 0; i < 4; i++)
		printf("%02x:%d||", byte_src[i], byte_src[i]);
}








int HexToInt(byte arry[], int postion)//big ���
{
	int value;
	value = (int)((arry[postion + 3] & 0xFF)
		| ((arry[postion + 2] << 8) & 0xFF00)
		| ((arry[postion + 1] << 16) & 0xFF0000)
		| ((arry[postion + 0] << 24) & 0xFF000000));
	return value;
}

//****************************
/*
void m800CInit(int x)
{
	byte byte_src[4];//transform 
	memset(M, 0, sizeof(M));
//��Ϣͷ

	for (int i = 0; i < 12; i++)
	{
		M[i] = Command800head[i];
	}
	IntToHex(byte_src, 128);//128   00:00:00:80
	for (int i = 12; i < 16; i++)
	{
		M[i] = byte_src[i - 12];
		printf("KKK:%02x\n", M[i]);
	}


	//int X = mInfo.m800Body.m800Body.length();
	char buf[129];//128+1

	//strcpy(letter, word.c_str());
	strcpy(buf,);

	cout << "ԭ����buf:" << buf << endl;


	for (int i = 16; i < 144; i++)
	{
		M[i] = buf[i - 16];
	}

	Analysis();

}
*/


//��>>������
void m801CInit(byte stemp[])
{
	string m;
	for (int i = 0; i < 128; i++)
	{
		m[i] = stemp[17 + i];
	}
	MD5 MD5(m);
	string result = MD5.md5();
	cout << "abc MD5 is:" << result << endl;
	memset(M, 0, 1024);
	for (int i = 0; i < 12; i++)
	{
		M[i] = Command800head[i];//801C��Ϣͷ
	}

	byte szlength[] = { 0x00, 0x00, 0x00, 0x10 };
	for (int i = 0; i < 4; i++)
	{
		M[i + 12] = szlength[i];//������ժҪ���� ==16
	}

	for (int i = 0; i < 16; i++)
	{
		M[i + 16] = MD5.getResult()[i];//������ժҪ
	}

	char mString[] = "12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678";
	printf("Դ����:");
	for (int i = 0; i < 128; i++)//����
	{
		M[i + 32] = mString[i];
		printf("%c", M[i + 32]);
	}
	printf("\n");

}

/*
//��>>�� ժҪ ��ͨ��801 command�е����� ʹ��rkey����
void m801RInit()
{
	MD5 MD5("abcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefgh");
	string result = MD5.md5();
	cout << "abc MD5 is:" << result << endl;
	memset(M, 0, 1024);
	for (int i = 0; i < 12; i++)
	{
		M[i] = Result801head[i];
	}//801R ͷ��Ϣ

	byte time[] = { 0x00, 0x00, 0x00, 0x11 };
	for (int i = 0; i < 4; i++) //ʱ�䳤��
	{
		M[i + 12] = time[i];
	}
	char nowTime[18];
	GetTime(nowTime);
	for (int i = 0; i < 17; i++)
	{
		M[i + 16] = nowTime[i];
	}
	byte czlength[] = { 0x00, 0x00, 0x00, 0x10 };
	for (int i = 0; i < 4; i++)
	{
		M[i + 33] = czlength[i];
	}

	for (int i = 0; i < 53; i++)
	{
		M[i + 47] = MD5.getResult()[i];
	}

	printf("Դ���ݣ�");
	printf("\n");
	Analysis();
}
*/
/*s
//������>>����
void m802CInit()
{
	//�û���ѯĳһʱ����е�Ϣ��test
	char time[18];
	memset(M, 0, 1024);
	for (int i = 0; i < 12; i++)
	{
		M[i] = Result801head[i];
	}//801R ͷ��Ϣ

	for (int i = 0; i < 12; i++)
	{
		M[i] = Command802head[i];
	}//��Ϣͷ
	for (int i = 0; i < 3; i++)
	{
		M[i + 12] = 0x00;
	}
	M[15] = 0x01;//ͨ����  �ɸ���

	for (int i = 0; i < 3; i++)
	{
		M[i + 16] = 0x00;
	}
	M[20] = 0x11;
	char startTime[18];
	GetTime(startTime);
	for (int i = 0; i < 17; i++)
	{
		M[i + 21] = startTime[i];
	}
	for (int i = 0; i < 3; i++)
	{
		M[i + 38] = 0x00;
	}
	M[39] = 0x01;
	for (int i = 0; i < 17; i++)
	{
		M[i + 40] = startTime[i];
	}


	printf("Դ���ݣ�");
	printf("\n");
	Analysis();


}
*/
//***************************9






int main(int argc, char* argv[])
{

	

	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0)
	{
		return 0;
	}

	SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sclient == INVALID_SOCKET)
	{
		printf("invalid socket !");
		return 0;
	}

	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(8888);
	serAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{
		printf("connect error !");
		closesocket(sclient);
		return 0;
	}
	while (1)
	{
		char * sendData = "��ã�TCP����ˣ����ǿͻ���!\n";
		 cout << "�������ݰ�" << endl;
		//send(sclient, sendData, strlen(sendData), 0);
		

		cout << "�������ݰ�" << endl;
		char recData[255];
		int ret = recv(sclient, recData, 255, 0);
		if (ret > 0)
		{
			//recData[ret] = 0x00;
			printf("dddddddd\n");
			printf(recData);
			byte stemp[16];
			for (int i = 0; i < 16; i++)
				stemp[i + 16] = recData[i+16];
		
			

			Analysis((byte*)recData);
			char m[128];
			for (int i = 0; i < 128; i++)
			{
				m[i] = recData[i + 17];
			}
			m801CInit(m);

		}



	//	if ()

		//send(sclient, (char*)m801C, 164, 0);
		cout << "over or not?  1:over" << endl;
		int n;
		if (cin >> n)
		{
			char exitBuf[2] = {'1'};
			send(sclient, exitBuf, 1, 0);
			cout << "exitBuf:" << exitBuf << endl;
			//getchar();
			break;
		}
	}
	cout << "OVER" << endl;
	
	Sleep(2);
	closesocket(sclient);
	WSACleanup();
	return 0;
}


void Analysis(byte M[])
{
	//head
	printf("��Ϣͷ��");
	for (int i = 0; i <16; i++)
		printf("%02x:", M[i]);

	printf("\n");
	printf("���ݣ�");
	printf("body: ");
	//msg body
	for (int i = 16; i < 144; i++)
		printf("%02x��", M[i]);


	//To String
	printf("\n");
	//cout << "allMsg:%c" << M << endl;
	printf("����allMsg:%s", M);


	printf("++++++++++++++++++++++++++++%d\n", HexToInt(M, 0));
	printf("++++++++++++++++++++++++++++%d\n", HexToInt(M, 4));
	printf("++++++++++++++++++++++++++++%d\n", HexToInt(M, 8));

	printf("++++++++++++++++++++++++++++%d\n", HexToInt(M, 12));
}



void Analysis801C()
{
	//Analysis();//
	for (int i = 0; i < 16; i++)
	{
	}


}

void GetTime(char time[])
{
	//��������
	SYSTEMTIME sys_time;
	//BUG
	//������ֵ����Ϊ����ʱ��
	GetLocalTime(&sys_time);

	//char time[18];//����ʱ���ַ���
	sprintf(time, "%04d", sys_time.wYear);//year
	sprintf(time + 4, "%02d", sys_time.wMonth);
	sprintf(time + 6, "%02d", sys_time.wDay);
	sprintf(time + 8, "%02d", sys_time.wHour);
	sprintf(time + 10, "%02d", sys_time.wMinute);
	sprintf(time + 12, "%02d", sys_time.wSecond);
	sprintf(time + 14, "%s", "000");
	cout << "ʱ��:" << time << endl;
	cout << "ʱ��:" << (byte*)time << endl;
	
}