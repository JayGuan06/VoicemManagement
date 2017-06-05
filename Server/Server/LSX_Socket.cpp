#include "stdafx.h"
#include <WinSock2.h>
#include "LSX_Socket.h"
#include <string>
#include <iostream>
#include <sstream>

#include "LSX_md5.h"

using namespace std;


char* LSX_message_control::GetTime(char time[])
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
	m800CInit(msg_length);
	
	/*
	m801CInit();
	m801RInit();

	m802CInit();
	m803CInit();
	m804CInit(1,15);
	m805CInit();
	//m806CInit();
	m807CInit();
	m808CInit();
	m809CInit();

	*/


}


void LSX_message_control::m800CInit(int x)
{
	byte byte_src[4];//transform 
	memset(M, 0, sizeof(M));
	/*IntToHex(byte_src, x);//144  00��00��00��90
	for (int i = 0; i < 4; i++)
	{
		M[i] = byte_src[i];
		printf("KKK:%02x\n", M[i]);
	}


	IntToHex(byte_src, mInfo.mHead.msg_type);//0
	for (int i = 4; i < 8; i++)
	{
		M[i] = byte_src[i - 4];
		printf("KKK:%02x\n", M[i]);
	}


	IntToHex(byte_src, mInfo.mHead.msg_id);//800   00��00��00��03��20
	for (int i = 8; i < 12; i++)
	{
		M[i] = byte_src[i - 8];
		printf("KKK:%02x\n", M[i]);
	}//��Ϣͷ
	*/
	for (int i = 0; i < 12; i++)
	{
		M[i] = Command800head[i];
	}
	IntToHex(byte_src, mInfo.mC800Body.m800Body_length);//128   00:00:00:80
	for (int i = 12; i < 16; i++)
	{
		M[i] = byte_src[i - 12];
		printf("KKK:%02x\n", M[i]);
	}


	//int X = mInfo.m800Body.m800Body.length();
	char buf[129];//128+1

	//strcpy(letter, word.c_str());
	strcpy(buf, mInfo.mC800Body.m800Body.c_str());

	cout << "ԭ����buf:" << buf << endl;


	for (int i = 16; i < 144; i++)
	{
		M[i] = buf[i - 16];
	}

	Analysis();

}



void LSX_message_control::m801CInit()
{
	MD5 MD5("abcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefgh");
	string result = MD5.md5();
	cout << "abc MD5 is:" << result << endl;
	memset(M,0,1024);
	for (int i = 0; i < 12; i++)
	{
		M[i] = Command801head[i];//801C��Ϣͷ
	}

	byte szlength[] = {0x00,0x00,0x00,0x10};
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
		printf("%c",M[i+32]);
	}
	printf("\n");
	Analysis();
}

//��>>�� ժҪ ��ͨ��801 command�е����� ʹ��rkey����
void LSX_message_control::m801RInit()
{
	MD5 MD5("abcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefghijabcdefgh");
	string result = MD5.md5();
	cout << "abc MD5 is:" << result << endl;
	memset(M,0,1024);
	for (int i = 0; i < 12; i++)
	{
		M[i] = Result801head[i];
	}//801R ͷ��Ϣ

	byte time[] = {0x00,0x00,0x00,0x11};
	for (int i = 0; i < 4; i++) //ʱ�䳤��
	{
		M[i+12] = time[i];
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


//������>>����
void LSX_message_control:: m802CInit()
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
		M[i+12] = 0x00;
	}
	M[15] = 0x01;//ͨ����  �ɸ���

	for (int i = 0; i < 3; i++)
	{
		M[i + 16] = 0x00;
	}
	M[20] = 0x11;
	char startTime[18];
	GetTime(startTime);
	for (int i = 0; i < 17;i++)
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
	memset(M, 0, 1024);
	for (int i = 0; i < 12;i++)
	M[i] = Command803head[i];


	printf("Դ���ݣ�");
	printf("\n");
	Analysis();
}

//������>>����  ��ȡ����ĳһ��id��������¼��Ϣ
void LSX_message_control::m804CInit(int id,int second)
{
	
	memset(M, 0, 1024);
	for (int i = 0; i < 12; i++)
		M[i] = Command804head[i];//ͷ��Ϣ
	
	byte byte_src[4];
	IntToHex(byte_src,id);//Ҫ��ѯһ����id
	for (int i = 0; i < 4; i++)
	{
		M[i+12] = byte_src[i];
	}

	IntToHex(byte_src,second);//һ����¼��ʱ�� ��λΪ˵

	for (int i = 0; i < 4; i++)
	{
		M[i + 16] = byte_src[i];
	}

	printf("Դ���ݣ�");
	printf("\n");
	Analysis();

}

//����>>������  ֪ͨ��������ѯ����id������¼�Ľ��
void LSX_message_control::m805CInit()
{
	memset(M, 0, 1024);
	for (int i = 0; i < 12; i++)
		M[i] = Command805head[i];
	for (int i = 0; i < 3; i++)
		M[i+12] = 0x00;
	M[15] = 0x01;//������ȷ���Ǵ���  һ������ֵ 1����ȷ2������

	byte byte_src[4];
	IntToHex(byte_src,12);//���ļ�����ת��Ϊ16���ƴ洢
	for (int i = 0; i < 4; i++)
	{
		M[i+16] = byte_src[i];
	}

	printf("Դ���ݣ�");
	printf("\n");
	Analysis();



}

//����>>������  ����������Ϣ
void LSX_message_control::m806CInit()
{
	memset(M, 0, 1024);
	for (int i = 0; i < 12; i++)
		M[i] = Command803head[i];
	byte byte_src[4];
	IntToHex(byte_src,50);//�������ݳ��� byte
	//for ()
}


//����>>������ ֪ͨ����������
void LSX_message_control::m807CInit()
{
	memset(M, 0, 1024);
	for (int i = 0; i < 12; i++)
		M[i] = Command807head[i];
	for (int i = 0; i < 3; i++)
		M[i + 12] = 0x00;
	M[15] = 0x01;// 1������������  2:�쳣��ֹ
	//1 over 2 exception over

	printf("Դ���ݣ�");
	printf("\n");
	Analysis();

}


//������>>���� ֪ͨ������¼��ֹͣ������������
void LSX_message_control::m808CInit()
{
	memset(M, 0, 1024);
	for (int i = 0; i < 12; i++)
		M[i] = Command808head[i];

	printf("Դ���ݣ�");
	printf("\n");
	Analysis();

}


//������>>����	������
void LSX_message_control::m809CInit()
{
	memset(M, 0, 1024);
	for (int i = 0; i < 12; i++)
		M[i] = Command809head[i];

	printf("Դ���ݣ�");
	printf("\n");
	Analysis();
}



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
	//������Ϣͷ
	printf("\n");
	printf("���룺");
	printf("%d", HexToInt((byte*)revData, 0));
	printf("%d", HexToInt((byte*)revData, 4));
	printf("%d", HexToInt((byte*)revData, 8));
	printf("%d", HexToInt((byte*)revData, 12));

	

	//����body
	/*
	char dbuf[33];
	for (int i = 0; i<16; i++)
		sprintf(dbuf + i * 2, "%02x", revData[i+16]);//��16Byte��16���Ƶ�ժҪתΪ�ַ������
	dbuf[32] = 0;

	*/

	for (int i = 16; i < 36; i++)
	{
		printf("%02x:", revData[i]);
	}

	//802
	for (int i = 36; i < 165; i++)
		printf("%c:",revData[i]);
	//cout << "++++:" << dbuf << endl;
	

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


void LSX_message_control::Analysis()
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