#include "head.h"
void shellOutput(string strToShell)
{
	strcpy(pBuf->contents, strToShell.c_str());  //��������ݴ�ŵ������ڴ���
	pBuf->shellOutput = true;
	Sleep(1000);
}

void shellInput(char command[MAX_COMMAND_LENGTH], char arg1[MAX_PATH_LENGTH], char arg2[MAX_PATH_LENGTH])
{
	pBuf->toShell = true;
	while (1)
	{
		if (pBuf->toSimdisk == true)
			break;
	}
	stringstream streamCmd;
	char cd[MAX_COMMAND_LENGTH];  //��������
	char a1[MAX_PATH_LENGTH];  //����1
	char a2[MAX_PATH_LENGTH];  //����2
	streamCmd << pBuf->contents;
	pBuf->clear();
	streamCmd >> cd >> a1 >> a2;
	//��simdisk�ն����debug��Ϣ
	cout << "cd: " << cd << endl;
	if (a1[0] != '\0')
	{
		cout << "a1: " << a1 << endl;
		strcpy(arg1, a1);
	}
	if (a2[0] != '\0')
	{
		strcpy(arg2, a2);
		cout << "a2: " << a2 << endl;
	}
	if (cd[0] != '\0')
		strcpy(command, cd);
	pBuf->toSimdisk = false;
}