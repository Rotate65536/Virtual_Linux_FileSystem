#include"head.h"
//��ʾģ���ļ�ϵͳ�汾��Ϣ
void cmd_ver()
{
	cout << "virtualFS. " << 
		"Copyright 2021 Rotate65536. All rights reserved.\n";
	shellOutput("virtualFS. Copyright 2021 Rotate65536. All rights reserved.\n");
}

//��½����
bool cmd_login()
{
	char nameAccount[32], pwd[32];
	cout << "��½......\n�˺ţ�";
	shellOutput("��½......\n�˺ţ�");
	shellInput(nameAccount);
	cout << "���룺";
	shellOutput("���룺");
	shellInput(pwd);
	for (int i = 0; i < users.size(); i++)  //�����û�
	{
		if (!strcmp(users[i].name, nameAccount)) 
		{
			if (!strcmp(users[i].password, pwd))  //�Ƚ�����
			{
				presentUser = user(users[i]);
				return true;
			}
			else
			{
				cout << "�������\n";
				shellOutput(
					"�������\n"
				);
				return false;
			}
		}
		else;
	}
	cout << "�û������ڣ�\n";
	shellOutput(
		"�û������ڣ�\n");
	cout << "�Ƿ��˳�ģ���ļ�ϵͳ����Y/N��\n";
	shellOutput(
		"�Ƿ��˳�ģ���ļ�ϵͳ����Y/N��\n");
	string ifExit;
	char charifExit[MAX_COMMAND_LENGTH];
	shellInput(charifExit);
	ifExit = charifExit;
	if (ifExit == "y" || ifExit == "Y")
		exit(0);
	return false;
}

//��ʾϵͳ��Ϣ
void cmd_info()
{
	shellOutput(string("ģ���ļ�ϵͳ��Ϣ��\n") +
		string("������Ϣ��\n") +
		string("�û�������") +
		string(to_string(users.size())) +
		string("��\n") +
		string("����������") +
		string(to_string(TOTAL_BLOCKS_NUM * BLOCK_SIZE)) +
		string(" �ֽ�\t") +
		string(to_string((float)TOTAL_BLOCKS_NUM / 1024)) +
		string(" MB\n") +
		string("���ÿռ䣺") +
		string(to_string((TOTAL_BLOCKS_NUM - blockGroup[0].superBlock.nFreeBlocks) * BLOCK_SIZE)) +
		string(" �ֽ�\t") +
		string(to_string((float)(TOTAL_BLOCKS_NUM - blockGroup[0].superBlock.nFreeBlocks) / 1024)) +
		string(" MB\n") +
		string("���ÿռ䣺") +
		string(to_string(blockGroup[0].superBlock.nFreeBlocks * BLOCK_SIZE)) +
		string(" �ֽ�\t") +
		string(to_string((float)blockGroup[0].superBlock.nFreeBlocks / 1024)) +
		string(" MB\n") +
		string("���ÿռ������") +
		string(to_string(((float)blockGroup[0].superBlock.nFreeBlocks / (float)TOTAL_BLOCKS_NUM) * 100)) +
		string("%\n") +
		string("\n��ϸ��Ϣ��\n") +
		string("�̿��С��") +
		string(to_string(BLOCK_SIZE)) +
		string(" �ֽ�\n") +
		string("ÿ���̿�����") +
		string(to_string(BLOCK_NUM_EACH_GROUP)) +
		string(" ��\n") +
		string("ÿ��i-�������") +
		string(to_string(INODES_NUM_EACH_GROUP)) +
		string(" ��\n") +
		string("�̿�������") +
		string(to_string(TOTAL_BLOCKS_NUM)) +
		string(" ��\n") +
		string("i-���������") +
		string(to_string(TOTAL_INODES_NUM)) +
		string(" ��\n") +
		string("���п�������") +
		string(to_string(blockGroup[0].superBlock.nFreeBlocks)) +
		string(" ��\n")+
		string("����i-���������")+
		string(to_string(blockGroup[0].superBlock.nFreeInodes))+
		string(" ��\n") +
		string("Ŀ¼������") +
		string(to_string(vecDir.size())) +
		string("��\n") +
		string("�ļ�������") +
		string(to_string(vecFile.size())) +
		string("��\n"));
	cout << "ģ���ļ�ϵͳ��Ϣ��\n\n" 
		<< "������Ϣ��\n" 
		<< "�û�������"
		<< users.size()
		<< " ��\n" << "����������"
		<< TOTAL_BLOCKS_NUM * BLOCK_SIZE
		<< " �ֽ�\t"
		<< (float)TOTAL_BLOCKS_NUM / 1024
		<< " MB\n"
		<< "���ÿռ䣺"
		<< (TOTAL_BLOCKS_NUM - blockGroup[0].superBlock.nFreeBlocks) * BLOCK_SIZE
		<< " �ֽ�\t"
		<< (float)(TOTAL_BLOCKS_NUM - blockGroup[0].superBlock.nFreeBlocks) / 1024
		<< " MB\n"
		<< "���ÿռ䣺"
		<< blockGroup[0].superBlock.nFreeBlocks * BLOCK_SIZE
		<< " �ֽ�\t"
		<< (float)blockGroup[0].superBlock.nFreeBlocks / 1024
		<< " MB\n"
		<< "���ÿռ������"
		<< ((float)blockGroup[0].superBlock.nFreeBlocks / (float)TOTAL_BLOCKS_NUM) * 100
		<< "%\n"
		<< "\n��ϸ��Ϣ��\n"
		<< "�̿��С��"
		<< BLOCK_SIZE
		<< " �ֽ�\n"
		<< "ÿ���̿�����"
		<< BLOCK_NUM_EACH_GROUP
		<< " ��\n"
		<< "ÿ��i-�������"
		<< INODES_NUM_EACH_GROUP
		<< " ��\n"
		<< "�̿�������"
		<< TOTAL_BLOCKS_NUM
		<< " ��\n"
		<< "i-���������"
		<< TOTAL_INODES_NUM
		<< " ��\n"
		<< "���п�������"
		<< blockGroup[0].superBlock.nFreeBlocks
		<< " ��\n"
		<< "����i-���������"
		<< blockGroup[0].superBlock.nFreeInodes
		<< " ��\n"
		<< "Ŀ¼������"
		<< vecDir.size()
		<< "��\n"
		<< "�ļ�������"
		<< vecFile.size()
		<< "��\n";
}

//�˳�ϵͳ
void cmd_exit()
{
	cout << "�����˳�ϵͳ......\n";
	shellOutput(
		"�����˳�ϵͳ......\n");
	saveDisk();
	exit(0);
}

//Ѱ������
void execute_cmd(const char* command, const char* arg1, const char* arg2)
{
	int i, j;
	//��������̽�������Ϣ�����ڴ�������û���Ϣ����
	fsVirtualDisk.open("../../virtual_file_system.bin", ios::in | ios::binary);
	//������ڣ�����������е����ݶ����ڴ�
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)  //��������Ϣ
		fsVirtualDisk.read(reinterpret_cast<char*>(&blockGroup[i]), sizeof(block_group));
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)  //���̿�λͼ��Ϣ
		for (j = 0; j < BLOCK_NUM_EACH_GROUP; j++)
			fsVirtualDisk.read(reinterpret_cast<char*>(&blockBitmap[i][j]), sizeof(bitmap));
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)  //i�ڵ�λͼ��Ϣ
		for (j = 0; j < INODES_NUM_EACH_GROUP; j++)
			fsVirtualDisk.read(reinterpret_cast<char*>(&inodeBitmap[i][j]), sizeof(bitmap));
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)  //i�ڵ����Ϣ
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
	fsVirtualDisk.close();

	for (i = 0; i < COMMAND_COUNT; i++)
		if (strcmp(command, commands[i]) == 0)
			break;
	switch (i)
	{
	case 0://ver
	{
		if (arg1[0] != '\0')
		{
			cout << "�������࣡\n";
			shellOutput(
				"�������࣡\n"
			);
			return;
		}
		cmd_ver();
		break;
	}
	case 1://login
	{
		if (arg1[0] != '\0')
		{
			cout << "�������࣡\n";
			shellOutput(
				"�������࣡\n"
			);
			return;
		}
		cmd_login();
		break;
	}
	case 2://init
	{
		if (arg1[0] != '\0')
		{
			cout << "�������࣡\n";
			shellOutput(
				"�������࣡\n"
			);
			return;
		}
		if (presentUser.type == VISITOR)
		{
			cout << "Ȩ�޲��㣡\n";
			shellOutput(
				"Ȩ�޲��㣡\n"
			);
			return;
		}
		cout << "���棺��ʼ��������ģ���ļ�ϵͳ������Ϣ����Y/N��\n";
		shellOutput(
			"���棺��ʼ��������ģ���ļ�ϵͳ������Ϣ����Y/N��\n");
		string ifInit;
		char charifInit[MAX_COMMAND_LENGTH];
		shellInput(charifInit);
		ifInit = charifInit;
		if (ifInit == "Y" || ifInit == "y")
		{
			fsVirtualDisk.clear();
			system("cls");
			loadDisk();
			system("title ģ���ļ�ϵͳ");
			while (!cmd_login());
			cmd_ver();
		}
		else
		{
			cout << "ȡ������\n";
			shellOutput(
				"ȡ������\n"
			);
		}
		break;
	}
	case 3://info
	{
		if (arg1[0] != '\0')
		{
			cout << "�������࣡\n";
			shellOutput(
				"�������࣡\n"
			);
			return;
		}
		cmd_info();
		break;
	}
	case 4://cd
	{
		if (arg2[0] != '\0')
		{
			cout << "�������࣡\n";
			shellOutput(
				"�������࣡\n"
			);
			return;
		}
		cmd_cd(arg1);
		break;
	}
	case 5://dir
	{
		if (arg2[0] != '\0')
		{
			cout << "�������࣡\n";
			shellOutput(
				"�������࣡\n"
			);
			return;
		}
		if (arg1[0] == '\0')
			cmd_dir(inodeTable[vecDir[currentPathDir].nGroup][vecDir[currentPathDir].nGroup].strName);
		else
			cmd_dir(arg1);
		break;
	}
	case 6://md
	{
		if (arg1[0] == '\0')
		{
			cout << "����������٣�\n";
			shellOutput(
				"�������٣�\n"
			);
			return;
		}
		if (arg2[0] != '\0')
		{
			if (strcmp("/r", arg2) == 0)
			{
				cmd_md(arg1, READ_ONLY);
				return;
			}
			if (strcmp("/s", arg2) == 0)
			{
				cmd_md(arg1, PROTECTED);
				return;
			}
			if (strcmp("/a", arg2) == 0)
			{
				cmd_md(arg1, READ_WRITE);
				return;
			}
			cout << "�����������\n";
			shellOutput(
				"��������\n"
			);
			return;
		}
		else
		{
			cmd_md(arg1, READ_WRITE);
		}
		break;
	}
	case 7://rd
	{
		if (arg1[0] == '\0' || arg2[0] != '\0')
		{
			cout << "��������\n";
			shellOutput(
				"��������\n"
			);
			return;
		}
		int thisPath = currentPathDir;
		cmd_rd(arg1, thisPath);
		//��ʱֻ��ɾ�ļ��к��ļ������ļ����޷�ɾ�ļ������ļ���
		break;
	}
	case 8://newfile
	{
		if (arg1[0] == '\0')
		{
			cout << "��������\n";
			shellOutput(
				"��������\n"
			);
			return;
		}
		if (arg2[0] != '\0')
		{
			if (strcmp("/r", arg2) == 0)
			{
				cmd_newfile(arg1, READ_ONLY);
				return;
			}			
			if (strcmp("/s", arg2) == 0)
			{
				cmd_newfile(arg1, PROTECTED);
				return;
			}
			if (strcmp("/a", arg2) == 0)
			{
				cmd_newfile(arg1, READ_WRITE);
				return;
			}
			cout << "��������\n";
			shellOutput(
				"��������\n"
			);
			return;
		}
		cmd_newfile(arg1, READ_WRITE);
		break;
	}
	case 9://cat
	{
		if (arg1[0] == '\0' || arg2[0] != '\0')
		{
			cout << "��������\n";
			shellOutput(
				"��������\n"
			);
			return;
		}
		cmd_cat(arg1);
		break;
	}
	case 10://copy
	{
		if (arg2[0] == '\0')
		{
			cout << "��������\n";
			shellOutput(
				"��������\n"
			);
			return;
		}
		// ������ת���ļ���ģ���ļ�ϵͳʱ��������
		cmd_copy(arg1, arg2);
	}
	case 11://del
	{
		if (arg1[0] == '\0' || arg2[0] != '\0')
		{
			cout << "��������\n";
			shellOutput(
				"��������\n"
			);
			return;
		}
		cmd_del(arg1);
		break;
	}
	case 12://check
	{
		if (arg1[0] != '\0')
		{
			cout << "�������࣡\n";
			shellOutput(
				"�������࣡\n"
			);
			return;
		}
		cmd_check();
	}
	case 13://register
	{
		if (arg1[0] != '\0')
		{
			cout << "�������࣡\n";
			shellOutput(
				"�������࣡\n"
			);
			return;
		}
		if (presentUser.type == VISITOR)
		{
			cout << "Ȩ�޲��㣡\n";
			shellOutput(
				"Ȩ�޲��㣡\n"
			);
			return;
		}
		cout << "�Ƿ����ע���û�����Y/N��\n";
		shellOutput(
			"�Ƿ����ע���û�����Y/N��\n"
		);
		string ifRegister;
		char charifRegister[MAX_COMMAND_LENGTH];
		shellInput(charifRegister);
		ifRegister = charifRegister;
		while (ifRegister == "Y" || ifRegister == "y")
		{
			char nme[32], pwd[32];
			cout << "\n�������û�����";
			shellOutput(
				"\n�������û�����");
			shellInput(nme);
			cout << "\n�������û����룺";
			shellOutput(
				"\n�������û����룺");
			shellInput(nme);
			users.push_back(user(nme, sizeof(users), VISITOR, pwd));
			cout << "\n�û������ɹ���\n�Ƿ�ע���û�����Y/N��";
			shellOutput(
				"\n�û������ɹ���\n�Ƿ�ע���û�����Y/N��");
			shellInput(charifRegister);
			ifRegister = charifRegister;
			cout << "�Ƿ����ע���û�����Y/N��";
			shellOutput(
				"�Ƿ����ע���û�����Y/N��\n"
			);
		}
		//�洢�û���Ϣ���������ļ�
		cout << "\n���ڴ洢�û���Ϣ......\n";
		shellOutput(
			"\n���ڴ洢�û���Ϣ......\n"
		);
		fstream outfile;
		outfile.open("../../users.dat", ios::out);
		for (i = 0; i < users.size(); i++)
			outfile.write((char*)&users[i], sizeof(user));
		outfile.close();
		//���û���Ϣ�����ڴ�
		fstream infile;
		infile.open("../../users.dat", ios::in | ios::out);
		users.clear();  //���vectorԪ��
		user sb;
		while (infile.read((char*)&sb, sizeof(sb))) { //һֱ�����ļ�����
			users.push_back(sb);
		}
		break;
	}
	case 14://exit
	{
		if (arg1[0] != '\0')
		{
			cout << "�������࣡\n";
			shellOutput(
				"�������࣡\n"
			);
			return;
		}
		cmd_exit();
		return;
	}
	case 15://show_dir
	{
		if (arg1[0] != '\0')
		{
			cout << "�������࣡\n";
			shellOutput(
				"�������࣡\n"
			);
			return;
		}
		cmd_show_files();
		return;
	}
	}
}