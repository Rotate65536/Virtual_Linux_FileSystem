#include"head.h"
void cmd_newfile(const char* fileName, file_authority authority) 
{
	int i;
	if (authority == PROTECTED && presentUser.type == VISITOR)
	{
		cout << "需要管理员权限！\n";
		return;
	}
	if (inodeTable[0][vecDir[currentPathDir].nInode].id != presentUser.id
		&& presentUser.type == VISITOR && inodeTable[0][vecDir[currentPathDir].nInode].id != 0)
	{
		cout << "非当前用户目录！\n";
		return;
	}
	cout << "正在新建文件\"" << fileName << "\"\n";
	for (i = 1; i < BLOCK_GROUPS_NUM; i++)
	{
		if (blockGroup[i].superBlock.nFreeBlocks > 0 && blockGroup[i].superBlock.nFreeInodes > 0)
			break;
	}
	cout << "输入文件内容：\n";
	string strContent;
	cin >> strContent;
	if (strContent.length() > 1019)
	{
		cout << "申请多个磁盘块......";
		//  未实现  //
	}
	else
	{
		cout << "申请一个磁盘块......";
		char content[MAX_NAME_LENGTH];
		strcpy(content, strContent.c_str());
		//更新i结点表、位图、超级块
		updateFileInfo(i, vecFile.size() % BLOCK_NUM_EACH_GROUP, fileName, typeFILE,
			authority, emptyFileBlockIndex, sizeof(file) - 1020 + sizeof(content));
		//更新当前目录信息
		vecDir[currentPathDir].nSubInodenGroup[vecDir[currentPathDir].nFile] = i;
		vecDir[currentPathDir].nSubInodenInode[vecDir[currentPathDir].nFile] = vecFile.size() % BLOCK_NUM_EACH_GROUP;
		vecDir[currentPathDir].nFile += 1;
		//更新空闲盘块索引
		emptyFileBlockIndex += sizeof(file);
		//更新内存文件信息
		vecFile.push_back(file(i, vecFile.size(), content));
		//更新磁盘
		saveDisk();
		cout << "完成！\n";
		return;
	}
}
void cmd_cat(const char* fileName) 
{
	//判断权限
	if (inodeTable[0][vecDir[currentPathDir].nInode].id != presentUser.id
		&& presentUser.type == VISITOR && inodeTable[0][vecDir[currentPathDir].nInode].id != 0)
	{
		cout << "非当前用户目录！\n";
		return;
	}
	//寻找文件
	int i, ifExist = 0;
	for (i = 0; i < vecDir[currentPathDir].nFile; i++)
	{
		if (strcmp(inodeTable[vecDir[currentPathDir].nSubInodenGroup[i]][vecDir[currentPathDir].nSubInodenInode[i]].strName,
			fileName) == 0)
		{
			ifExist = 1;
			break;
		}
	}
	if (ifExist);
	else
	{
		cout << "文件不存在！\n";
		return;
	}
	short tempNGroup = vecDir[currentPathDir].nSubInodenGroup[i],
		tempNInode = vecDir[currentPathDir].nSubInodenInode[i];
	//判断权限
	if ((inodeTable[tempNGroup][tempNInode].id != presentUser.id
		&& presentUser.type == VISITOR)
		||
		inodeTable[tempNGroup][tempNInode].authority == PROTECTED
		&& presentUser.type == VISITOR)
	{
		cout << "权限不足！\n";
		return;
	}
	//打开文件
	cout << "正在打开文件......\n";
	//一个文件占用多个磁盘块
	if (inodeTable[tempNGroup][tempNInode].nBlock > 1)
	{
		// 未完成 //
	}
	//一个文件占用一个磁盘块
	else
	{
		for (i = 0; i < vecFile.size(); i++)
		{
			if (vecFile[i].nGroup == tempNGroup && vecFile[i].nInode == tempNInode)
				break;
		}
		cout << "文件内容：\n";
		cout << vecFile[i].content << endl;
	}
}

void cmd_copy(const char* srcPath, const char* desPath)
{
	if (inodeTable[0][vecDir[currentPathDir].nInode].id != presentUser.id
		&& presentUser.type == VISITOR && inodeTable[0][vecDir[currentPathDir].nInode].id != 0)
	{
		cout << "非当前用户目录！\n";
		return;
	}
	char ifSrcHost[7], ifDesHost[7];
	strncpy(ifSrcHost, srcPath, 6);
	ifSrcHost[6] = '\0';
	ifDesHost[6] = '\0';
	strncpy(ifDesHost, desPath, 6);
	if (strcmp(ifSrcHost, "<host>") == 0)
	{
		int i, j;
		cout << "正从宿主系统复制文件到虚拟文件系统......\n";
		string strSrcPath = srcPath;
		string hostPath = string(strSrcPath, 5, strSrcPath.length() - 1);
		ifstream input(hostPath, ios::in);
		char* hostContent;
		//假设只需占用一个磁盘块
		hostContent = (char*)malloc(sizeof(char) * 1019);
		int ifMoreBlock = 0, countChar = 0;
		char buff[1019];
		int pt = 0;
		while (input.getline(buff, 1019))
		{
			for (i = 0; i < 1019; i++) {
				if (buff[i] == '\0' || countChar >= 1019)
				{
					hostContent[pt] = '\n';
					pt++;
					countChar++;
					ifMoreBlock = 1;
					break;
				}
				hostContent[pt] = buff[i];
				pt++;
				countChar++;
			}
		}
		hostContent[pt] = '\0';
		if (ifMoreBlock)
		{
			// 占用多个磁盘块 //
		}
		else
		{
			cmd_cd(desPath);
			if (inodeTable[0][vecDir[currentPathDir].nInode].id != presentUser.id
				&& presentUser.type == VISITOR && inodeTable[0][vecDir[currentPathDir].nInode].id != 0)
			{
				cout << "非当前用户目录！\n";
				return;
			}
			cout << "正在复制文件\"" << srcPath << "\"\n";
			for (i = 1; i < BLOCK_GROUPS_NUM; i++)
			{
				if (blockGroup[i].superBlock.nFreeBlocks > 0 && blockGroup[i].superBlock.nFreeInodes > 0)
					break;
			}
			cout << "申请一个磁盘块......";
			char content[1020];
			strcpy(content, hostContent);
			//更新i结点表、位图、超级块
			updateFileInfo(i, vecFile.size() % BLOCK_NUM_EACH_GROUP, hostPath.c_str(), typeFILE,
				READ_WRITE, emptyFileBlockIndex, sizeof(file) - 1020 + sizeof(content));
			//更新当前目录信息
			vecDir[currentPathDir].nSubInodenGroup[vecDir[currentPathDir].nFile] = i;
			vecDir[currentPathDir].nSubInodenInode[vecDir[currentPathDir].nFile] = vecFile.size() % BLOCK_NUM_EACH_GROUP;
			vecDir[currentPathDir].nFile += 1;
			//更新空闲盘块索引
			emptyFileBlockIndex += sizeof(file);
			//更新内存文件信息
			vecFile.push_back(file(i, vecFile.size(), content));
			//更新磁盘
			saveDisk();
			cout << "完成！\n";
			return;
		}
	}
	else if (strcmp(ifDesHost, "<host>") == 0)
	{
		cout << "正从虚拟文件系统复制文件到宿主系统......\n";
		// 未要求完成 //
	}
	else
	{
		//先去到文件夹才能操作
		int i, j, ifExist = 0;
		for (i = 0; i < vecDir[currentPathDir].nFile; i++)
		{
			if (strcmp(inodeTable[vecDir[currentPathDir].nSubInodenGroup[i]][vecDir[currentPathDir].nSubInodenInode[i]].strName,
				srcPath) == 0)
			{
				ifExist = 1;
				break;
			}
		}
		if (ifExist);
		else
		{
			cout << "文件不存在！\n";
			return;
		}
		short tempNGroup = vecDir[currentPathDir].nSubInodenGroup[i],
			tempNInode = vecDir[currentPathDir].nSubInodenInode[i];
		//如果占用磁盘块大于1
		if (inodeTable[tempNGroup][tempNInode].nBlock > 1)
		{
			// 未完成 //
		}
		else
		{
			//判断权限
			if ((inodeTable[tempNGroup][tempNInode].id != presentUser.id
				&& presentUser.type == VISITOR)
				||
				inodeTable[tempNGroup][tempNInode].authority == PROTECTED
				&& presentUser.type == VISITOR)
			{
				cout << "权限不足！\n";
				return;
			}
			//for (i = 0; i < vecFile.size(); i++)
			//{
			//	if (vecFile[i].nGroup == tempNGroup && vecFile[i].nInode == tempNInode)
			//		break;
			//}
			//int indexFile = i;
			cmd_cd(desPath);
			if (inodeTable[0][vecDir[currentPathDir].nInode].id != presentUser.id
				&& presentUser.type == VISITOR && inodeTable[0][vecDir[currentPathDir].nInode].id != 0)
			{
				cout << "非当前用户目录！\n";
				return;
			}
			cout << "正在复制文件\"" << srcPath << "\"\n";
			for (i = 1; i < BLOCK_GROUPS_NUM; i++)
			{
				if (blockGroup[i].superBlock.nFreeBlocks > 0 && blockGroup[i].superBlock.nFreeInodes > 0)
					break;
			}
			for (j = 0; j < vecFile.size(); j++)
			{
				if (vecFile[j].nGroup == tempNGroup && vecFile[j].nInode == tempNInode)
					break;
			}
			cout << "申请一个磁盘块......";
			char content[1020];
			strcpy(content, vecFile[j].content);
			//更新i结点表、位图、超级块
			updateFileInfo(i, vecFile.size() % BLOCK_NUM_EACH_GROUP, inodeTable[tempNGroup][tempNInode].strName, typeFILE,
				inodeTable[tempNGroup][tempNInode].authority, emptyFileBlockIndex, sizeof(file) - 1020 + sizeof(content));
			//更新当前目录信息
			vecDir[currentPathDir].nSubInodenGroup[vecDir[currentPathDir].nFile] = i;
			vecDir[currentPathDir].nSubInodenInode[vecDir[currentPathDir].nFile] = vecFile.size() % BLOCK_NUM_EACH_GROUP;
			vecDir[currentPathDir].nFile += 1;
			//更新空闲盘块索引
			emptyFileBlockIndex += sizeof(file);
			//更新内存文件信息
			vecFile.push_back(file(i, vecFile.size(), content));
			//更新磁盘
			saveDisk();
			cout << "完成！\n";
			return;
		}
	}
}


void cmd_del(const char* fileName) 
{
	//判断权限
	if (inodeTable[0][vecDir[currentPathDir].nInode].id != presentUser.id
		&& presentUser.type == VISITOR && inodeTable[0][vecDir[currentPathDir].nInode].id != 0)
	{
		cout << "非当前用户目录！\n";
		return;
	}
	//寻找文件
	int i, ifExist = 0;
	for (i = 0; i < vecDir[currentPathDir].nFile; i++)
	{
		if (strcmp(inodeTable[vecDir[currentPathDir].nSubInodenGroup[i]][vecDir[currentPathDir].nSubInodenInode[i]].strName,
			fileName) == 0)
		{
			ifExist = 1;
			break;
		}
	}
	if (ifExist);
	else
	{
		cout << "文件不存在！\n";
		return;
	}
	short tempNGroup = vecDir[currentPathDir].nSubInodenGroup[i],
		tempNInode = vecDir[currentPathDir].nSubInodenInode[i];
	//判断权限
	if ((inodeTable[tempNGroup][tempNInode].id != presentUser.id
		&& presentUser.type == VISITOR)
		||
		inodeTable[tempNGroup][tempNInode].authority == PROTECTED
		&& presentUser.type == VISITOR)
	{
		cout << "权限不足！\n";
		return;
	}
	//v.erase(v.begin() + 2);
	//一个文件占用多个磁盘块
	if (inodeTable[tempNGroup][tempNInode].nBlock > 1)
	{
		// 未完成 //
	}
	//一个文件占用一个磁盘块
	else
	{
		for (i = 0; i < vecFile.size(); i++)
		{
			if (vecFile[i].nGroup == tempNGroup && vecFile[i].nInode == tempNInode)
				break;
		}
		cout << "正在删除文件：\"" << inodeTable[tempNGroup][tempNInode].strName << "\"\n";
		//更新i结点表、位图、超级块
		inodeTable[tempNGroup][tempNInode].createTime = time_t();
		inodeTable[tempNGroup][tempNInode].id = 0;
		inodeTable[tempNGroup][tempNInode].authority = READ_WRITE;
		inodeTable[tempNGroup][tempNInode].type = typeBlock;
		inodeTable[tempNGroup][tempNInode].addrBlock = -1;
		inodeTable[tempNGroup][tempNInode].size = 0;
		blockGroup[tempNGroup].superBlock.nFreeBlocks += inodeTable[tempNGroup][tempNInode].nBlock;
		inodeTable[tempNGroup][tempNInode].nBlock = 0;  
		inodeBitmap[tempNGroup][tempNInode] = EMPTY;
		blockBitmap[tempNGroup][tempNInode] = EMPTY;
		blockGroup[tempNGroup].superBlock.nFreeInodes += 1;
		//更新目录信息
		vecDir[currentPathDir].nFile -= 1;
		//清楚内存对应元素
		vecFile.erase(vecFile.begin() + i);
		cout << "文件已删除！\n";
		saveDisk();
	}
	return;
}

void del(int thisPath, const char* fileName)
{
	currentPathDir;
	//判断权限
	if (inodeTable[0][vecDir[thisPath].nInode].id != presentUser.id
		&& presentUser.type == VISITOR && inodeTable[0][vecDir[thisPath].nInode].id != 0)
	{
		cout << "非当前用户目录！\n";
		return;
	}
	//寻找文件
	int i, ifExist = 0;
	for (i = 0; i < vecDir[thisPath].nFile; i++)
	{
		if (strcmp(inodeTable[vecDir[thisPath].nSubInodenGroup[i]][vecDir[thisPath].nSubInodenInode[i]].strName,
			fileName) == 0)
		{
			ifExist = 1;
			break;
		}
	}
	if (ifExist);
	else
	{
		cout << "文件不存在！\n";
		return;
	}
	short tempNGroup = vecDir[thisPath].nSubInodenGroup[i],
		tempNInode = vecDir[thisPath].nSubInodenInode[i];
	//判断权限
	if ((inodeTable[tempNGroup][tempNInode].id != presentUser.id
		&& presentUser.type == VISITOR)
		||
		inodeTable[tempNGroup][tempNInode].authority == PROTECTED
		&& presentUser.type == VISITOR)
	{
		cout << "权限不足！\n";
		return;
	}
	//v.erase(v.begin() + 2);
	//一个文件占用多个磁盘块
	if (inodeTable[tempNGroup][tempNInode].nBlock > 1)
	{
		// 未完成 //
	}
	//一个文件占用一个磁盘块
	else
	{
		for (i = 0; i < vecFile.size(); i++)
		{
			if (vecFile[i].nGroup == tempNGroup && vecFile[i].nInode == tempNInode)
				break;
		}
		cout << "正在删除文件：\"" << inodeTable[tempNGroup][tempNInode].strName << "\"\n";
		//更新i结点表、位图、超级块
		inodeTable[tempNGroup][tempNInode].createTime = time_t();
		inodeTable[tempNGroup][tempNInode].id = 0;
		inodeTable[tempNGroup][tempNInode].authority = READ_WRITE;
		inodeTable[tempNGroup][tempNInode].type = typeBlock;
		inodeTable[tempNGroup][tempNInode].addrBlock = -1;
		inodeTable[tempNGroup][tempNInode].size = 0;
		blockGroup[tempNGroup].superBlock.nFreeBlocks += inodeTable[tempNGroup][tempNInode].nBlock;
		inodeTable[tempNGroup][tempNInode].nBlock = 0;
		inodeBitmap[tempNGroup][tempNInode] = EMPTY;
		blockBitmap[tempNGroup][tempNInode] = EMPTY;
		blockGroup[tempNGroup].superBlock.nFreeInodes += 1;
		//更新目录信息
		vecDir[thisPath].nFile -= 1;
		//清楚内存对应元素
		vecFile.erase(vecFile.begin() + i);
		cout << "文件已删除！\n";
		saveDisk();
	}
	return;
}