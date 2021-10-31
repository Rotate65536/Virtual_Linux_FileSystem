#include"head.h"
void cmd_show_files()
{
	cout << "Ŀ¼������" << vecDir.size() << endl;
	for (int i = 0; i < vecDir.size(); i++)
	{
		cout << inodeTable[vecDir[i].nGroup][vecDir[i].nInode].strName << endl;
	}
	cout << "�ļ�������" << vecFile.size() << endl;
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
		cout << "·�������ڣ�\n";
		return;
	}
	strcpy(currentPath, inodeTable[vecDir[i].nGroup][vecDir[i].nInode].strName);
	currentPathDir = i;
}

void cmd_dir(const char* path)
{
	//Ѱ��·��
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
		cout << "·�������ڣ�\n";
		return;
	}
	//�ж�Ȩ��
	if (inodeTable[0][vecDir[i].nInode].id != presentUser.id
		&& presentUser.type == VISITOR && inodeTable[0][vecDir[i].nInode].id != 0)
	{
		cout << "�ǵ�ǰ�û�Ŀ¼��\n";
		return;
	}
	//��ʾ��Ϣ
	for (j = 0; j < vecDir[currentPathDir].nFile; j++)
	{
		inodeTable[vecDir[currentPathDir].nSubInodenGroup[j]][vecDir[currentPathDir].nSubInodenInode[j]];
		cout << "�ļ�����"
			<< inodeTable[vecDir[currentPathDir].nSubInodenGroup[j]][vecDir[currentPathDir].nSubInodenInode[j]].strName
			<< "\n����ʱ�䣺" << (char*)(ctime(&inodeTable[vecDir[currentPathDir].nSubInodenGroup[j]][vecDir[currentPathDir].nSubInodenInode[j]].createTime))
			<< "������id��" << inodeTable[vecDir[currentPathDir].nSubInodenGroup[j]][vecDir[currentPathDir].nSubInodenInode[j]].id
			<< "\n��ַ��" << inodeTable[vecDir[currentPathDir].nSubInodenGroup[j]][vecDir[currentPathDir].nSubInodenInode[j]].addrBlock;
		switch (inodeTable[vecDir[currentPathDir].nSubInodenGroup[j]][vecDir[currentPathDir].nSubInodenInode[j]].authority)
		{
		case PROTECTED:
		{
			cout << "\nȨ�ޣ�" << "ϵͳ";
			break;
		}
		case READ_WRITE:
		{
			cout << "\nȨ�ޣ�" << "��д";
			break;
		}
		case READ_ONLY:
		{
			cout << "\nȨ�ޣ�" << "ֻ��";
			break;
		}
		};
		switch (inodeTable[vecDir[currentPathDir].nSubInodenGroup[j]][vecDir[currentPathDir].nSubInodenInode[j]].type)
		{
		case typeFILE:
		{
			cout << "\n���ͣ�" << "�ļ�\n\n";
			break;
		}
		case typeDIRECTORY:
		{
			cout << "\n���ͣ�" << "Ŀ¼\n\n";
			break;
		}
		case typeBlock:
		{
			cout << "\n���ͣ�" << "�տ�\n\n";
			break;
		}
		};
	}
	return;
}

void cmd_md(const char* fileName, file_authority authority)
{
	//�ж�Ȩ��
	if (authority == PROTECTED && presentUser.type == VISITOR)
	{
		cout << "��Ҫ����ԱȨ�ޣ�\n";
		return;
	}
	if (inodeTable[0][vecDir[currentPathDir].nInode].id != presentUser.id
		&& presentUser.type == VISITOR && inodeTable[0][vecDir[currentPathDir].nInode].id != 0)
	{
		cout << "�ǵ�ǰ�û�Ŀ¼��\n";
		return;
	}
	cout << "�����½�Ŀ¼\"";
	//����i����
	char filePathName[MAX_NAME_LENGTH];
	strcpy(filePathName, inodeTable[vecDir[currentPathDir].nGroup][vecDir[currentPathDir].nInode].strName);
	strcat(filePathName, "/");
	strcat(filePathName, (char*)fileName);
	cout << filePathName << "\"\n";
	updateFileInfo(0, vecDir.size(), (char*)filePathName, typeDIRECTORY, authority,
		emptyDirBlockIndex, sizeof(dir) - 1016);
	//���µ�ǰĿ¼��Ϣ
	vecDir[currentPathDir].nSubInodenGroup[vecDir[currentPathDir].nFile] = 0;
	vecDir[currentPathDir].nSubInodenInode[vecDir[currentPathDir].nFile] = vecDir.size();
	vecDir[currentPathDir].nFile += 1;
	vecDir.push_back(dir(0, vecDir.size()));
	emptyDirBlockIndex += sizeof(dir);
	//���´���
	saveDisk();
	cout << "��ɣ�\n";
	return;
}

void cmd_rd(const char* path, int thisPath)
{
	int i, j, ifExist = 0;
	if (inodeTable[0][vecDir[currentPathDir].nInode].id != presentUser.id
		&& presentUser.type == VISITOR && inodeTable[0][vecDir[currentPathDir].nInode].id != 0)
	{
		cout << "�ǵ�ǰ�û�Ŀ¼��\n";
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
		cout << "·�������ڣ�\n";
		return;
	}
	short tempNGroup = vecDir[thisPath].nSubInodenGroup[i],
		tempNInode = vecDir[thisPath].nSubInodenInode[i];
	int thisP = i;
	//�ж�Ȩ��
	if ((inodeTable[tempNGroup][tempNInode].id != presentUser.id
		&& presentUser.type == VISITOR)
		||
		inodeTable[tempNGroup][tempNInode].authority == PROTECTED
		&& presentUser.type == VISITOR)
	{
		cout << "Ȩ�޲��㣡\n";
		return;
	}

	for (i = 0; i < vecDir.size(); i++)
	{
		if (vecDir[i].nGroup == tempNGroup && vecDir[i].nInode == tempNInode)
			break;
	}

	if (vecDir[i].nFile > 0)
	{
		cout << "���棡Ŀ¼��" << inodeTable[vecDir[i].nGroup][vecDir[i].nInode].strName << "�ǿգ��Ƿ��������Y/N��";
		string ifContinue;
		cin >> ifContinue;
		if (ifContinue == "Y" || ifContinue == "y");
		else
		{
			cout << "ȡ��������\n";
			return;
		}
	}
	for (j = 0; j < vecDir[i].nFile; j++)
	{
		//������ļ�����
		if (inodeTable[vecDir[i].nSubInodenGroup[j]][vecDir[i].nSubInodenInode[j]].type == typeFILE)
		{
			del(i, inodeTable[vecDir[i].nSubInodenGroup[j]][vecDir[i].nSubInodenInode[j]].strName);
		}
		//�����Ŀ¼����
		else if (inodeTable[vecDir[i].nSubInodenGroup[j]][vecDir[i].nSubInodenInode[j]].type == typeDIRECTORY)
		{
			cmd_rd(inodeTable[vecDir[i].nSubInodenGroup[j]][vecDir[i].nSubInodenInode[j]].strName, i);
		}
		else;
	}
	//ɾ��Ŀ¼
	cout << "Ŀ¼��\"" << inodeTable[tempNGroup][tempNInode].strName << "\"\n";
	//����i����λͼ��������
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
	//����Ŀ¼��Ϣ
	vecDir[thisPath].nFile -= 1;
	//����ڴ��ӦԪ��
	vecDir.erase(vecDir.begin() + i);
	cout << "Ŀ¼��ɾ����\n";
	saveDisk();
}