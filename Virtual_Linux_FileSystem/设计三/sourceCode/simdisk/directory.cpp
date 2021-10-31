#include"head.h"
void cmd_show_files()
{
	//显示所有目录文件和普通文件的数量，用于debug
	cout << "目录数量：" << vecDir.size() << endl;
	int strSize = vecDir.size();
	string strShellOutput = "目录数量：" + to_string(strSize) + "\n";
	shellOutput(strShellOutput);
	for (int i = 0; i < vecDir.size(); i++)
	{
		cout << inodeTable[vecDir[i].nGroup][vecDir[i].nInode].strName << endl;
	}
	cout << "文件数量：" << vecFile.size() << endl;
	strSize = vecFile.size();
	strShellOutput = "文件数量：" + to_string(strSize) + "\n";
	shellOutput(strShellOutput);
	for (int i = 0; i < vecFile.size(); i++)
	{
		cout << inodeTable[vecFile[i].nGroup][vecFile[i].nInode].strName << endl;
	}
}

void cmd_cd(const char* path)
{
	int i, ifExist = 0;
	for (i = 0; i < vecDir.size(); i++)  //寻找目标目录
	{
		if (strcmp(inodeTable[vecDir[i].nGroup][vecDir[i].nInode].strName, path) == 0)
		{
			ifExist = 1;
			break;
		}
	}
	if (ifExist);  //存在
	else  //不存在
	{
		cout << "路径不存在！\n";
		shellOutput("路径不存在！\n");
		return;
	}
	//判断用户权限
	if (presentUser.type == VISITOR && inodeTable[0][vecDir[i].nInode].authority == PROTECTED)
	{
		cout << "游客无法访问系统目录！\n";
		shellOutput("游客无法访问系统目录！\n");
		return;
	}
	//更新当前目录 
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
	if (ifExist);  //存在
	else  //不存在
	{
		cout << "路径不存在！\n";
		shellOutput("路径不存在！\n");
		return;
	}
	//判断用户权限
	if (presentUser.type == VISITOR && inodeTable[0][vecDir[i].nInode].authority == PROTECTED)
	{
		cout << "游客无法访问系统目录！\n";
		shellOutput("游客无法访问系统目录！\n");
		return;
	}
	//显示信息
	if (readInode(0, vecDir[currentPathDir].nInode, true) == false)  //查看是否可读
		return;  //若不可读则退出
	for (j = 0; j < vecDir[currentPathDir].nFile; j++)
	{
		inodeTable[vecDir[currentPathDir].nSubInodenGroup[j]][vecDir[currentPathDir].nSubInodenInode[j]];
		cout << "文件名："
			<< inodeTable[vecDir[currentPathDir].nSubInodenGroup[j]][vecDir[currentPathDir].nSubInodenInode[j]].strName
			<< "\n创建时间：" << (char*)(ctime(&inodeTable[vecDir[currentPathDir].nSubInodenGroup[j]][vecDir[currentPathDir].nSubInodenInode[j]].createTime))
			<< "所有者id：" << inodeTable[vecDir[currentPathDir].nSubInodenGroup[j]][vecDir[currentPathDir].nSubInodenInode[j]].id
			<< "\n地址：" << inodeTable[vecDir[currentPathDir].nSubInodenGroup[j]][vecDir[currentPathDir].nSubInodenInode[j]].addrBlock;
		string strShellOutput = "文件名："
			+ string(inodeTable[vecDir[currentPathDir].nSubInodenGroup[j]][vecDir[currentPathDir].nSubInodenInode[j]].strName)
			+ "\n创建时间：" + string((char*)(ctime(&inodeTable[vecDir[currentPathDir].nSubInodenGroup[j]][vecDir[currentPathDir].nSubInodenInode[j]].createTime)))
			+ "所有者id：" + to_string(inodeTable[vecDir[currentPathDir].nSubInodenGroup[j]][vecDir[currentPathDir].nSubInodenInode[j]].id)
			+ "\n地址：" + to_string(inodeTable[vecDir[currentPathDir].nSubInodenGroup[j]][vecDir[currentPathDir].nSubInodenInode[j]].addrBlock);
		shellOutput(strShellOutput);
		switch (inodeTable[vecDir[currentPathDir].nSubInodenGroup[j]][vecDir[currentPathDir].nSubInodenInode[j]].authority)
		{
		case PROTECTED:
		{
			cout << "\n权限：系统";
			shellOutput("\n权限：系统");
			break;
		}
		case READ_WRITE:
		{
			cout << "\n权限：" << "读写";
			shellOutput("\n权限：读写");
			break;
		}
		case READ_ONLY:
		{
			cout << "\n权限：" << "只读";
			shellOutput("\n权限：只读");
			break;
		}
		};
		switch (inodeTable[vecDir[currentPathDir].nSubInodenGroup[j]][vecDir[currentPathDir].nSubInodenInode[j]].type)
		{
		case typeFILE:
		{
			cout << "\n类型：" << "文件\n\n";
			shellOutput("\n类型：文件\n\n");
			break;
		}
		case typeDIRECTORY:
		{
			cout << "\n类型：" << "目录\n\n";
			shellOutput("\n类型：目录\n\n");
			break;
		}
		case typeBlock:
		{
			cout << "\n类型：" << "空块\n\n";
			shellOutput("\n类型：空块\n\n");
			break;
		}
		};
	}
	string choiceIfExit = "N";
	//cin >> choiceIfInit;
	char charChoiceIfExit[MAX_COMMAND_LENGTH];
	while (choiceIfExit == "N" || choiceIfExit == "n")
	{
		cout << "是否退出查看？（Y/N）";
		shellOutput("是否退出查看？（Y/N）");
		shellInput(charChoiceIfExit);
		choiceIfExit = charChoiceIfExit;
	}
	readInode(0, vecDir[currentPathDir].nInode, false);  //退出读，更新互斥量
	return;
}

void cmd_md(const char* fileName, file_authority authority)
{
	//判断权限
	if (authority == PROTECTED && presentUser.type == VISITOR)
	{
		cout << "创建系统目录需要管理员权限！\n";
		shellOutput("创建系统目录需要管理员权限！\n");
		return;
	}
	if (presentUser.type == VISITOR 
		&& inodeTable[0][vecDir[currentPathDir].nInode].id != presentUser.id 
		&& inodeTable[0][vecDir[currentPathDir].nInode].id != 0)
	{
		cout << "非当前用户目录！\n";
		shellOutput("非当前用户目录！\n");
		return;
	}
	if (presentUser.type == VISITOR
		&& inodeTable[0][vecDir[currentPathDir].nInode].authority == PROTECTED)
	{
		cout << "当前目录为系统目录！\n";
		shellOutput("当前目录为系统目录！\n");
		return;
	}
	if (writeInode(0, vecDir[currentPathDir].nInode, true) == false)  //判断是否可写
		return;
	cout << "正在新建目录\"";
	shellOutput("正在新建目录\"");
	//更新i结点表
	char filePathName[MAX_NAME_LENGTH];
	strcpy(filePathName, inodeTable[vecDir[currentPathDir].nGroup][vecDir[currentPathDir].nInode].strName);
	strcat(filePathName, "/");
	strcat(filePathName, (char*)fileName);
	cout << filePathName << "\"\n";
	string strShellOutput = string(filePathName) + "\"\n";
	shellOutput(strShellOutput);
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
	shellOutput("完成！\n");
	writeInode(0, vecDir[currentPathDir].nInode, false);
	return;
}

void cmd_rd(const char* path, int thisPath)
{
	int i, j, ifExist = 0;
	string strShellOutput;
	for (i = 0; i < vecDir[thisPath].nFile; i++)  //寻找目录
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
		shellOutput("路径不存在！\n");
		return;
	}

	//存储i节点号
	short tempNGroup = vecDir[thisPath].nSubInodenGroup[i],
		tempNInode = vecDir[thisPath].nSubInodenInode[i];
	short indexSub = i;
	int thisP = i;
	//判断权限
	if ((inodeTable[tempNGroup][tempNInode].id != presentUser.id
		&& presentUser.type == VISITOR)
		||
		inodeTable[tempNGroup][tempNInode].authority == PROTECTED
		&& presentUser.type == VISITOR)
	{
		cout << "权限不足！\n";
		shellOutput("权限不足！\n");
		return;
	}

	for (i = 0; i < vecDir.size(); i++)
	{
		if (vecDir[i].nGroup == tempNGroup && vecDir[i].nInode == tempNInode)
			break;
	}

	if (writeInode(0, vecDir[currentPathDir].nInode, true) == false)
		return;
	if (vecDir[i].nFile > 0)
	{
		cout << "警告！目录：" << inodeTable[vecDir[i].nGroup][vecDir[i].nInode].strName << "非空，是否继续：（Y/N）";
		strShellOutput = "警告！目录：" + string(inodeTable[vecDir[i].nGroup][vecDir[i].nInode].strName) + "非空，是否继续：（Y/N）";
		shellOutput(strShellOutput);
		string ifContinue;
		//cin >> ifContinue;
		char charifContinue[MAX_COMMAND_LENGTH];
		shellInput(charifContinue);
		ifContinue = charifContinue;
		if (ifContinue == "Y" || ifContinue == "y");
		else
		{
			cout << "取消操作！\n";
			shellOutput("取消操作！\n");
			//writeInode(0, vecDir[currentPathDir].nInode, false);
			return;
		}
	}
	for (j = 0; j < vecDir[i].nFile; j++)
	{
		//如果是文件类型
		if (inodeTable[vecDir[i].nSubInodenGroup[j]][vecDir[i].nSubInodenInode[j]].type == typeFILE)
		{
			//递归删除文件
			del(i, inodeTable[vecDir[i].nSubInodenGroup[j]][vecDir[i].nSubInodenInode[j]].strName);
		}
		//如果是目录类型
		else if (inodeTable[vecDir[i].nSubInodenGroup[j]][vecDir[i].nSubInodenInode[j]].type == typeDIRECTORY)
		{
			//递归删除目录
			cmd_rd(inodeTable[vecDir[i].nSubInodenGroup[j]][vecDir[i].nSubInodenInode[j]].strName, i);
		}
		else;
	}
	//删除目录
	cout << "目录：\"" << inodeTable[tempNGroup][tempNInode].strName << "\"\n";
	strShellOutput = "目录：\"" + string(inodeTable[tempNGroup][tempNInode].strName) + "\"\n";
	shellOutput(strShellOutput);
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
	vecDir[currentPathDir].nSubInodenGroup[indexSub] = vecDir[currentPathDir].nSubInodenGroup[vecDir[currentPathDir].nFile - 1];
	vecDir[currentPathDir].nSubInodenInode[indexSub] = vecDir[currentPathDir].nSubInodenInode[vecDir[currentPathDir].nFile - 1];
	vecDir[thisPath].nFile -= 1;
	//清楚内存对应元素
	vecDir.erase(vecDir.begin() + i);


	cout << "目录已删除！\n";
	shellOutput("目录已删除！\n");

	writeInode(0, vecDir[currentPathDir].nInode, true);
	saveDisk();
}