#include"head.h"

int emptyDirBlockIndex;  //当前第一个空闲的目录磁盘块位置
int emptyFileBlockIndex;

block_group blockGroup[BLOCK_GROUPS_NUM];  //磁盘块组
i_node inodeTable[BLOCK_GROUPS_NUM][INODES_NUM_EACH_GROUP];  //i结点表
bitmap inodeBitmap[BLOCK_GROUPS_NUM][INODES_NUM_EACH_GROUP];  //i结点位图
bitmap blockBitmap[BLOCK_GROUPS_NUM][BLOCK_NUM_EACH_GROUP];  //磁盘位图

vector<user> users;  //记录用户
user presentUser;  //记录当前用户

fstream fsVirtualDisk;

int currentPathDir;  //当前目录
vector<dir> vecDir;  //记录已有目录
vector<file> vecFile;  //记录已有文件
char currentPath[MAX_PATH_LENGTH];

char rwName[] = "ShareMemoryRW";
char szName[] = "ShareMemory2";
HANDLE hMapFile;
ShareMemory* pBuf;
HANDLE hMapFile_RW;
ShareMemoryRW* pBuf_RW;

int main(int argc, char* argv[])
{
	system("title simdisk2");
	string strShellOutput;

	//根据句柄打开simdisk和shell之间的共享内存
	hMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS, 0,
		(LPCWSTR)szName                   // 共享内存名称
	);

	//设置指向共享内存的指针
	pBuf = (ShareMemory*)MapViewOfFile(
		hMapFile,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		sizeof(ShareMemory)
	);
	pBuf->toSimdisk = false;

	//创建或打开simdisk之间的共享内存句柄
	hMapFile_RW = OpenFileMapping(
		FILE_MAP_ALL_ACCESS, 0,
		(LPCWSTR)rwName                   // 共享内存名称
	);

	//设置指向共享内存的指针
	pBuf_RW = (ShareMemoryRW*)MapViewOfFile(
		hMapFile_RW,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		sizeof(ShareMemoryRW)
	);

	emptyDirBlockIndex = DATA_AREA_ADDR;
	emptyFileBlockIndex = DATA_AREA_ADDR + BLOCK_NUM_EACH_GROUP * sizeof(emptyBlock);
	//system("title 模拟文件系统");            
	int i, j;
	loadDisk();
	while (!cmd_login());
	const int lengthCmd = MAX_COMMAND_LENGTH + MAX_PATH_LENGTH * 2 + 2;
	char strCommandLine[lengthCmd],  //整行命令
		command[MAX_COMMAND_LENGTH],  //命令类型
		arg1[MAX_PATH_LENGTH] = "",  //参数1
		arg2[MAX_PATH_LENGTH] = "";  //参数2
	stringstream streamCmd;
	cmd_ver();
	cout << currentPath << "/" << "@" << presentUser.name << ">";
	strShellOutput = string(currentPath) + "/" + "@" + string(presentUser.name) + ">";
	shellOutput(strShellOutput);
	while (1)
	{
		strcpy(command, "");
		strcpy(arg1, "");
		strcpy(arg2, "");
		shellInput(command, arg1, arg2);
		cout << "cd: " << command << endl;
		cout << "a1: " << arg1 << endl;
		cout << "a2: " << arg2 << endl;
		if (command[0] == '\0')
		{
			continue;
		}
		execute_cmd(command, arg1, arg2);
		cout << currentPath << "/" << "@" << presentUser.name << ">";
		strShellOutput = string(currentPath) + "/" + "@" + string(presentUser.name) + ">";
		shellOutput(strShellOutput);
	}
	return 0;
}