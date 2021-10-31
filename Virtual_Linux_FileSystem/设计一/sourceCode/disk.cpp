#include"head.h"

//���´�����Ϣ
void updateFileInfo(int indexGroup, int index, const char* fileName, file_type type, file_authority authority, int blockAddr, int size) 
{
	strcpy(inodeTable[indexGroup][index].strName, fileName);
	cout << "����Ŀ¼��" << inodeTable[indexGroup][index].strName
		<< "��i����λͼ��������......\n";
	time_t now;
	now = time(&now);
	inodeTable[indexGroup][index].createTime = now;  //��ʼ��ʱ��
	inodeTable[indexGroup][index].id = 0;  //��ʼ��id
	inodeTable[indexGroup][index].authority = authority;  //��ʼ����дȫ����
	inodeTable[indexGroup][index].type = type;  //��ʼ���ļ����� 
	inodeTable[indexGroup][index].addrBlock = blockAddr;  //��ʼ�����̿�λ��
	inodeTable[indexGroup][index].size = size;  //��ʼ���ļ���С��ֻ������Ч�ֽ������Ƕ���Ŀռ�
	inodeTable[indexGroup][index].nBlock = 1;  //�ݶ�ÿ���ļ�ֻռ��һ����
	inodeBitmap[indexGroup][index] = OCCUPIED;  //����λͼ
	blockBitmap[indexGroup][index] = OCCUPIED;  //����λͼ
	blockGroup[indexGroup].superBlock.nFreeBlocks -= inodeTable[indexGroup][index].nBlock;
	blockGroup[indexGroup].superBlock.nFreeInodes -= 1;
}

void saveDisk()
{
	int i, j;
	fsVirtualDisk.open("../../virtual_file_system.bin", ios::out | ios::binary);
	if (!fsVirtualDisk.is_open())
	{
		cout << "�޷���������̣�\n";
		exit(0);
	}
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)
		fsVirtualDisk.write(reinterpret_cast<char*>(&blockGroup[i]), sizeof(block_group));
	//�Դ��̿�λͼ��i���λͼ��ʼ��
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)
		for (j = 0; j < BLOCK_NUM_EACH_GROUP; j++)
			fsVirtualDisk.write(reinterpret_cast<char*>(&blockBitmap[i][j]), sizeof(bitmap));
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)
		for (j = 0; j < INODES_NUM_EACH_GROUP; j++)
			fsVirtualDisk.write(reinterpret_cast<char*>(&inodeBitmap[i][j]), sizeof(bitmap));
	//��i�����ʼ��
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)
		for (j = 0; j < INODES_NUM_EACH_GROUP; j++)
			fsVirtualDisk.write(reinterpret_cast<char*>(&inodeTable[i][j]), sizeof(i_node));
	cout << "���ڳ�ʼ�����̿ռ�......\n";
	emptyBlock eB;
	//��1�����̿���ר�ŷָ�Ŀ¼�ļ�
	for (j = 0; j < BLOCK_NUM_EACH_GROUP; j++)
	{
		if (j < vecDir.size())
		{
			fsVirtualDisk.write(reinterpret_cast<char*>(&vecDir[j]), sizeof(dir));
		}
		else
			fsVirtualDisk.write(reinterpret_cast<char*>(&eB), sizeof(emptyBlock));
	}
	//�������̿���ָ������ļ�
	for (i = 1; i < BLOCK_GROUPS_NUM; i++)
	{
		for (j = 0; j < BLOCK_NUM_EACH_GROUP; j++)
		{
			if(j < vecFile.size())
				fsVirtualDisk.write(reinterpret_cast<char*>(&vecFile[j]), sizeof(file));
			else
				fsVirtualDisk.write(reinterpret_cast<char*>(&eB), sizeof(emptyBlock));
		}
	}
	fsVirtualDisk.close();
}

void initFileSystem()
{
	cout << "���ڳ�ʼ��ģ���ļ�ϵͳ......\n";
	int i, j;
	//��ÿһ���̿���Ĵ��̿��ʼ��
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)
	{
		blockGroup[i].superBlock.nFreeBlocks = TOTAL_BLOCKS_NUM - 1;
		blockGroup[i].superBlock.nFreeInodes = TOTAL_INODES_NUM - 1;
	}
	//�Դ��̿�λͼ��i���λͼ��ʼ��
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)
		for (j = 0; j < BLOCK_NUM_EACH_GROUP; j++)
			blockBitmap[i][j] = EMPTY;
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)
		for (j = 0; j < INODES_NUM_EACH_GROUP; j++)
			inodeBitmap[i][j] = EMPTY;
	//��i�����ʼ��
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)
		for(j = 0; j < INODES_NUM_EACH_GROUP; j++)
		{
			inodeTable[i][j].id = 0;
			inodeTable[i][j].authority = READ_WRITE;
			inodeTable[i][j].size = 0;
			inodeTable[i][j].type = typeBlock;
			inodeTable[i][j].addrBlock = -1;
			inodeTable[i][j].nBlock = 0;
		}

	//�½�rootĿ¼�ļ���������bitmap��inodeTable��superblock����root�ļ�д���ڴ沢д�����
	vecDir.push_back(dir(0, 0));
	updateFileInfo(0, 0, (char*)"root", typeDIRECTORY, READ_WRITE, DATA_AREA_ADDR, sizeof(dir) - 1016);
	emptyDirBlockIndex += sizeof(dir);

	//��������Ϣ���浽.bin�ļ���
	saveDisk(); //blockGroup + inodeBitmap + inodeTable + blockBitmap + block

	currentPathDir = 0;
	strcpy(currentPath, inodeTable[vecDir[currentPathDir].nGroup][vecDir[currentPathDir].nInode].strName);
	//��ʼ��System�û�
	users.push_back(user((char*)"SYSTEM", 0, SYSTEM, (char*)"123456"));
	cout << "��ʼ����ɣ�\n�����ù���Ա���룺";
	char admPassword[32];
	cin >> admPassword;
	users.push_back(user((char*)"ADMIN", 1, ADMINISTRATOR, admPassword));
	cout << "��ʼ����ɣ�\n�Ƿ�ע���û�����Y/N��";
	string ifRegister;
	cin >> ifRegister;
	while (ifRegister == "Y" || ifRegister == "y")
	{
		char nme[32], pwd[32];
		cout << "�������û�����";
		cin >> nme;
		cout << "�������û����룺";
		cin >> pwd;
		users.push_back(user(nme, sizeof(users), VISITOR, pwd));
		cout << "�û������ɹ���\n�Ƿ�ע���û�����Y/N��";
		cin >> ifRegister;
	}
	cout << "���ڴ洢�û���Ϣ......\n";
	fstream outfile;
	outfile.open("../../users.dat", ios::out);
	for (i = 0; i < users.size(); i++)
		outfile.write((char*)&users[i], sizeof(user));
	outfile.close();
	cout << "��ʼ����ɣ�ת���½����......\n";
}

void loadDisk()
{
	int i, j;
	system("cls");  //�����Ļ
	fsVirtualDisk.open("../../virtual_file_system.bin", ios::in | ios::binary);
	if (!fsVirtualDisk.is_open())  //������ģ���ļ�ϵͳ�������
	{
		cout << "ģ���ļ�ϵͳδ��ʼ��\n"
			<< "�Ƿ��ʼ��ģ���ļ�ϵͳ����Y/N��";
		string choiceIfInit;
		cin >> choiceIfInit;
		if (choiceIfInit == "n" || choiceIfInit == "N")  //����ʼ��ģ���ļ�ϵͳ���˳�
		{
			cout << "��ʼ��ģ���ļ�ϵͳʧ�ܣ��˳���......\n";
			exit(0);  //�޷���ʼ���ļ�ϵͳ���˳�
		}
		else  //��ʼ���ļ�ϵͳ������cmd_init()��Ȼ��return
		{
			fsVirtualDisk.clear();
			initFileSystem();
			return;
		}
	}
	cout << "���ڳ�ʼ���ļ�ϵͳ......\n"
		<< "���ڶ����������......\n";
	//������ڣ�����������е����ݶ����ڴ�
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)
		fsVirtualDisk.read(reinterpret_cast<char*>(&blockGroup[i]), sizeof(block_group));
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)
		for (j = 0; j < BLOCK_NUM_EACH_GROUP; j++)
			fsVirtualDisk.read(reinterpret_cast<char*>(&blockBitmap[i][j]), sizeof(bitmap));
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)
		for (j = 0; j < INODES_NUM_EACH_GROUP; j++)
			fsVirtualDisk.read(reinterpret_cast<char*>(&inodeBitmap[i][j]), sizeof(bitmap));
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)
		for (j = 0; j < INODES_NUM_EACH_GROUP; j++)
			fsVirtualDisk.read(reinterpret_cast<char*>(&inodeTable[i][j]), sizeof(i_node));
	vecDir.clear();
	vecFile.clear();
	dir tempDir;
	file tempFile;
	emptyBlock eB;
	//�������е�Ŀ¼�����ڴ�
	for (j = 0; j < INODES_NUM_EACH_GROUP; j++)
	{
		if (inodeBitmap[0][j] == OCCUPIED)
		{
			fsVirtualDisk.read(reinterpret_cast<char*>(&tempDir), sizeof(dir));
			vecDir.push_back(tempDir);
			emptyDirBlockIndex += sizeof(dir);
		}
		else
			fsVirtualDisk.read(reinterpret_cast<char*>(&eB), sizeof(emptyBlock));
	}
	//�������е��ļ������ڴ�
	for (i = 1; i < BLOCK_GROUPS_NUM; i++)
	{
		for (j = 0; j < INODES_NUM_EACH_GROUP; j++)
		{
			if (inodeBitmap[i][j] == OCCUPIED)
			{
				fsVirtualDisk.read(reinterpret_cast<char*>(&tempFile), sizeof(file));
				vecFile.push_back(tempFile);
				emptyDirBlockIndex += sizeof(file);
			}
			else
				fsVirtualDisk.read(reinterpret_cast<char*>(&eB), sizeof(emptyBlock));
		}
	}
	currentPathDir = 0;
	strcpy(currentPath, inodeTable[vecDir[currentPathDir].nGroup][vecDir[currentPathDir].nInode].strName);
	fsVirtualDisk.close();
	cout << "���ڶ����û���Ϣ......\n";
	//���û���Ϣ�����ڴ�
	fstream infile;
	infile.open("../../users.dat", ios::in | ios::out);
	users.clear();  //���vectorԪ��
	user sb;
	while (infile.read((char*)&sb, sizeof(sb))) { //һֱ�����ļ�����
		users.push_back(sb);
	}
	cout << "ģ���ļ�ϵͳ��ʼ���ɹ���\n";
}

void cmd_check()
{
	if (presentUser.type == VISITOR)
	{
		cout << "��Ҫ����ԱȨ�ޣ�\n";
		return;
	}
	cout << "���ڼ��ģ���ļ�ϵͳ����һ����......";
	int i, j;
	fsVirtualDisk.open("../../virtual_file_system.bin", ios::in | ios::binary);
	block_group checkBlockGroup[BLOCK_GROUPS_NUM];  //���̿���
	i_node checkInodeTable[BLOCK_GROUPS_NUM][INODES_NUM_EACH_GROUP];  //i����
	bitmap checkInodeBitmap[BLOCK_GROUPS_NUM][INODES_NUM_EACH_GROUP];  //i���λͼ
	bitmap checkBlockBitmap[BLOCK_GROUPS_NUM][BLOCK_NUM_EACH_GROUP];  //����λͼ
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)
		fsVirtualDisk.read(reinterpret_cast<char*>(&checkBlockGroup[i]), sizeof(block_group));
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)
		for (j = 0; j < BLOCK_NUM_EACH_GROUP; j++)
			fsVirtualDisk.read(reinterpret_cast<char*>(&checkBlockBitmap[i][j]), sizeof(bitmap));
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)
		for (j = 0; j < INODES_NUM_EACH_GROUP; j++)
			fsVirtualDisk.read(reinterpret_cast<char*>(&checkInodeBitmap[i][j]), sizeof(bitmap));
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)
		for (j = 0; j < INODES_NUM_EACH_GROUP; j++)
			fsVirtualDisk.read(reinterpret_cast<char*>(&checkInodeTable[i][j]), sizeof(i_node));
	fsVirtualDisk.close();
	int ifConsist = 1;
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)
	{
		if (reinterpret_cast<char*>(&checkBlockGroup[i]) == reinterpret_cast<char*>(&blockGroup[i]) && ifConsist == 1);
		else
		{
			ifConsist = 0;
			break;
		}
	}
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)
		for (j = 0; j < INODES_NUM_EACH_GROUP; j++)
		{
			if (reinterpret_cast<char*>(&checkBlockBitmap[i][j]) == reinterpret_cast<char*>(&blockBitmap[i][j]) && ifConsist == 1);
			else
			{
				ifConsist = 0;
				break;
			}
		}
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)
		for (j = 0; j < INODES_NUM_EACH_GROUP; j++)
		{
			if (reinterpret_cast<char*>(&checkInodeBitmap[i][j]) == reinterpret_cast<char*>(&inodeBitmap[i]) && ifConsist == 1);
			else
			{
				ifConsist = 0;
				break;
			}
		}
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)
		for (j = 0; j < BLOCK_NUM_EACH_GROUP; j++)
		{
			if (reinterpret_cast<char*>(&checkInodeTable[i][j]) == reinterpret_cast<char*>(&inodeTable[i][j]) && ifConsist == 1);
			else
			{
				ifConsist = 0;
				break;
			}
		}
	if (ifConsist)
	{
		cout << "�����ϣ�δ�������⣡\n";
		return;
	}
	else
	{
		cout << "ģ���ļ�ϵͳ������һ���ԣ�\n��ʼ�ָ�......";
		saveDisk();
		return;
	}
}