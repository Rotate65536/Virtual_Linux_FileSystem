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

bool writeInode(int indexGroup, int index, bool ifWrite)
{
	while (pBuf_RW->occupied == true)
	{
		shellOutput("д����......�����ڴ汻ռ�ã�\n");
		Sleep(10000);
	}
	pBuf_RW->occupied = true;
	bool ifWriteable = true;
	//����д���ļ�
	if (ifWrite == true)
	{
		//�����жϸ�i�ڵ��Ƿ����ڱ�д
		if (pBuf_RW->writingInode[indexGroup][index] == true)
		{
			ifWriteable = false;
		}
		//Ȼ���жϸ�i�ڵ��Ƿ����ڱ���
		if (pBuf_RW->readingInode[indexGroup][index] > 0 && ifWriteable == true)
		{
			ifWriteable = false;
		}
		if (ifWriteable)
		{
			//����
			cout << "����д......i�ڵ�ţ�" << indexGroup << " " << index << endl;
			//���߹����ڴ�����дi�ڵ�
			pBuf_RW->writingInode[indexGroup][index] = true;
		}
		else  //�鿴Ϊʲô����д
		{
			if (inodeTable[indexGroup][index].type == typeDIRECTORY)
			{
				if (pBuf_RW->readingInode[indexGroup][index] > 0)
					shellOutput("��Ŀ¼���ڱ�����\n");
				else
					shellOutput("��Ŀ¼���ڱ�д��\n");
			}
			else
				if (pBuf_RW->readingInode[indexGroup][index] > 0)
					shellOutput("���ļ����ڱ�����\n");
				else
					shellOutput("���ļ����ڱ�д��\n");
		}
	}
	//ȡ�������ļ�
	if (ifWrite == false)
	{
		pBuf_RW->writingInode[indexGroup][index] = false;
	}
	pBuf_RW->occupied = false;
	return ifWriteable;
}
/*
1. д�ļ�
if(writeInode(indexGroup, index, true) == false)
	return;
2. д���ļ�
writeInode(indexGroup, index, false)
*/

bool readInode(int indexGroup, int index, bool ifRead)
{
	while (pBuf_RW->occupied == true)
	{
		shellOutput("������......�����ڴ汻ռ�ã�\n");
		Sleep(10000);
	}
	pBuf_RW->occupied = true;
	bool ifReadable = true;
	//��������ļ�
	if (ifRead == true)
	{
		//�����жϸ�i�ڵ��Ƿ����ڱ�д
		if (pBuf_RW->writingInode[indexGroup][index] == true)
			ifReadable = false;
		//�����
		if (ifReadable)
		{
			cout << "���ڶ�......i�ڵ�ţ�" << indexGroup << " " << index << endl;
			//����
			//���߹����ڴ����ڶ�i�ڵ�
			pBuf_RW->readingInode[indexGroup][index] += 1;
		}
		else
		{
			if (inodeTable[indexGroup][index].type == typeDIRECTORY)
				shellOutput("��Ŀ¼��ռ�ã�\n");
			else
				shellOutput("���ļ���ռ�ã�\n");
		}
	}
	//������ļ�
	else
	{
		pBuf_RW->readingInode[indexGroup][index] -= 1;
	}
	pBuf_RW->occupied = false;
	return ifReadable;
}
/*
1. ���ļ�
if(readInode(indexGroup, index, true) == false)
	return;
else
	readInode(indexGroup', index', false)
2. �����ļ�
readInode(indexGroup, index, false)
*/