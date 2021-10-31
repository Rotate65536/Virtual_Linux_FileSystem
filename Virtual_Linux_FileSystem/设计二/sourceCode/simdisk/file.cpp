#include"head.h"
void cmd_newfile(const char* fileName, file_authority authority) 
{
	string strShellOutput;
	int i;
	//�ж��û�Ȩ��
	if (authority == PROTECTED && presentUser.type == VISITOR)
	{
		cout << "��Ҫ����ԱȨ�ޣ�\n";
		shellOutput("��Ҫ����ԱȨ�ޣ�\n");
		return;
	}
	if (inodeTable[0][vecDir[currentPathDir].nInode].id != presentUser.id
		&& presentUser.type == VISITOR
		&& inodeTable[0][vecDir[currentPathDir].nInode].id != 0)
	{
		cout << "�ǵ�ǰ�û�Ŀ¼��\n";
		shellOutput("�ǵ�ǰ�û�Ŀ¼��\n");
		return;
	}

	
	cout << "�����½��ļ�\"" << fileName << "\"\n";
	strShellOutput = "�����½��ļ�\"" + string(fileName) + "\"\n";
	shellOutput(strShellOutput);
	for (i = 1; i < BLOCK_GROUPS_NUM; i++)  //ת�����еĴ��̿���
	{
		if (blockGroup[i].superBlock.nFreeBlocks > 0 && blockGroup[i].superBlock.nFreeInodes > 0)
			break;
	}
	cout << "�����ļ����ݣ�\n";
	shellOutput("�����ļ����ݣ�\n");
	string strContent;
	char charstrContent[MAX_COMMAND_LENGTH];
	shellInput(charstrContent);
	strContent = charstrContent;
	if (strContent.length() > 1019)
	{
		cout << "���������̿�......";
		shellOutput("���������̿�......");
		//  δʵ��  //
	}
	else
	{
		cout << "����һ�����̿�......";
		shellOutput("����һ�����̿�......");
		char content[MAX_NAME_LENGTH];
		strcpy(content, strContent.c_str());
		//����i����λͼ��������
		cout << "presentUser.id" << presentUser.id << endl;
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
		shellOutput("��ɣ�\n");
		
	}
	return;
}
void cmd_cat(const char* fileName) 
{
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
		shellOutput("�ļ������ڣ�\n");
		return;
	}

	//��¼i�ڵ��
	short tempNGroup = vecDir[currentPathDir].nSubInodenGroup[i],
		tempNInode = vecDir[currentPathDir].nSubInodenInode[i];

	//���ļ�
	cout << "���ڴ��ļ�......\n";
	shellOutput("���ڴ��ļ�......\n");
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
		shellOutput("�ļ����ݣ�\n");
		cout << vecFile[i].content << endl;
		string strShellOutput = string(vecFile[i].content) + "\n";
		shellOutput(strShellOutput);
		cout << "�Ƿ���и��ģ���Y/N��\n";
		shellOutput("�Ƿ���и��ģ���Y/N��\n");
		string ifChange;
		char charifChange[MAX_COMMAND_LENGTH];
		shellInput(charifChange);
		ifChange = charifChange;
		if (ifChange == "Y" || ifChange == "y")
		{
			if (presentUser.id != inodeTable[tempNGroup][tempNInode].id
				&& presentUser.type == VISITOR)
			{
				cout << "Ȩ�޲��㣡\n";
				shellOutput("Ȩ�޲��㣡\n");
				return;
			}
			cout << "���������ݣ�\n";
			shellOutput("���������ݣ�\n");
			char charContents[MAX_COMMAND_LENGTH];
			shellInput(charContents);
			strcpy(vecFile[i].content, charContents);
			saveDisk();
		}
		else;
	}
}

void cmd_copy(const char* srcPath, const char* desPath)
{
	//�ж�Ȩ��
	if (inodeTable[0][vecDir[currentPathDir].nInode].id != presentUser.id
		&& presentUser.type == VISITOR)
	{
		cout << "�ǵ�ǰ�û�Ŀ¼��\n";
		shellOutput("�ǵ�ǰ�û�Ŀ¼��\n");
		return;
	}
	char ifSrcHost[7], ifDesHost[7];
	cout << "srcPath: " << srcPath << endl;
	strncpy(ifSrcHost, srcPath, 6);
	ifSrcHost[6] = '\0';
	ifDesHost[6] = '\0';
	cout << "ifSrcHost: " << ifSrcHost << endl;
	cout << "ifDesHost: " << ifDesHost << endl;
	strncpy(ifDesHost, desPath, 6);
	string strShellOutput;
	//�ж��Ƿ���������й�
	if (strcmp(ifSrcHost, "<host>") == 0)
	{
		int i, j;
		cout << "��������ϵͳ�����ļ��������ļ�ϵͳ......\n";
		shellOutput("��������ϵͳ�����ļ��������ļ�ϵͳ......\n");
		string strSrcPath = srcPath;
		string hostPath = string(strSrcPath, 6, strSrcPath.length() - 1);
		cout << "hostPath: " << hostPath << endl;
		ifstream txtFile(hostPath, ios::in||ios::out);
		if (!txtFile.is_open())
		{
			cout << "�޷����ļ���\n";
			shellOutput("�޷����ļ���\n");
			return;
		}
		char* hostContent;
		//����ֻ��ռ��һ�����̿�
		hostContent = (char*)malloc(sizeof(char) * 1019);
		int ifMoreBlock = 0, countChar = 0;
		char buff[1019];
		int pt = 0;
		while (txtFile.getline(buff, 1019))
		{
			for (i = 0; i < 1019; i++) {
				if (buff[i] == '\0' && countChar >= 1019)
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
		cout << "\nhostContent: " << hostContent << endl;
		if (ifMoreBlock)
		{
			// ռ�ö�����̿� //
		}
		else
		{
			cout << "here\n";
			cmd_cd(desPath);
			if (inodeTable[0][vecDir[currentPathDir].nInode].id != presentUser.id
				&& presentUser.type == VISITOR && inodeTable[0][vecDir[currentPathDir].nInode].id != 0)
			{
				cout << "�ǵ�ǰ�û�Ŀ¼��\n";
				shellOutput("�ǵ�ǰ�û�Ŀ¼��\n");
				return;
			}
			cout << "���ڸ����ļ�\"" << srcPath << "\"\n";
			strShellOutput = "���ڸ����ļ�\"" + string(srcPath) + "\"\n";
			shellOutput(strShellOutput);
			for (i = 1; i < BLOCK_GROUPS_NUM; i++)
			{
				if (blockGroup[i].superBlock.nFreeBlocks > 0 && blockGroup[i].superBlock.nFreeInodes > 0)
					break;
			}
			cout << "����һ�����̿�......";
			shellOutput("����һ�����̿�......");
			cout << "\nhostContent: " << hostContent << endl;
			cout << "\nhostPath.c_str(): " << hostPath.c_str() << endl;
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
			shellOutput("��ɣ�\n");
			return;
		}
	}
	else if (strcmp(ifDesHost, "<host>") == 0)
	{
		cout << "���������ļ�ϵͳ�����ļ�������ϵͳ......\n";
		shellOutput("���������ļ�ϵͳ�����ļ�������ϵͳ......\n");
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
			shellOutput("�ļ������ڣ�\n");
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
				shellOutput("Ȩ�޲��㣡\n");
				return;
			}
			cmd_cd(desPath);

			if (inodeTable[0][vecDir[currentPathDir].nInode].id != presentUser.id
				&& presentUser.type == VISITOR && inodeTable[0][vecDir[currentPathDir].nInode].id != 0)
			{
				cout << "�ǵ�ǰ�û�Ŀ¼��\n";
				shellOutput("�ǵ�ǰ�û�Ŀ¼��\n");
				return;
			}
			cout << "���ڸ����ļ�\"" << srcPath << "\"\n";
			strShellOutput = "���ڸ����ļ�\"" + string(srcPath) + "\"\n";
			shellOutput(strShellOutput);
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
			shellOutput("����һ�����̿�......");
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
			shellOutput("��ɣ�\n");
			return;
		}
	}
}


void cmd_del(const char* fileName) 
{
	string strShellOutput;
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
		shellOutput("�ļ������ڣ�\n");
		return;
	}

	short tempNGroup = vecDir[currentPathDir].nSubInodenGroup[i],
		tempNInode = vecDir[currentPathDir].nSubInodenInode[i];
	short indexSub = i;
	//�ж�Ȩ��
	if ((inodeTable[tempNGroup][tempNInode].id != presentUser.id
		&& presentUser.type == VISITOR)
		||
		inodeTable[tempNGroup][tempNInode].authority == PROTECTED
		&& presentUser.type == VISITOR)
	{
		cout << "Ȩ�޲��㣡\n";
		shellOutput("Ȩ�޲��㣡\n");
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
		strShellOutput = "����ɾ���ļ���\"" + string(inodeTable[tempNGroup][tempNInode].strName) + "\"\n";
		shellOutput(strShellOutput);
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
		vecDir[currentPathDir].nSubInodenGroup[indexSub] = vecDir[currentPathDir].nSubInodenGroup[vecDir[currentPathDir].nFile - 1];
		vecDir[currentPathDir].nSubInodenInode[indexSub] = vecDir[currentPathDir].nSubInodenInode[vecDir[currentPathDir].nFile - 1];
		vecDir[currentPathDir].nFile -= 1;
		//����ڴ��ӦԪ��
		vecFile.erase(vecFile.begin() + i);
		cout << "�ļ���ɾ����\n";
		shellOutput("�ļ���ɾ����\n");
		saveDisk();
	}


	return;
}

void del(int thisPath, const char* fileName)
{
	string strShellOutput;
	currentPathDir;
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
		shellOutput("�ļ������ڣ�\n");
		return;
	}
	short tempNGroup = vecDir[thisPath].nSubInodenGroup[i],
		tempNInode = vecDir[thisPath].nSubInodenInode[i];
	short indexSub = i;
	//�ж�Ȩ��
	if ((inodeTable[tempNGroup][tempNInode].id != presentUser.id
		&& presentUser.type == VISITOR)
		||
		inodeTable[tempNGroup][tempNInode].authority == PROTECTED
		&& presentUser.type == VISITOR)
	{
		cout << "Ȩ�޲��㣡\n";
		shellOutput("Ȩ�޲��㣡\n");
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
		strShellOutput = "����ɾ���ļ���\"" + string(inodeTable[tempNGroup][tempNInode].strName) + "\"\n";
		shellOutput(strShellOutput);
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
		vecDir[currentPathDir].nSubInodenGroup[indexSub] = vecDir[currentPathDir].nSubInodenGroup[vecDir[currentPathDir].nFile - 1];
		vecDir[currentPathDir].nSubInodenInode[indexSub] = vecDir[currentPathDir].nSubInodenInode[vecDir[currentPathDir].nFile - 1];
		//����ڴ��ӦԪ��
		vecFile.erase(vecFile.begin() + i);
		cout << "�ļ���ɾ����\n";
		shellOutput("�ļ���ɾ����\n");
		saveDisk();
			return;
	}
	return;
}