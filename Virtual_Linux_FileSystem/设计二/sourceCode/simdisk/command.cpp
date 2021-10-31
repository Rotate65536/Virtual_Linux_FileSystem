#include"head.h"
//显示模拟文件系统版本信息
void cmd_ver()
{
	cout << "virtualFS. " << 
		"Copyright 2021 Rotate65536. All rights reserved.\n";
	shellOutput("virtualFS. Copyright 2021 Rotate65536. All rights reserved.\n");
}

//登陆命令
bool cmd_login()
{
	char nameAccount[32], pwd[32];
	cout << "登陆......\n账号：";
	shellOutput("登陆......\n账号：");
	shellInput(nameAccount);
	cout << "密码：";
	shellOutput("密码：");
	shellInput(pwd);
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
				shellOutput(
					"密码错误！\n"
				);
				return false;
			}
		}
		else;
	}
	cout << "用户不存在！\n";
	shellOutput(
		"用户不存在！\n");
	cout << "是否退出模拟文件系统：（Y/N）\n";
	shellOutput(
		"是否退出模拟文件系统：（Y/N）\n");
	string ifExit;
	char charifExit[MAX_COMMAND_LENGTH];
	shellInput(charifExit);
	ifExit = charifExit;
	if (ifExit == "y" || ifExit == "Y")
		exit(0);
	return false;
}

//显示系统信息
void cmd_info()
{
	shellOutput(string("模拟文件系统信息：\n") +
		string("整体信息：\n") +
		string("用户数量：") +
		string(to_string(users.size())) +
		string("个\n") +
		string("磁盘容量：") +
		string(to_string(TOTAL_BLOCKS_NUM * BLOCK_SIZE)) +
		string(" 字节\t") +
		string(to_string((float)TOTAL_BLOCKS_NUM / 1024)) +
		string(" MB\n") +
		string("已用空间：") +
		string(to_string((TOTAL_BLOCKS_NUM - blockGroup[0].superBlock.nFreeBlocks) * BLOCK_SIZE)) +
		string(" 字节\t") +
		string(to_string((float)(TOTAL_BLOCKS_NUM - blockGroup[0].superBlock.nFreeBlocks) / 1024)) +
		string(" MB\n") +
		string("可用空间：") +
		string(to_string(blockGroup[0].superBlock.nFreeBlocks * BLOCK_SIZE)) +
		string(" 字节\t") +
		string(to_string((float)blockGroup[0].superBlock.nFreeBlocks / 1024)) +
		string(" MB\n") +
		string("可用空间比例：") +
		string(to_string(((float)blockGroup[0].superBlock.nFreeBlocks / (float)TOTAL_BLOCKS_NUM) * 100)) +
		string("%\n") +
		string("\n详细信息：\n") +
		string("盘块大小：") +
		string(to_string(BLOCK_SIZE)) +
		string(" 字节\n") +
		string("每组盘块数：") +
		string(to_string(BLOCK_NUM_EACH_GROUP)) +
		string(" 块\n") +
		string("每组i-结点数：") +
		string(to_string(INODES_NUM_EACH_GROUP)) +
		string(" 个\n") +
		string("盘块总数：") +
		string(to_string(TOTAL_BLOCKS_NUM)) +
		string(" 块\n") +
		string("i-结点总数：") +
		string(to_string(TOTAL_INODES_NUM)) +
		string(" 个\n") +
		string("空闲块总数：") +
		string(to_string(blockGroup[0].superBlock.nFreeBlocks)) +
		string(" 块\n")+
		string("空闲i-结点总数：")+
		string(to_string(blockGroup[0].superBlock.nFreeInodes))+
		string(" 个\n") +
		string("目录总数：") +
		string(to_string(vecDir.size())) +
		string("个\n") +
		string("文件总数：") +
		string(to_string(vecFile.size())) +
		string("个\n"));
	cout << "模拟文件系统信息：\n\n" 
		<< "整体信息：\n" 
		<< "用户数量："
		<< users.size()
		<< " 个\n" << "磁盘容量："
		<< TOTAL_BLOCKS_NUM * BLOCK_SIZE
		<< " 字节\t"
		<< (float)TOTAL_BLOCKS_NUM / 1024
		<< " MB\n"
		<< "已用空间："
		<< (TOTAL_BLOCKS_NUM - blockGroup[0].superBlock.nFreeBlocks) * BLOCK_SIZE
		<< " 字节\t"
		<< (float)(TOTAL_BLOCKS_NUM - blockGroup[0].superBlock.nFreeBlocks) / 1024
		<< " MB\n"
		<< "可用空间："
		<< blockGroup[0].superBlock.nFreeBlocks * BLOCK_SIZE
		<< " 字节\t"
		<< (float)blockGroup[0].superBlock.nFreeBlocks / 1024
		<< " MB\n"
		<< "可用空间比例："
		<< ((float)blockGroup[0].superBlock.nFreeBlocks / (float)TOTAL_BLOCKS_NUM) * 100
		<< "%\n"
		<< "\n详细信息：\n"
		<< "盘块大小："
		<< BLOCK_SIZE
		<< " 字节\n"
		<< "每组盘块数："
		<< BLOCK_NUM_EACH_GROUP
		<< " 块\n"
		<< "每组i-结点数："
		<< INODES_NUM_EACH_GROUP
		<< " 个\n"
		<< "盘块总数："
		<< TOTAL_BLOCKS_NUM
		<< " 块\n"
		<< "i-结点总数："
		<< TOTAL_INODES_NUM
		<< " 个\n"
		<< "空闲块总数："
		<< blockGroup[0].superBlock.nFreeBlocks
		<< " 块\n"
		<< "空闲i-结点总数："
		<< blockGroup[0].superBlock.nFreeInodes
		<< " 个\n"
		<< "目录总数："
		<< vecDir.size()
		<< "个\n"
		<< "文件总数："
		<< vecFile.size()
		<< "个\n";
}

//退出系统
void cmd_exit()
{
	cout << "正在退出系统......\n";
	shellOutput(
		"正在退出系统......\n");
	saveDisk();
	exit(0);
}

//寻找命令
void execute_cmd(const char* command, const char* arg1, const char* arg2)
{
	int i, j;
	//打开虚拟磁盘将其中信息读入内存避免多个用户信息错乱
	fsVirtualDisk.open("../../virtual_file_system.bin", ios::in | ios::binary);
	//如果存在，则将虚拟磁盘中的数据读入内存
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)  //超级块信息
		fsVirtualDisk.read(reinterpret_cast<char*>(&blockGroup[i]), sizeof(block_group));
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)  //磁盘块位图信息
		for (j = 0; j < BLOCK_NUM_EACH_GROUP; j++)
			fsVirtualDisk.read(reinterpret_cast<char*>(&blockBitmap[i][j]), sizeof(bitmap));
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)  //i节点位图信息
		for (j = 0; j < INODES_NUM_EACH_GROUP; j++)
			fsVirtualDisk.read(reinterpret_cast<char*>(&inodeBitmap[i][j]), sizeof(bitmap));
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)  //i节点表信息
		for (j = 0; j < INODES_NUM_EACH_GROUP; j++)
			fsVirtualDisk.read(reinterpret_cast<char*>(&inodeTable[i][j]), sizeof(i_node));
	vecDir.clear();
	vecFile.clear();
	dir tempDir;
	file tempFile;
	emptyBlock eB;
	//将磁盘中的目录读入内存
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
	//将磁盘中的文件读入内存
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
			cout << "参数过多！\n";
			shellOutput(
				"参数过多！\n"
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
			cout << "参数过多！\n";
			shellOutput(
				"参数过多！\n"
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
			cout << "参数过多！\n";
			shellOutput(
				"参数过多！\n"
			);
			return;
		}
		if (presentUser.type == VISITOR)
		{
			cout << "权限不足！\n";
			shellOutput(
				"权限不足！\n"
			);
			return;
		}
		cout << "警告：初始化将重置模拟文件系统所有信息：（Y/N）\n";
		shellOutput(
			"警告：初始化将重置模拟文件系统所有信息：（Y/N）\n");
		string ifInit;
		char charifInit[MAX_COMMAND_LENGTH];
		shellInput(charifInit);
		ifInit = charifInit;
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
			shellOutput(
				"取消操作\n"
			);
		}
		break;
	}
	case 3://info
	{
		if (arg1[0] != '\0')
		{
			cout << "参数过多！\n";
			shellOutput(
				"参数过多！\n"
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
			cout << "参数过多！\n";
			shellOutput(
				"参数过多！\n"
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
			cout << "参数过多！\n";
			shellOutput(
				"参数过多！\n"
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
			cout << "输入参数过少！\n";
			shellOutput(
				"参数过少！\n"
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
			cout << "输入参数错误！\n";
			shellOutput(
				"参数错误！\n"
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
			cout << "参数错误！\n";
			shellOutput(
				"参数错误！\n"
			);
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
			shellOutput(
				"参数错误！\n"
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
			cout << "参数错误！\n";
			shellOutput(
				"参数错误！\n"
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
			cout << "参数错误！\n";
			shellOutput(
				"参数错误！\n"
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
			cout << "参数错误！\n";
			shellOutput(
				"参数错误！\n"
			);
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
			shellOutput(
				"参数错误！\n"
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
			cout << "参数过多！\n";
			shellOutput(
				"参数过多！\n"
			);
			return;
		}
		cmd_check();
	}
	case 13://register
	{
		if (arg1[0] != '\0')
		{
			cout << "参数过多！\n";
			shellOutput(
				"参数过多！\n"
			);
			return;
		}
		if (presentUser.type == VISITOR)
		{
			cout << "权限不足！\n";
			shellOutput(
				"权限不足！\n"
			);
			return;
		}
		cout << "是否继续注册用户：（Y/N）\n";
		shellOutput(
			"是否继续注册用户：（Y/N）\n"
		);
		string ifRegister;
		char charifRegister[MAX_COMMAND_LENGTH];
		shellInput(charifRegister);
		ifRegister = charifRegister;
		while (ifRegister == "Y" || ifRegister == "y")
		{
			char nme[32], pwd[32];
			cout << "\n请输入用户名：";
			shellOutput(
				"\n请输入用户名：");
			shellInput(nme);
			cout << "\n请输入用户密码：";
			shellOutput(
				"\n请输入用户密码：");
			shellInput(nme);
			users.push_back(user(nme, sizeof(users), VISITOR, pwd));
			cout << "\n用户创建成功！\n是否注册用户：（Y/N）";
			shellOutput(
				"\n用户创建成功！\n是否注册用户：（Y/N）");
			shellInput(charifRegister);
			ifRegister = charifRegister;
			cout << "是否继续注册用户：（Y/N）";
			shellOutput(
				"是否继续注册用户：（Y/N）\n"
			);
		}
		//存储用户信息到二进制文件
		cout << "\n正在存储用户信息......\n";
		shellOutput(
			"\n正在存储用户信息......\n"
		);
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
			shellOutput(
				"参数过多！\n"
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
			cout << "参数过多！\n";
			shellOutput(
				"参数过多！\n"
			);
			return;
		}
		cmd_show_files();
		return;
	}
	}
}