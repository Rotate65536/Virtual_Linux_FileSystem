#include"head.h"
void cmd_show_files()
{
	cout << "目录数量：" << vecDir.size() << endl;
	for (int i = 0; i < vecDir.size(); i++)
	{
		cout << inodeTable[vecDir[i].nGroup][vecDir[i].nInode].strName << endl;
	}
	cout << "文件数量：" << vecFile.size() << endl;
	for (int i = 0; i < vecFile.size(); i++)
	{
		cout << inodeTable[vecFile[i].nGroup][vecFile[i].nInode].strName << endl;
	}
}

void cmd_cd(const char* path)
{
	int i, ifExist = 0;
	for (i = 0; i < vecDir.size(); i++)
	{
		if (strcmp(inodeTable[vecDir[i].nGroup][vecDir[i].nInode].strName, path) == 0)
		{
			ifExist = 1;
			break;
		}
	}
	if (ifExist);
	else
	{
		cout << "路径不存在！\n";
		return;
	}
	strcpy(currentPath, inodeTable[vecDir[i].nGroup][vecDir[i].nInode].strName);
	currentPathDir = i;
}

void cmd_dir(const char* path)
{
	//寻找路径
	int i, j, ifExist = 0;
	for (i = 0; i < vecDir.size(); i++)
	{
		if (strcmp(inodeTable[vecDir[i].nGroup][vecDir[i].nInode].strName, path) == 0)
		{
			ifExist = 1;
			break;
		}
	}
	if (ifExist);
	else
	{
		cout << "路径不存在！\n";
		return;
	}
	//判断权限
	if (inodeTable[0][vecDir[i].nInode].id != presentUser.id
		&& presentUser.type == VISITOR && inodeTable[0][vecDir[i].nInode].id != 0)
	{
		cout << "非当前用户目录！\n";
		return;
	}
	//显示信息
	for (j = 0; j < vecDir[currentPathDir].nFile; j++)
	{
		inodeTable[vecDir[currentPathDir].nSubInodenGroup[j]][vecDir[currentPathDir].nSubInodenInode[j]];
		cout << "文件名："
			<< inodeTable[vecDir[currentPathDir].nSubInodenGroup[j]][vecDir[currentPathDir].nSubInodenInode[j]].strName
			<< "\n创建时间：" << (char*)(ctime(&inodeTable[vecDir[currentPathDir].nSubInodenGroup[j]][vecDir[currentPathDir].nSubInodenInode[j]].createTime))
			<< "所有者id：" << inodeTable[vecDir[currentPathDir].nSubInodenGroup[j]][vecDir[currentPathDir].nSubInodenInode[j]].id
			<< "\n地址：" << inodeTable[vecDir[currentPathDir].nSubInodenGroup[j]][vecDir[currentPathDir].nSubInodenInode[j]].addrBlock;
		switch (inodeTable[vecDir[currentPathDir].nSubInodenGroup[j]][vecDir[currentPathDir].nSubInodenInode[j]].authority)
		{
		case PROTECTED:
		{
			cout << "\n权限：" << "系统";
			break;
		}
		case READ_WRITE:
		{
			cout << "\n权限：" << "读写";
			break;
		}
		case READ_ONLY:
		{
			cout << "\n权限：" << "只读";
			break;
		}
		};
		switch (inodeTable[vecDir[currentPathDir].nSubInodenGroup[j]][vecDir[currentPathDir].nSubInodenInode[j]].type)
		{
		case typeFILE:
		{
			cout << "\n类型：" << "文件\n\n";
			break;
		}
		case typeDIRECTORY:
		{
			cout << "\n类型：" << "目录\n\n";
			break;
		}
		case typeBlock:
		{
			cout << "\n类型：" << "空块\n\n";
			break;
		}
		};
	}
	return;
}

void cmd_md(const char* fileName, file_authority authority)
{
	//判断权限
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
	cout << "正在新建目录\"";
	//更新i结点表
	char filePathName[MAX_NAME_LENGTH];
	strcpy(filePathName, inodeTable[vecDir[currentPathDir].nGroup][vecDir[currentPathDir].nInode].strName);
	strcat(filePathName, "/");
	strcat(filePathName, (char*)fileName);
	cout << filePathName << "\"\n";
	updateFileInfo(0, vecDir.size(), (char*)filePathName, typeDIRECTORY, authority,
		emptyDirBlockIndex, sizeof(dir) - 1016);
	//更新当前目录信息
	vecDir[currentPathDir].nSubInodenGroup[vecDir[currentPathDir].nFile] = 0;
	vecDir[currentPathDir].nSubInodenInode[vecDir[currentPathDir].nFile] = vecDir.size();
	vecDir[currentPathDir].nFile += 1;
	vecDir.push_back(dir(0, vecDir.size()));
	emptyDirBlockIndex += sizeof(dir);
	//更新磁盘
	saveDisk();
	cout << "完成！\n";
	return;
}

void cmd_rd(const char* path, int thisPath)
{
	int i, j, ifExist = 0;
	if (inodeTable[0][vecDir[currentPathDir].nInode].id != presentUser.id
		&& presentUser.type == VISITOR && inodeTable[0][vecDir[currentPathDir].nInode].id != 0)
	{
		cout << "非当前用户目录！\n";
		return;
	}
	for (i = 0; i < vecDir[thisPath].nFile; i++)
	{
		if (strcmp(inodeTable[vecDir[thisPath].nSubInodenGroup[i]][vecDir[currentPathDir].nSubInodenInode[i]].strName,
			path) == 0)
		{
			ifExist = 1;
			break;
		}
	}
	if (ifExist);
	else
	{
		cout << "路径不存在！\n";
		return;
	}
	short tempNGroup = vecDir[thisPath].nSubInodenGroup[i],
		tempNInode = vecDir[thisPath].nSubInodenInode[i];
	int thisP = i;
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

	for (i = 0; i < vecDir.size(); i++)
	{
		if (vecDir[i].nGroup == tempNGroup && vecDir[i].nInode == tempNInode)
			break;
	}

	if (vecDir[i].nFile > 0)
	{
		cout << "警告！目录：" << inodeTable[vecDir[i].nGroup][vecDir[i].nInode].strName << "非空，是否继续：（Y/N）";
		string ifContinue;
		cin >> ifContinue;
		if (ifContinue == "Y" || ifContinue == "y");
		else
		{
			cout << "取消操作！\n";
			return;
		}
	}
	for (j = 0; j < vecDir[i].nFile; j++)
	{
		//如果是文件类型
		if (inodeTable[vecDir[i].nSubInodenGroup[j]][vecDir[i].nSubInodenInode[j]].type == typeFILE)
		{
			del(i, inodeTable[vecDir[i].nSubInodenGroup[j]][vecDir[i].nSubInodenInode[j]].strName);
		}
		//如果是目录类型
		else if (inodeTable[vecDir[i].nSubInodenGroup[j]][vecDir[i].nSubInodenInode[j]].type == typeDIRECTORY)
		{
			cmd_rd(inodeTable[vecDir[i].nSubInodenGroup[j]][vecDir[i].nSubInodenInode[j]].strName, i);
		}
		else;
	}
	//删除目录
	cout << "目录：\"" << inodeTable[tempNGroup][tempNInode].strName << "\"\n";
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
	vecDir.erase(vecDir.begin() + i);
	cout << "目录已删除！\n";
	saveDisk();
}