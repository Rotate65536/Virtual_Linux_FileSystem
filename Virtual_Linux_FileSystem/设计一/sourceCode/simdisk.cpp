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

int main(int argc, char* argv[])
{
	emptyDirBlockIndex = DATA_AREA_ADDR;
	emptyFileBlockIndex = DATA_AREA_ADDR + BLOCK_NUM_EACH_GROUP * sizeof(emptyBlock);
	system("title 模拟文件系统");
	int i, j;
	loadDisk();
	while (!cmd_login());
	const int lengthCmd = MAX_COMMAND_LENGTH + MAX_PATH_LENGTH * 2 + 2;
	char strCommandLine[lengthCmd],  //整行命令
		command[MAX_COMMAND_LENGTH],  //命令类型
		arg1[MAX_PATH_LENGTH],  //参数1
		arg2[MAX_PATH_LENGTH];  //参数2
	stringstream streamCmd;
	cmd_ver();
	cout << currentPath << "/" << "@" << presentUser.name << ">";
	while (1)
	{
		cin.getline(strCommandLine, lengthCmd);
		streamCmd << strCommandLine;
		streamCmd >> command >> arg1 >> arg2;
		if (command[0] == '\0')
		{
			streamCmd.clear();
			continue;
		}
		if (!streamCmd.eof())
		{
			cout << "参数过多！\n";
			continue;
		}
		streamCmd.clear();
		execute_cmd(command, arg1, arg2);
		cout << currentPath << "/" << "@" << presentUser.name << ">";
	}
	return 0;
}