#include"head.h"
void cmd_newfile(const char* fileName, file_authority authority) 
{
	int i;
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
	cout << "�����½��ļ�\"" << fileName << "\"\n";
	for (i = 1; i < BLOCK_GROUPS_NUM; i++)
	{
		if (blockGroup[i].superBlock.nFreeBlocks > 0 && blockGroup[i].superBlock.nFreeInodes > 0)
			break;
	}
	cout << "�����ļ����ݣ�\n";
	string strContent;
	cin >> strContent;
	if (strContent.length() > 1019)
	{
		cout << "���������̿�......";
		//  δʵ��  //
	}
	else
	{
		cout << "����һ�����̿�......";
		char content[MAX_NAME_LENGTH];
		strcpy(content, strContent.c_str());
		//����i����λͼ��������
		updateFileInfo(i, vecFile.size() % BLOCK_NUM_EACH_GROUP, fileName, typeFILE,
			authority, emptyFileBlockIndex, sizeof(file) - 1020 + sizeof(content));
		//���µ�ǰĿ¼��Ϣ
		vecDir[currentPathDir].nSubInodenGroup[vecDir[currentPathDir].nFile] = i;
		vecDir[currentPathDir].nSubInodenInode[vecDir[currentPathDir].nFile] = vecFile.size() % BLOCK_NUM_EACH_GROUP;
		vecDir[currentPathDir].nFile += 1;
		//���¿����̿�����
		emptyFileBlockIndex += sizeof(file);
		//�����ڴ��ļ���Ϣ
		vecFile.push_back(file(i, vecFile.size(), content));
		//���´���
		saveDisk();
		cout << "��ɣ�\n";
		return;
	}
}
void cmd_cat(const char* fileName) 
{
	//�ж�Ȩ��
	if (inodeTable[0][vecDir[currentPathDir].nInode].id != presentUser.id
		&& presentUser.type == VISITOR && inodeTable[0][vecDir[currentPathDir].nInode].id != 0)
	{
		cout << "�ǵ�ǰ�û�Ŀ¼��\n";
		return;
	}
	//Ѱ���ļ�
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
		cout << "�ļ������ڣ�\n";
		return;
	}
	short tempNGroup = vecDir[currentPathDir].nSubInodenGroup[i],
		tempNInode = vecDir[currentPathDir].nSubInodenInode[i];
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
	//���ļ�
	cout << "���ڴ��ļ�......\n";
	//һ���ļ�ռ�ö�����̿�
	if (inodeTable[tempNGroup][tempNInode].nBlock > 1)
	{
		// δ��� //
	}
	//һ���ļ�ռ��һ�����̿�
	else
	{
		for (i = 0; i < vecFile.size(); i++)
		{
			if (vecFile[i].nGroup == tempNGroup && vecFile[i].nInode == tempNInode)
				break;
		}
		cout << "�ļ����ݣ�\n";
		cout << vecFile[i].content << endl;
	}
}

void cmd_copy(const char* srcPath, const char* desPath)
{
	if (inodeTable[0][vecDir[currentPathDir].nInode].id != presentUser.id
		&& presentUser.type == VISITOR && inodeTable[0][vecDir[currentPathDir].nInode].id != 0)
	{
		cout << "�ǵ�ǰ�û�Ŀ¼��\n";
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
		cout << "��������ϵͳ�����ļ��������ļ�ϵͳ......\n";
		string strSrcPath = srcPath;
		string hostPath = string(strSrcPath, 5, strSrcPath.length() - 1);
		ifstream input(hostPath, ios::in);
		char* hostContent;
		//����ֻ��ռ��һ�����̿�
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
			// ռ�ö�����̿� //
		}
		else
		{
			cmd_cd(desPath);
			if (inodeTable[0][vecDir[currentPathDir].nInode].id != presentUser.id
				&& presentUser.type == VISITOR && inodeTable[0][vecDir[currentPathDir].nInode].id != 0)
			{
				cout << "�ǵ�ǰ�û�Ŀ¼��\n";
				return;
			}
			cout << "���ڸ����ļ�\"" << srcPath << "\"\n";
			for (i = 1; i < BLOCK_GROUPS_NUM; i++)
			{
				if (blockGroup[i].superBlock.nFreeBlocks > 0 && blockGroup[i].superBlock.nFreeInodes > 0)
					break;
			}
			cout << "����һ�����̿�......";
			char content[1020];
			strcpy(content, hostContent);
			//����i����λͼ��������
			updateFileInfo(i, vecFile.size() % BLOCK_NUM_EACH_GROUP, hostPath.c_str(), typeFILE,
				READ_WRITE, emptyFileBlockIndex, sizeof(file) - 1020 + sizeof(content));
			//���µ�ǰĿ¼��Ϣ
			vecDir[currentPathDir].nSubInodenGroup[vecDir[currentPathDir].nFile] = i;
			vecDir[currentPathDir].nSubInodenInode[vecDir[currentPathDir].nFile] = vecFile.size() % BLOCK_NUM_EACH_GROUP;
			vecDir[currentPathDir].nFile += 1;
			//���¿����̿�����
			emptyFileBlockIndex += sizeof(file);
			//�����ڴ��ļ���Ϣ
			vecFile.push_back(file(i, vecFile.size(), content));
			//���´���
			saveDisk();
			cout << "��ɣ�\n";
			return;
		}
	}
	else if (strcmp(ifDesHost, "<host>") == 0)
	{
		cout << "���������ļ�ϵͳ�����ļ�������ϵͳ......\n";
		// δҪ����� //
	}
	else
	{
		//��ȥ���ļ��в��ܲ���
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
			cout << "�ļ������ڣ�\n";
			return;
		}
		short tempNGroup = vecDir[currentPathDir].nSubInodenGroup[i],
			tempNInode = vecDir[currentPathDir].nSubInodenInode[i];
		//���ռ�ô��̿����1
		if (inodeTable[tempNGroup][tempNInode].nBlock > 1)
		{
			// δ��� //
		}
		else
		{
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
				cout << "�ǵ�ǰ�û�Ŀ¼��\n";
				return;
			}
			cout << "���ڸ����ļ�\"" << srcPath << "\"\n";
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
			cout << "����һ�����̿�......";
			char content[1020];
			strcpy(content, vecFile[j].content);
			//����i����λͼ��������
			updateFileInfo(i, vecFile.size() % BLOCK_NUM_EACH_GROUP, inodeTable[tempNGroup][tempNInode].strName, typeFILE,
				inodeTable[tempNGroup][tempNInode].authority, emptyFileBlockIndex, sizeof(file) - 1020 + sizeof(content));
			//���µ�ǰĿ¼��Ϣ
			vecDir[currentPathDir].nSubInodenGroup[vecDir[currentPathDir].nFile] = i;
			vecDir[currentPathDir].nSubInodenInode[vecDir[currentPathDir].nFile] = vecFile.size() % BLOCK_NUM_EACH_GROUP;
			vecDir[currentPathDir].nFile += 1;
			//���¿����̿�����
			emptyFileBlockIndex += sizeof(file);
			//�����ڴ��ļ���Ϣ
			vecFile.push_back(file(i, vecFile.size(), content));
			//���´���
			saveDisk();
			cout << "��ɣ�\n";
			return;
		}
	}
}


void cmd_del(const char* fileName) 
{
	//�ж�Ȩ��
	if (inodeTable[0][vecDir[currentPathDir].nInode].id != presentUser.id
		&& presentUser.type == VISITOR && inodeTable[0][vecDir[currentPathDir].nInode].id != 0)
	{
		cout << "�ǵ�ǰ�û�Ŀ¼��\n";
		return;
	}
	//Ѱ���ļ�
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
		cout << "�ļ������ڣ�\n";
		return;
	}
	short tempNGroup = vecDir[currentPathDir].nSubInodenGroup[i],
		tempNInode = vecDir[currentPathDir].nSubInodenInode[i];
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
	//v.erase(v.begin() + 2);
	//һ���ļ�ռ�ö�����̿�
	if (inodeTable[tempNGroup][tempNInode].nBlock > 1)
	{
		// δ��� //
	}
	//һ���ļ�ռ��һ�����̿�
	else
	{
		for (i = 0; i < vecFile.size(); i++)
		{
			if (vecFile[i].nGroup == tempNGroup && vecFile[i].nInode == tempNInode)
				break;
		}
		cout << "����ɾ���ļ���\"" << inodeTable[tempNGroup][tempNInode].strName << "\"\n";
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
		vecDir[currentPathDir].nFile -= 1;
		//����ڴ��ӦԪ��
		vecFile.erase(vecFile.begin() + i);
		cout << "�ļ���ɾ����\n";
		saveDisk();
	}
	return;
}

void del(int thisPath, const char* fileName)
{
	currentPathDir;
	//�ж�Ȩ��
	if (inodeTable[0][vecDir[thisPath].nInode].id != presentUser.id
		&& presentUser.type == VISITOR && inodeTable[0][vecDir[thisPath].nInode].id != 0)
	{
		cout << "�ǵ�ǰ�û�Ŀ¼��\n";
		return;
	}
	//Ѱ���ļ�
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
		cout << "�ļ������ڣ�\n";
		return;
	}
	short tempNGroup = vecDir[thisPath].nSubInodenGroup[i],
		tempNInode = vecDir[thisPath].nSubInodenInode[i];
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
	//v.erase(v.begin() + 2);
	//һ���ļ�ռ�ö�����̿�
	if (inodeTable[tempNGroup][tempNInode].nBlock > 1)
	{
		// δ��� //
	}
	//һ���ļ�ռ��һ�����̿�
	else
	{
		for (i = 0; i < vecFile.size(); i++)
		{
			if (vecFile[i].nGroup == tempNGroup && vecFile[i].nInode == tempNInode)
				break;
		}
		cout << "����ɾ���ļ���\"" << inodeTable[tempNGroup][tempNInode].strName << "\"\n";
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
		vecFile.erase(vecFile.begin() + i);
		cout << "�ļ���ɾ����\n";
		saveDisk();
	}
	return;
}