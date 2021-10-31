#include"head.h"

//更新磁盘信息
void updateFileInfo(int indexGroup, int index, const char* fileName, file_type type, file_authority authority, int blockAddr, int size) 
{
	strcpy(inodeTable[indexGroup][index].strName, fileName);
	cout << "更新目录：" << inodeTable[indexGroup][index].strName
		<< "的i结点表、位图、超级块......\n";
	time_t now;
	now = time(&now);
	inodeTable[indexGroup][index].createTime = now;  //初始化时间
	inodeTable[indexGroup][index].id = 0;  //初始化id
	inodeTable[indexGroup][index].authority = authority;  //初始化读写全南县
	inodeTable[indexGroup][index].type = type;  //初始化文件类型 
	inodeTable[indexGroup][index].addrBlock = blockAddr;  //初始化磁盘块位置
	inodeTable[indexGroup][index].size = size;  //初始化文件大小，只考虑有效字节数而非多余的空间
	inodeTable[indexGroup][index].nBlock = 1;  //暂定每个文件只占用一个块
	inodeBitmap[indexGroup][index] = OCCUPIED;  //更新位图
	blockBitmap[indexGroup][index] = OCCUPIED;  //更新位图
	blockGroup[indexGroup].superBlock.nFreeBlocks -= inodeTable[indexGroup][index].nBlock;
	blockGroup[indexGroup].superBlock.nFreeInodes -= 1;
}

void saveDisk()
{
	int i, j;
	fsVirtualDisk.open("../../virtual_file_system.bin", ios::out | ios::binary);
	if (!fsVirtualDisk.is_open())
	{
		cout << "无法打开虚拟磁盘！\n";
		exit(0);
	}
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)
		fsVirtualDisk.write(reinterpret_cast<char*>(&blockGroup[i]), sizeof(block_group));
	//对磁盘块位图和i结点位图初始化
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)
		for (j = 0; j < BLOCK_NUM_EACH_GROUP; j++)
			fsVirtualDisk.write(reinterpret_cast<char*>(&blockBitmap[i][j]), sizeof(bitmap));
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)
		for (j = 0; j < INODES_NUM_EACH_GROUP; j++)
			fsVirtualDisk.write(reinterpret_cast<char*>(&inodeBitmap[i][j]), sizeof(bitmap));
	//对i结点表初始化
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)
		for (j = 0; j < INODES_NUM_EACH_GROUP; j++)
			fsVirtualDisk.write(reinterpret_cast<char*>(&inodeTable[i][j]), sizeof(i_node));
	cout << "正在初始化磁盘空间......\n";
	emptyBlock eB;
	//第1个磁盘块组专门分给目录文件
	for (j = 0; j < BLOCK_NUM_EACH_GROUP; j++)
	{
		if (j < vecDir.size())
		{
			fsVirtualDisk.write(reinterpret_cast<char*>(&vecDir[j]), sizeof(dir));
		}
		else
			fsVirtualDisk.write(reinterpret_cast<char*>(&eB), sizeof(emptyBlock));
	}
	//其他磁盘块组分给其他文件
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
	cout << "正在初始化模拟文件系统......\n";
	int i, j;
	//对每一个盘块组的磁盘块初始化
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)
	{
		blockGroup[i].superBlock.nFreeBlocks = TOTAL_BLOCKS_NUM - 1;
		blockGroup[i].superBlock.nFreeInodes = TOTAL_INODES_NUM - 1;
	}
	//对磁盘块位图和i结点位图初始化
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)
		for (j = 0; j < BLOCK_NUM_EACH_GROUP; j++)
			blockBitmap[i][j] = EMPTY;
	for (i = 0; i < BLOCK_GROUPS_NUM; i++)
		for (j = 0; j < INODES_NUM_EACH_GROUP; j++)
			inodeBitmap[i][j] = EMPTY;
	//对i结点表初始化
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

	//新建root目录文件——更新bitmap、inodeTable、superblock，将root文件写入内存并写入磁盘
	vecDir.push_back(dir(0, 0));
	updateFileInfo(0, 0, (char*)"root", typeDIRECTORY, READ_WRITE, DATA_AREA_ADDR, sizeof(dir) - 1016);
	emptyDirBlockIndex += sizeof(dir);

	//将基本信息保存到.bin文件中
	saveDisk(); //blockGroup + inodeBitmap + inodeTable + blockBitmap + block

	currentPathDir = 0;
	strcpy(currentPath, inodeTable[vecDir[currentPathDir].nGroup][vecDir[currentPathDir].nInode].strName);
	//初始化System用户
	users.push_back(user((char*)"SYSTEM", 0, SYSTEM, (char*)"123456"));
	cout << "初始化完成！\n请设置管理员密码：";
	char admPassword[32];
	cin >> admPassword;
	users.push_back(user((char*)"ADMIN", 1, ADMINISTRATOR, admPassword));
	cout << "初始化完成！\n是否注册用户：（Y/N）";
	string ifRegister;
	cin >> ifRegister;
	while (ifRegister == "Y" || ifRegister == "y")
	{
		char nme[32], pwd[32];
		cout << "请输入用户名：";
		cin >> nme;
		cout << "请输入用户密码：";
		cin >> pwd;
		users.push_back(user(nme, sizeof(users), VISITOR, pwd));
		cout << "用户创建成功！\n是否注册用户：（Y/N）";
		cin >> ifRegister;
	}
	cout << "正在存储用户信息......\n";
	fstream outfile;
	outfile.open("../../users.dat", ios::out);
	for (i = 0; i < users.size(); i++)
		outfile.write((char*)&users[i], sizeof(user));
	outfile.close();
	cout << "初始化完成！转入登陆界面......\n";
}

void loadDisk()
{
	int i, j;
	system("cls");  //清楚屏幕
	fsVirtualDisk.open("../../virtual_file_system.bin", ios::in | ios::binary);
	if (!fsVirtualDisk.is_open())  //不存在模拟文件系统虚拟磁盘
	{
		cout << "模拟文件系统未初始化\n"
			<< "是否初始化模拟文件系统：（Y/N）";
		string choiceIfInit;
		cin >> choiceIfInit;
		if (choiceIfInit == "n" || choiceIfInit == "N")  //不初始化模拟文件系统，退出
		{
			cout << "初始化模拟文件系统失败，退出中......\n";
			exit(0);  //无法初始化文件系统，退出
		}
		else  //初始化文件系统，调用cmd_init()，然后return
		{
			fsVirtualDisk.clear();
			initFileSystem();
			return;
		}
	}
	cout << "正在初始化文件系统......\n"
		<< "正在读入磁盘数据......\n";
	//如果存在，则将虚拟磁盘中的数据读入内存
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
	currentPathDir = 0;
	strcpy(currentPath, inodeTable[vecDir[currentPathDir].nGroup][vecDir[currentPathDir].nInode].strName);
	fsVirtualDisk.close();
	cout << "正在读入用户信息......\n";
	//将用户信息读入内存
	fstream infile;
	infile.open("../../users.dat", ios::in | ios::out);
	users.clear();  //清空vector元素
	user sb;
	while (infile.read((char*)&sb, sizeof(sb))) { //一直读到文件结束
		users.push_back(sb);
	}
	cout << "模拟文件系统初始化成功！\n";
}

void cmd_check()
{
	if (presentUser.type == VISITOR)
	{
		cout << "需要管理员权限！\n";
		return;
	}
	cout << "正在检测模拟文件系统数据一致性......";
	int i, j;
	fsVirtualDisk.open("../../virtual_file_system.bin", ios::in | ios::binary);
	block_group checkBlockGroup[BLOCK_GROUPS_NUM];  //磁盘块组
	i_node checkInodeTable[BLOCK_GROUPS_NUM][INODES_NUM_EACH_GROUP];  //i结点表
	bitmap checkInodeBitmap[BLOCK_GROUPS_NUM][INODES_NUM_EACH_GROUP];  //i结点位图
	bitmap checkBlockBitmap[BLOCK_GROUPS_NUM][BLOCK_NUM_EACH_GROUP];  //磁盘位图
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
		cout << "检测完毕，未发现问题！\n";
		return;
	}
	else
	{
		cout << "模拟文件系统不满足一致性！\n开始恢复......";
		saveDisk();
		return;
	}
}