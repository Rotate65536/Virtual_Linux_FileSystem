#include"head.h"

int emptyDirBlockIndex;  //��ǰ��һ�����е�Ŀ¼���̿�λ��
int emptyFileBlockIndex;

block_group blockGroup[BLOCK_GROUPS_NUM];  //���̿���
i_node inodeTable[BLOCK_GROUPS_NUM][INODES_NUM_EACH_GROUP];  //i����
bitmap inodeBitmap[BLOCK_GROUPS_NUM][INODES_NUM_EACH_GROUP];  //i���λͼ
bitmap blockBitmap[BLOCK_GROUPS_NUM][BLOCK_NUM_EACH_GROUP];  //����λͼ

vector<user> users;  //��¼�û�
user presentUser;  //��¼��ǰ�û�

fstream fsVirtualDisk;

int currentPathDir;  //��ǰĿ¼
vector<dir> vecDir;  //��¼����Ŀ¼
vector<file> vecFile;  //��¼�����ļ�
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

	//���ݾ����simdisk��shell֮��Ĺ����ڴ�
	hMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS, 0,
		(LPCWSTR)szName                   // �����ڴ�����
	);

	//����ָ�����ڴ��ָ��
	pBuf = (ShareMemory*)MapViewOfFile(
		hMapFile,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		sizeof(ShareMemory)
	);
	pBuf->toSimdisk = false;

	//�������simdisk֮��Ĺ����ڴ���
	hMapFile_RW = OpenFileMapping(
		FILE_MAP_ALL_ACCESS, 0,
		(LPCWSTR)rwName                   // �����ڴ�����
	);

	//����ָ�����ڴ��ָ��
	pBuf_RW = (ShareMemoryRW*)MapViewOfFile(
		hMapFile_RW,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		sizeof(ShareMemoryRW)
	);

	emptyDirBlockIndex = DATA_AREA_ADDR;
	emptyFileBlockIndex = DATA_AREA_ADDR + BLOCK_NUM_EACH_GROUP * sizeof(emptyBlock);
	//system("title ģ���ļ�ϵͳ");            
	int i, j;
	loadDisk();
	while (!cmd_login());
	const int lengthCmd = MAX_COMMAND_LENGTH + MAX_PATH_LENGTH * 2 + 2;
	char strCommandLine[lengthCmd],  //��������
		command[MAX_COMMAND_LENGTH],  //��������
		arg1[MAX_PATH_LENGTH] = "",  //����1
		arg2[MAX_PATH_LENGTH] = "";  //����2
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