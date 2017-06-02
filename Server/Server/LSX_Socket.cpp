#include "stdafx.h"
#include <WinSock2.h>
#include "LSX_Socket.h"
#include <string>
#include <iostream>
#include <sstream>

#include "LSX_md5.h"

using namespace std;


char* LSX_message_control::GetTime()
{
	//��������
	SYSTEMTIME sys_time;
//BUG
	//������ֵ����Ϊ����ʱ��
	GetLocalTime(&sys_time);
	
	char time[18];//����ʱ���ַ���
	sprintf(time, "%04d", sys_time.wYear);//year
	sprintf(time + 4, "%02d", sys_time.wMonth);
	sprintf(time + 6, "%02d", sys_time.wDay);
	sprintf(time + 8, "%02d", sys_time.wHour);
	sprintf(time + 10, "%02d", sys_time.wMinute);
	sprintf(time + 12, "%02d", sys_time.wSecond);
	sprintf(time + 14, "%s", "000");
	cout << "ʱ��:" << time << endl;
	cout << "ʱ��:" << (byte*)time << endl;
	return time;//����ʱ����ַ���
}

LSX_message_control::LSX_message_control(string clear_text, int id, int type)//�������ڲ��Լ��ɽӿ�
{
	//************************************MSG******************************
	/*800 Command*/
	mInfo.mC800Body.m800Body_length = clear_text.length();
	mInfo.mC800Body.m800Body = clear_text;
	cout << "Body_length:" << mInfo.mC800Body.m800Body_length << endl;
	int msg_length = InitMsgHead(id, type);
	AllMsg(msg_length);
	
	/*801 Command*/

	/*801 Result*/
	m801RInit();
	test();
	
	/*802*/
	//m802CInit();
	/*803...*/




}
/*
//��>>�� ժҪ������skey��800����   //�ͻ��˹���
void LSX_message_control::m801CInit()
{
	mInfo.mC801Body.m801_summary_length =16;//16byte
	mInfo.mC801Body.m801serverRemark;
	mInfo.mC801Body.m801Body_length;
	mInfo.mC801Body.m801Body;
}
*/

//��>>�� ժҪ ��ͨ��801 command�е����� ʹ��rkey����
void LSX_message_control::m801RInit()
{
	MD5 MD5("abcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefgh");
	string result = MD5.md5();
	cout << "abc MD5 is:" << result << endl;
	mInfo.mR801Body.time_length = 17;////17Byte
	strcpy((char*)(mInfo.mC802Body.start_time), GetTime());//��ǰʱ�� ���͸� ��
	mInfo.mR801Body.remark_length = 16;//16Byte
	strcpy((char*)(mInfo.mR801Body.voiceRemark), (char*)MD5.getResult());//�������16����
	
	char buf[33];
	for (int i = 0; i<16; i++)
		sprintf(buf + i * 2, "%02x", mInfo.mR801Body.voiceRemark[i]);//��16Byte��16���Ƶ�ժҪתΪ�ַ������
	buf[32] = 0; //��16����ת�ַ����

		cout << "��������֤md5��" << buf<< endl;


		cout << "+++++++++++++++++++++++++++++++++++++++++++" << endl;
		strcpy((char*)(mInfo.mC802Body.start_time), GetTime());//start serach time //17byte
		cout << "��ʼʱ�䣺" << mInfo.mC802Body.start_time << endl;
}


//������>>����
void LSX_message_control:: m802CInit(int startTimeLength,char startTime[],int stopTimeLength,char stopTime[])
{
	//�û���ѯĳһʱ����е�Ϣ��test
	mInfo.mC802Body.channel = 1;
	strcpy((char*)(mInfo.mC802Body.start_time), GetTime());//start serach time //17byte
	cout << "��ʼʱ�䣺" << mInfo.mC802Body.start_time << endl;
	mInfo.mC802Body.start_time_length = startTimeLength;
	mInfo.mC802Body.stop_time_length=stopTimeLength;
	strcpy((char*)(mInfo.mC802Body.start_time), GetTime());//over serach time //17byte
	mInfo.mC802Body.stop_time;


	Msg_Head m802Head;
	m802Head.msg_id = 802;
	m802Head.msg_type = 0;
	m802Head.msg_length = 58;

	



}


//��>>��  ���ط�������ѯ��Ϣ Result
/*
void LSX_message_control::m802RInit()
{
	mInfo.mR802Body.record_number;
	mInfo.mR802Body.start1_time;
	mInfo.mR802Body.record1_length;	
}
*/


//����>>������
void LSX_message_control::m803CInit()
{
	//NO body
}

//������>>����  ��ȡ����ĳһ��id��������¼��Ϣ
void LSX_message_control::m804CInit(int id,int second)
{
	mInfo.mC804Body.recoder_id=id;//���� 1-100
	mInfo.mC804Body.time_offset = second;//��
}

//����>>������  ֪ͨ��������ѯ����id������¼�Ľ��
void LSX_message_control::m805CInit()
{
	mInfo.mC805Body.result;
	mInfo.mC805Body.file_length;

}

//����>>������  ����������Ϣ
void LSX_message_control::m806CInit()
{
	
}


//����>>������ ֪ͨ����������
void LSX_message_control::m807CInit()
{
	mInfo.mC807Body.data;//1 over 2 exception over
}


//������>>���� ֪ͨ������¼��ֹͣ������������
void LSX_message_control::m808CInit()
{

}


//������>>����	������
void LSX_message_control::m809CInit(){}



int LSX_message_control::InitMsgHead(int id,int type)
{
	mInfo.mHead.msg_id= id;
	mInfo.mHead.msg_type =type;
	mInfo.mHead.msg_length = sizeof(mInfo.mHead.msg_id) + sizeof(mInfo.mHead.msg_type) + sizeof(mInfo.mHead.msg_length) + mInfo.mC800Body.m800Body_length + sizeof(mInfo.mC800Body.m800Body_length);

	cout << "allMsg_length:" << mInfo.mHead.msg_length << endl;
	return mInfo.mHead.msg_length;
}

string LSX_message_control::IntToString(int x)
{
	string str;
	std::stringstream tmp;
	tmp << x;
	tmp >> str;
	return str;
}

int LSX_message_control:: StringToInt(string x)
{
	return (atoi(x.c_str()));
}





void LSX_message_control::AllMsg(int x)
{
	byte byte_src[4];//transform 
	memset(M,0,sizeof(M));
	IntToHex(byte_src,x);//144  00��00��00��90
	for (int i = 0; i < 4; i++)
	{
		M[i] = byte_src[i];
		printf("KKK:%02x\n", M[i]);
	}
		

	IntToHex(byte_src,mInfo.mHead.msg_type);//0
	for (int i = 4; i < 8; i++)
	{
		M[i] = byte_src[i-4];
		printf("KKK:%02x\n", M[i]);
	}
		

	IntToHex(byte_src,mInfo.mHead.msg_id);//800   00��00��00��03��20
	for (int i = 8; i < 12; i++)
	{
		M[i] = byte_src[i-8];
		printf("KKK:%02x\n", M[i]);
	}

	IntToHex(byte_src,mInfo.mC800Body.m800Body_length);//128   00:00:00:80
	for (int i = 12; i < 16; i++)
	{
		M[i] = byte_src[i-12];
		printf("KKK:%02x\n", M[i]);
	}

	
	//int X = mInfo.m800Body.m800Body.length();
	char buf[129];//128+1

	//strcpy(letter, word.c_str());
	strcpy(buf, mInfo.mC800Body.m800Body.c_str());

	cout << "buf:" <<buf<< endl;


	for (int i = 16; i < 144; i++)
	{
		M[i] = buf[i-16];
	}
	//head
	for (int i =0; i <16;i++)
	printf("%02x:",M[i]);

	printf("\n");
	printf("body: ");
	//msg body
	for (int i = 16; i < 144; i++)
		printf("%02x��",M[i]);


	//To String
	
	//cout << "allMsg:%c" << M << endl;
	printf("allMsg:%s",M);

	
	printf("++++++++++++++++++++++++++++%d\n", HexToInt(M, 0));
	printf("++++++++++++++++++++++++++++%d\n", HexToInt(M, 4));
	printf("++++++++++++++++++++++++++++%d\n", HexToInt(M, 8));

	printf("++++++++++++++++++++++++++++%d\n", HexToInt(M, 12));
	
}

void  LSX_message_control::IntToHex(byte byte_src[], int value)//big  ���
{

	//byte byte_src[4];//transform 
	/*int to hex for trasform*/
	memset(byte_src,0,4);
	byte_src[0] = (byte)((value & 0xFF000000) >> 24);
	byte_src[1] = (byte)((value & 0x00FF0000) >> 16);
	byte_src[2] = (byte)((value & 0x0000FF00) >> 8);
	byte_src[3] = (byte)((value & 0x000000FF));

	for (int i = 0; i < 4;i++)
	printf("%02x:%d||",byte_src[i],byte_src[i]);
}

int LSX_message_control::HexToInt(byte arry[], int postion)//big ���
{
	int value;
	value = (int)((arry[postion+3] & 0xFF)
		| ((arry[postion + 2] << 8) & 0xFF00)
		| ((arry[postion + 1] << 16) & 0xFF0000)
		| ((arry[postion + 0] << 24) & 0xFF000000));
	return value;
}




void LSX_message_control::test()
{
	
	byte byte_src[4];
	IntToHex(byte_src, 58);
	for (int i = 0; i < 4; i++)
	{
		m802C[i] = byte_src[i];
		printf("KKK:%02x\n", m802C[i]);
	}

	IntToHex(byte_src, 0);
	for (int i = 4; i < 8; i++)
	{
		m802C[i] = byte_src[i-4];
		printf("KKK:%02x\n", m802C[i]);
	}

	IntToHex(byte_src, 802);
	for (int i = 8; i < 12; i++)
	{
		m802C[i] = byte_src[i-8];
		printf("KKK:%02x\n", m802C[i]);
	}

	IntToHex(byte_src, 1);//ͨ��
	for (int i = 12; i < 16; i++)
	{
		m802C[i] = byte_src[i-12];
		printf("KKK:%02x\n", m802C[i]);
	}

	IntToHex(byte_src, 17);//����0x11
	for (int i = 16; i < 20; i++)
	{
		m802C[i] = byte_src[i-16];
		printf("KKK:%02x\n", m802C[i]);
	}
	//11111BUG

	for (int i = 0; i < 17; i++)
		sprintf((char*)m802C + 20 + i, "%c", (mInfo.mC802Body.start_time[i]));

	IntToHex(byte_src, 17);
	for (int i = 37; i < 41; i++)
	{
		m802C[i] = byte_src[i-37];
		printf("************KKK:%02x\n", m802C[i]);
	}

	for (int i = 0; i<17; i++)
		sprintf((char *)m802C + 41 + i, "%c", (mInfo.mC802Body.start_time[i]));

	printf("802:\n");
	for (int i = 0; i < 58; i++)
	{
		printf("%02x:", m802C[i]);
	}

}
void LSX_message_control::MD5Init( int &a)
{
	//MD5 MD5("abc");
	//string result = MD5.md5();
	//cout << "abc MD5 is:" << result << endl;
	a = 100;
	/*
	this->md5ToKey = (char*)MD5.getResult();
	cout << "NNNNNNNNNNNN:" << md5ToKey << endl;
	*/
	//	IntToHex(86987456);

}


void LSX_socket_control::LSX_AddressServer()
{
	//��IP�Ͷ˿�
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	/*
	if (bind(lSocket, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}
	*/
	
}




int LSX_socket_control::LSX_WSA_Init()
{
	//��ʼ��WSA
	//WSADATA wsaData;
	sockVersion = MAKEWORD(2, 2);
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}
}
int LSX_socket_control::LSX_Socket()
{
	//�����׽���
	lSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (lSocket == INVALID_SOCKET)
	{
		printf("socket error !");
		return 0;
	}
}

void LSX_socket_control::LSX_Bind()
{
	//�󶨷�������ַ
	if (bind(lSocket, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
	}
}

int LSX_socket_control::LSX_Listen()
{
	//��ʼ����
	if (listen(lSocket, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
		return 0;
	}
}

int LSX_socket_control::LSX_Accept()
{
	sClient = accept(lSocket, (SOCKADDR *)&(remoteAddr), &nAddrlen);
	if (sClient == INVALID_SOCKET)
	{
		printf("accept error !");
	}
	return 0;
}

void LSX_message_control::LSX_ReceiveData()
{
	int ret = recv(socketControl.sClient,revData, 255, 0);
	if (ret > 0)
	{
		revData[ret] = 0x00;
		printf(revData);
	}

	for (int i = 0; i < 144; i++)
	{
		printf("%02x:", (byte)revData[i]);
		//cout << "=================" << tmp[i] << endl;
	}

	printf("\n");
	printf("���룺");
	printf("%d", HexToInt((byte*)revData, 0));
	printf("%d", HexToInt((byte*)revData, 4));
	printf("%d", HexToInt((byte*)revData, 8));
	printf("%d", HexToInt((byte*)revData, 12));


	/*
	for (int i = 16; i < 144; i++)
	{
		dbuf[i - 16] = revData[i];
		printf("%c", revData[i]);
	}

	cout << "++++:" << dbuf << endl;
	*/

}

void LSX_message_control::LSX_SendCmd()
{
	//send(socketControl.sClient, (char*)m802C, 58, 0);//802
	send(socketControl.sClient, (char*)M, 144, 0);
}

void LSX_socket_control::LSX_Close()
{
	closesocket(sClient);
}
void LSX_socket_control::LSX_WSA_Clean()
{
	WSACleanup();
}

