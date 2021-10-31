#include "head.h"
void shellOutput(string strToShell)
{
	strcpy(pBuf->contents, strToShell.c_str());  //将输出内容存放到共享内存中
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
	char cd[MAX_COMMAND_LENGTH];  //命令类型
	char a1[MAX_PATH_LENGTH];  //参数1
	char a2[MAX_PATH_LENGTH];  //参数2
	streamCmd << pBuf->contents;
	pBuf->clear();
	streamCmd >> cd >> a1 >> a2;
	//在simdisk终端输出debug信息
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
		shellOutput("写操作......共享内存被占用！\n");
		Sleep(10000);
	}
	pBuf_RW->occupied = true;
	bool ifWriteable = true;
	//申请写该文件
	if (ifWrite == true)
	{
		//首先判断该i节点是否正在被写
		if (pBuf_RW->writingInode[indexGroup][index] == true)
		{
			ifWriteable = false;
		}
		//然后判断该i节点是否正在被读
		if (pBuf_RW->readingInode[indexGroup][index] > 0 && ifWriteable == true)
		{
			ifWriteable = false;
		}
		if (ifWriteable)
		{
			//互斥
			cout << "正在写......i节点号：" << indexGroup << " " << index << endl;
			//告诉共享内存正在写i节点
			pBuf_RW->writingInode[indexGroup][index] = true;
		}
		else  //查看为什么不能写
		{
			if (inodeTable[indexGroup][index].type == typeDIRECTORY)
			{
				if (pBuf_RW->readingInode[indexGroup][index] > 0)
					shellOutput("该目录正在被读！\n");
				else
					shellOutput("该目录正在被写！\n");
			}
			else
				if (pBuf_RW->readingInode[indexGroup][index] > 0)
					shellOutput("该文件正在被读！\n");
				else
					shellOutput("该文件正在被写！\n");
		}
	}
	//取消访问文件
	if (ifWrite == false)
	{
		pBuf_RW->writingInode[indexGroup][index] = false;
	}
	pBuf_RW->occupied = false;
	return ifWriteable;
}
/*
1. 写文件
if(writeInode(indexGroup, index, true) == false)
	return;
2. 写完文件
writeInode(indexGroup, index, false)
*/

bool readInode(int indexGroup, int index, bool ifRead)
{
	while (pBuf_RW->occupied == true)
	{
		shellOutput("读操作......共享内存被占用！\n");
		Sleep(10000);
	}
	pBuf_RW->occupied = true;
	bool ifReadable = true;
	//申请读该文件
	if (ifRead == true)
	{
		//首先判断该i节点是否正在被写
		if (pBuf_RW->writingInode[indexGroup][index] == true)
			ifReadable = false;
		//共享读
		if (ifReadable)
		{
			cout << "正在读......i节点号：" << indexGroup << " " << index << endl;
			//互斥
			//告诉共享内存正在读i节点
			pBuf_RW->readingInode[indexGroup][index] += 1;
		}
		else
		{
			if (inodeTable[indexGroup][index].type == typeDIRECTORY)
				shellOutput("该目录被占用！\n");
			else
				shellOutput("该文件被占用！\n");
		}
	}
	//读完该文件
	else
	{
		pBuf_RW->readingInode[indexGroup][index] -= 1;
	}
	pBuf_RW->occupied = false;
	return ifReadable;
}
/*
1. 读文件
if(readInode(indexGroup, index, true) == false)
	return;
else
	readInode(indexGroup', index', false)
2. 读完文件
readInode(indexGroup, index, false)
*/