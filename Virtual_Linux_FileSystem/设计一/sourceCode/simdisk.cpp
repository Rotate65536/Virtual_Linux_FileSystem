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

int main(int argc, char* argv[])
{
	emptyDirBlockIndex = DATA_AREA_ADDR;
	emptyFileBlockIndex = DATA_AREA_ADDR + BLOCK_NUM_EACH_GROUP * sizeof(emptyBlock);
	system("title ģ���ļ�ϵͳ");
	int i, j;
	loadDisk();
	while (!cmd_login());
	const int lengthCmd = MAX_COMMAND_LENGTH + MAX_PATH_LENGTH * 2 + 2;
	char strCommandLine[lengthCmd],  //��������
		command[MAX_COMMAND_LENGTH],  //��������
		arg1[MAX_PATH_LENGTH],  //����1
		arg2[MAX_PATH_LENGTH];  //����2
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
			cout << "�������࣡\n";
			continue;
		}
		streamCmd.clear();
		execute_cmd(command, arg1, arg2);
		cout << currentPath << "/" << "@" << presentUser.name << ">";
	}
	return 0;
}