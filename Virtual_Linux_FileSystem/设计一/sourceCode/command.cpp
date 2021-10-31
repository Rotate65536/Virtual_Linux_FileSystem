#include"head.h"
//显示模拟文件系统版本信息
void cmd_ver()
{
	cout << "virtualFS. " << 
		"Copyright 2021 Rotate65536. All rights reserved.\n";
}

//登陆命令
bool cmd_login()
{
	char nameAccount[32], pwd[32];
	cout << "登陆......\n账号：";
	cin >> nameAccount;
	cout << "密码：";
	cin >> pwd;
	for (int i = 0; i < users.size(); i++)  //查找用户
	{
		if (!strcmp(users[i].name, nameAccount)) 
		{
			if (!strcmp(users[i].password, pwd))  //比较密码
			{
				presentUser = user(users[i]);
				return true;
			}
			else
			{
				cout << "密码错误！\n";
				return false;
			}
		}
		else;
	}
	cout << "用户不存在！\n";
	cout << "是否退出模拟文件系统：（Y/N）\n";
	string ifExit;
	cin >> ifExit;
	if (ifExit == "y" || ifExit == "Y")
		exit(0);
	return false;
}

//显示系统信息
void cmd_info()
{
	cout << "模拟文件系统信息：\n\n";
	cout << "整体信息：\n";
	cout << "用户数量：";
	cout << users.size();
	cout << " 个\n";
	cout << "磁盘容量：";
	cout << TOTAL_BLOCKS_NUM * BLOCK_SIZE;
	cout << " 字节\t";
	cout << (float)TOTAL_BLOCKS_NUM / 1024;
	cout << " MB\n";
	cout << "已用空间：";
	cout << (TOTAL_BLOCKS_NUM - blockGroup[0].superBlock.nFreeBlocks) * BLOCK_SIZE;
	cout << " 字节\t";
	cout << (float)(TOTAL_BLOCKS_NUM - blockGroup[0].superBlock.nFreeBlocks) / 1024;
	cout << " MB\n";
	cout << "可用空间：";
	cout << blockGroup[0].superBlock.nFreeBlocks * BLOCK_SIZE;
	cout << " 字节\t";
	cout << (float)blockGroup[0].superBlock.nFreeBlocks / 1024;
	cout << " MB\n";
	cout << "可用空间比例：";
	cout << ((float)blockGroup[0].superBlock.nFreeBlocks / (float)TOTAL_BLOCKS_NUM) * 100
		<< "%\n";
	cout << "\n详细信息：\n";
	cout << "盘块大小：";
	cout << BLOCK_SIZE;
	cout << " 字节\n";
	cout << "每组盘块数：";
	cout << BLOCK_NUM_EACH_GROUP;
	cout << " 块\n";
	cout << "每组i-结点数：";
	cout << INODES_NUM_EACH_GROUP;
	cout << " 个\n";
	cout << "盘块总数：";
	cout << TOTAL_BLOCKS_NUM;
	cout << " 块\n";
	cout << "i-结点总数：";
	cout << TOTAL_INODES_NUM;
	cout << " 个\n";
	cout << "空闲块总数：";
	cout << blockGroup[0].superBlock.nFreeBlocks;
	cout << " 块\n";
	cout << "空闲i-结点总数：";
	cout << blockGroup[0].superBlock.nFreeInodes;
	cout << " 个\n";
	cout << "目录总数：";
	cout << vecDir.size();
	cout << "个\n";
	cout << "文件总数：";
	cout << vecFile.size();
	cout << "个\n";
}

//退出系统
void cmd_exit()
{
	cout << "正在退出系统......\n";
	saveDisk();
	exit(0);
}

//寻找命令
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
			cout << "参数过多！\n";
			return;
		}
		cmd_ver();
		break;
	}
	case 1://login
	{
		if (arg1[0] != '\0')
		{
			cout << "参数过多！\n";
			return;
		}
		cmd_login();
		break;
	}
	case 2://init
	{
		if (arg1[0] != '\0')
		{
			cout << "参数过多！\n";
			return;
		}
		if (presentUser.type == VISITOR)
		{
			cout << "权限不足！\n";
			return;
		}
		cout << "警告：初始化将重置模拟文件系统所有信息：（Y/N）\n";
		string ifInit;
		cin >> ifInit;
		if (ifInit == "Y" || ifInit == "y")
		{
			fsVirtualDisk.clear();
			system("cls");
			loadDisk();
			system("title 模拟文件系统");
			while (!cmd_login());
			cmd_ver();
		}
		else
		{
			cout << "取消操作\n";
		}
		break;
	}
	case 3://info
	{
		if (arg1[0] != '\0')
		{
			cout << "参数过多！\n";
			return;
		}
		cmd_info();
		break;
	}
	case 4://cd
	{
		if (arg2[0] != '\0')
		{
			cout << "参数过多！\n";
			return;
		}
		cmd_cd(arg1);
		break;
	}
	case 5://dir
	{
		if (arg2[0] != '\0')
		{
			cout << "参数过多！\n";
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
			cout << "输入参数过少！\n";
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
			cout << "输入参数错误！\n";
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
			cout << "参数错误！\n";
			return;
		}
		int thisPath = currentPathDir;
		cmd_rd(arg1, thisPath);
		//暂时只能删文件夹和文件夹下文件，无法删文件夹下文件夹
		break;
	}
	case 8://newfile
	{
		if (arg1[0] == '\0')
		{
			cout << "参数错误！\n";
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
			cout << "参数错误！\n";
			return;
		}
		cmd_newfile(arg1, READ_WRITE);
		break;
	}
	case 9://cat
	{
		if (arg1[0] == '\0' || arg2[0] != '\0')
		{
			cout << "参数错误！\n";
			return;
		}
		cmd_cat(arg1);
		break;
	}
	case 10://copy
	{
		if (arg2[0] == '\0')
		{
			cout << "参数错误！\n";

			return;
		}
		// 宿主机转移文件到模拟文件系统时存在问题
		cmd_copy(arg1, arg2);
	}
	case 11://del
	{
		if (arg1[0] == '\0' || arg2[0] != '\0')
		{
			cout << "参数错误！\n";
			return;
		}
		cmd_del(arg1);
		break;
	}
	case 12://check
	{
		if (arg1[0] != '\0')
		{
			cout << "参数过多！\n";
			return;
		}
		cmd_check();
	}
	case 13://register
	{
		if (arg1[0] != '\0')
		{
			cout << "参数过多！\n";
			return;
		}
		if (presentUser.type == VISITOR)
		{
			cout << "权限不足！\n";
			return;
		}
		cout << "是否继续注册用户：（Y/N）\n";
		string ifRegister;
		cin >> ifRegister;
		while (ifRegister == "Y" || ifRegister == "y")
		{
			char nme[32], pwd[32];
			cout << "\n请输入用户名：";
			cin >> nme;
			cout << "\n请输入用户密码：";
			cin >> pwd;
			users.push_back(user(nme, sizeof(users), VISITOR, pwd));
			cout << "\n用户创建成功！\n是否注册用户：（Y/N）";
			cin >> ifRegister;
			cout << "是否继续注册用户：（Y/N）";
		}
		//存储用户信息到二进制文件
		cout << "\n正在存储用户信息......\n";
		fstream outfile;
		outfile.open("../../users.dat", ios::out);
		for (i = 0; i < users.size(); i++)
			outfile.write((char*)&users[i], sizeof(user));
		outfile.close();
		//将用户信息读入内存
		fstream infile;
		infile.open("../../users.dat", ios::in | ios::out);
		users.clear();  //清空vector元素
		user sb;
		while (infile.read((char*)&sb, sizeof(sb))) { //一直读到文件结束
			users.push_back(sb);
		}
		break;
	}
	case 14://exit
	{
		if (arg1[0] != '\0')
		{
			cout << "参数过多！\n";
			return;
		}
		cmd_exit();
		return;
	}
	case 15://show_dir
	{
		if (arg1[0] != '\0')
		{
			cout << "参数过多！\n";
			return;
		}
		cmd_show_files();
		return;
	}
	}
}