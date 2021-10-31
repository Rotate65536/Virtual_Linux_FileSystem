#include"head.h"
//��ʾģ���ļ�ϵͳ�汾��Ϣ
void cmd_ver()
{
	cout << "virtualFS. " << 
		"Copyright 2021 Rotate65536. All rights reserved.\n";
}

//��½����
bool cmd_login()
{
	char nameAccount[32], pwd[32];
	cout << "��½......\n�˺ţ�";
	cin >> nameAccount;
	cout << "���룺";
	cin >> pwd;
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
				return false;
			}
		}
		else;
	}
	cout << "�û������ڣ�\n";
	cout << "�Ƿ��˳�ģ���ļ�ϵͳ����Y/N��\n";
	string ifExit;
	cin >> ifExit;
	if (ifExit == "y" || ifExit == "Y")
		exit(0);
	return false;
}

//��ʾϵͳ��Ϣ
void cmd_info()
{
	cout << "ģ���ļ�ϵͳ��Ϣ��\n\n";
	cout << "������Ϣ��\n";
	cout << "�û�������";
	cout << users.size();
	cout << " ��\n";
	cout << "����������";
	cout << TOTAL_BLOCKS_NUM * BLOCK_SIZE;
	cout << " �ֽ�\t";
	cout << (float)TOTAL_BLOCKS_NUM / 1024;
	cout << " MB\n";
	cout << "���ÿռ䣺";
	cout << (TOTAL_BLOCKS_NUM - blockGroup[0].superBlock.nFreeBlocks) * BLOCK_SIZE;
	cout << " �ֽ�\t";
	cout << (float)(TOTAL_BLOCKS_NUM - blockGroup[0].superBlock.nFreeBlocks) / 1024;
	cout << " MB\n";
	cout << "���ÿռ䣺";
	cout << blockGroup[0].superBlock.nFreeBlocks * BLOCK_SIZE;
	cout << " �ֽ�\t";
	cout << (float)blockGroup[0].superBlock.nFreeBlocks / 1024;
	cout << " MB\n";
	cout << "���ÿռ������";
	cout << ((float)blockGroup[0].superBlock.nFreeBlocks / (float)TOTAL_BLOCKS_NUM) * 100
		<< "%\n";
	cout << "\n��ϸ��Ϣ��\n";
	cout << "�̿��С��";
	cout << BLOCK_SIZE;
	cout << " �ֽ�\n";
	cout << "ÿ���̿�����";
	cout << BLOCK_NUM_EACH_GROUP;
	cout << " ��\n";
	cout << "ÿ��i-�������";
	cout << INODES_NUM_EACH_GROUP;
	cout << " ��\n";
	cout << "�̿�������";
	cout << TOTAL_BLOCKS_NUM;
	cout << " ��\n";
	cout << "i-���������";
	cout << TOTAL_INODES_NUM;
	cout << " ��\n";
	cout << "���п�������";
	cout << blockGroup[0].superBlock.nFreeBlocks;
	cout << " ��\n";
	cout << "����i-���������";
	cout << blockGroup[0].superBlock.nFreeInodes;
	cout << " ��\n";
	cout << "Ŀ¼������";
	cout << vecDir.size();
	cout << "��\n";
	cout << "�ļ�������";
	cout << vecFile.size();
	cout << "��\n";
}

//�˳�ϵͳ
void cmd_exit()
{
	cout << "�����˳�ϵͳ......\n";
	saveDisk();
	exit(0);
}

//Ѱ������
void execute_cmd(const char* command, const char* arg1, const char* arg2)
{
	int i;
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
			return;
		}
		if (presentUser.type == VISITOR)
		{
			cout << "Ȩ�޲��㣡\n";
			return;
		}
		cout << "���棺��ʼ��������ģ���ļ�ϵͳ������Ϣ����Y/N��\n";
		string ifInit;
		cin >> ifInit;
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
		}
		break;
	}
	case 3://info
	{
		if (arg1[0] != '\0')
		{
			cout << "�������࣡\n";
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
			return;
		}
		if (arg2[0] != '\0')
		{
			if (strcmp("/r", arg2) == 0)
			{
				cmd_md(arg1, READ_ONLY);
				return;
			}
			if (strcmp("/a", arg2) == 0)
			{
				cmd_md(arg1, READ_WRITE);
				return;
			}
			cout << "�����������\n";
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
			return;
		}
		if (arg2[0] != '\0')
		{
			if (strcmp("/r", arg2) == 0)
			{
				cmd_newfile(arg1, READ_ONLY);
				return;
			}
			if (strcmp("/a", arg2) == 0)
			{
				cmd_newfile(arg1, READ_WRITE);
				return;
			}
			cout << "��������\n";
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
			return;
		}
		cmd_check();
	}
	case 13://register
	{
		if (arg1[0] != '\0')
		{
			cout << "�������࣡\n";
			return;
		}
		if (presentUser.type == VISITOR)
		{
			cout << "Ȩ�޲��㣡\n";
			return;
		}
		cout << "�Ƿ����ע���û�����Y/N��\n";
		string ifRegister;
		cin >> ifRegister;
		while (ifRegister == "Y" || ifRegister == "y")
		{
			char nme[32], pwd[32];
			cout << "\n�������û�����";
			cin >> nme;
			cout << "\n�������û����룺";
			cin >> pwd;
			users.push_back(user(nme, sizeof(users), VISITOR, pwd));
			cout << "\n�û������ɹ���\n�Ƿ�ע���û�����Y/N��";
			cin >> ifRegister;
			cout << "�Ƿ����ע���û�����Y/N��";
		}
		//�洢�û���Ϣ���������ļ�
		cout << "\n���ڴ洢�û���Ϣ......\n";
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
			return;
		}
		cmd_show_files();
		return;
	}
	}
}