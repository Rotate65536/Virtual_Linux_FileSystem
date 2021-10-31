#include"head.h"
void cmd_show_files()
{
	//��ʾ����Ŀ¼�ļ�����ͨ�ļ�������������debug
	cout << "Ŀ¼������" << vecDir.size() << endl;
	int strSize = vecDir.size();
	string strShellOutput = "Ŀ¼������" + to_string(strSize) + "\n";
	shellOutput(strShellOutput);
	for (int i = 0; i < vecDir.size(); i++)
	{
		cout << inodeTable[vecDir[i].nGroup][vecDir[i].nInode].strName << endl;
	}
	cout << "�ļ�������" << vecFile.size() << endl;
	strSize = vecFile.size();
	strShellOutput = "�ļ�������" + to_string(strSize) + "\n";
	shellOutput(strShellOutput);
	for (int i = 0; i < vecFile.size(); i++)
	{
		cout << inodeTable[vecFile[i].nGroup][vecFile[i].nInode].strName << endl;
	}
}

void cmd_cd(const char* path)
{
	int i, ifExist = 0;
	for (i = 0; i < vecDir.size(); i++)  //Ѱ��Ŀ��Ŀ¼
	{
		if (strcmp(inodeTable[vecDir[i].nGroup][vecDir[i].nInode].strName, path) == 0)
		{
			ifExist = 1;
			break;
		}
	}
	if (ifExist);  //����
	else  //������
	{
		cout << "·�������ڣ�\n";
		shellOutput("·�������ڣ�\n");
		return;
	}
	//�ж��û�Ȩ��
	if (presentUser.type == VISITOR && inodeTable[0][vecDir[i].nInode].authority == PROTECTED)
	{
		cout << "�ο��޷�����ϵͳĿ¼��\n";
		shellOutput("�ο��޷�����ϵͳĿ¼��\n");
		return;
	}
	//���µ�ǰĿ¼ 
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
	if (ifExist);  //����
	else  //������
	{
		cout << "·�������ڣ�\n";
		shellOutput("·�������ڣ�\n");
		return;
	}
	//�ж��û�Ȩ��
	if (presentUser.type == VISITOR && inodeTable[0][vecDir[i].nInode].authority == PROTECTED)
	{
		cout << "�ο��޷�����ϵͳĿ¼��\n";
		shellOutput("�ο��޷�����ϵͳĿ¼��\n");
		return;
	}
	//��ʾ��Ϣ
	if (readInode(0, vecDir[currentPathDir].nInode, true) == false)  //�鿴�Ƿ�ɶ�
		return;  //�����ɶ����˳�
	for (j = 0; j < vecDir[currentPathDir].nFile; j++)
	{
		inodeTable[vecDir[currentPathDir].nSubInodenGroup[j]][vecDir[currentPathDir].nSubInodenInode[j]];
		cout << "�ļ�����"
			<< inodeTable[vecDir[currentPathDir].nSubInodenGroup[j]][vecDir[currentPathDir].nSubInodenInode[j]].strName
			<< "\n����ʱ�䣺" << (char*)(ctime(&inodeTable[vecDir[currentPathDir].nSubInodenGroup[j]][vecDir[currentPathDir].nSubInodenInode[j]].createTime))
			<< "������id��" << inodeTable[vecDir[currentPathDir].nSubInodenGroup[j]][vecDir[currentPathDir].nSubInodenInode[j]].id
			<< "\n��ַ��" << inodeTable[vecDir[currentPathDir].nSubInodenGroup[j]][vecDir[currentPathDir].nSubInodenInode[j]].addrBlock;
		string strShellOutput = "�ļ�����"
			+ string(inodeTable[vecDir[currentPathDir].nSubInodenGroup[j]][vecDir[currentPathDir].nSubInodenInode[j]].strName)
			+ "\n����ʱ�䣺" + string((char*)(ctime(&inodeTable[vecDir[currentPathDir].nSubInodenGroup[j]][vecDir[currentPathDir].nSubInodenInode[j]].createTime)))
			+ "������id��" + to_string(inodeTable[vecDir[currentPathDir].nSubInodenGroup[j]][vecDir[currentPathDir].nSubInodenInode[j]].id)
			+ "\n��ַ��" + to_string(inodeTable[vecDir[currentPathDir].nSubInodenGroup[j]][vecDir[currentPathDir].nSubInodenInode[j]].addrBlock);
		shellOutput(strShellOutput);
		switch (inodeTable[vecDir[currentPathDir].nSubInodenGroup[j]][vecDir[currentPathDir].nSubInodenInode[j]].authority)
		{
		case PROTECTED:
		{
			cout << "\nȨ�ޣ�ϵͳ";
			shellOutput("\nȨ�ޣ�ϵͳ");
			break;
		}
		case READ_WRITE:
		{
			cout << "\nȨ�ޣ�" << "��д";
			shellOutput("\nȨ�ޣ���д");
			break;
		}
		case READ_ONLY:
		{
			cout << "\nȨ�ޣ�" << "ֻ��";
			shellOutput("\nȨ�ޣ�ֻ��");
			break;
		}
		};
		switch (inodeTable[vecDir[currentPathDir].nSubInodenGroup[j]][vecDir[currentPathDir].nSubInodenInode[j]].type)
		{
		case typeFILE:
		{
			cout << "\n���ͣ�" << "�ļ�\n\n";
			shellOutput("\n���ͣ��ļ�\n\n");
			break;
		}
		case typeDIRECTORY:
		{
			cout << "\n���ͣ�" << "Ŀ¼\n\n";
			shellOutput("\n���ͣ�Ŀ¼\n\n");
			break;
		}
		case typeBlock:
		{
			cout << "\n���ͣ�" << "�տ�\n\n";
			shellOutput("\n���ͣ��տ�\n\n");
			break;
		}
		};
	}
	string choiceIfExit = "N";
	//cin >> choiceIfInit;
	char charChoiceIfExit[MAX_COMMAND_LENGTH];
	while (choiceIfExit == "N" || choiceIfExit == "n")
	{
		cout << "�Ƿ��˳��鿴����Y/N��";
		shellOutput("�Ƿ��˳��鿴����Y/N��");
		shellInput(charChoiceIfExit);
		choiceIfExit = charChoiceIfExit;
	}
	readInode(0, vecDir[currentPathDir].nInode, false);  //�˳��������»�����
	return;
}

void cmd_md(const char* fileName, file_authority authority)
{
	//�ж�Ȩ��
	if (authority == PROTECTED && presentUser.type == VISITOR)
	{
		cout << "����ϵͳĿ¼��Ҫ����ԱȨ�ޣ�\n";
		shellOutput("����ϵͳĿ¼��Ҫ����ԱȨ�ޣ�\n");
		return;
	}
	if (presentUser.type == VISITOR 
		&& inodeTable[0][vecDir[currentPathDir].nInode].id != presentUser.id 
		&& inodeTable[0][vecDir[currentPathDir].nInode].id != 0)
	{
		cout << "�ǵ�ǰ�û�Ŀ¼��\n";
		shellOutput("�ǵ�ǰ�û�Ŀ¼��\n");
		return;
	}
	if (presentUser.type == VISITOR
		&& inodeTable[0][vecDir[currentPathDir].nInode].authority == PROTECTED)
	{
		cout << "��ǰĿ¼ΪϵͳĿ¼��\n";
		shellOutput("��ǰĿ¼ΪϵͳĿ¼��\n");
		return;
	}
	if (writeInode(0, vecDir[currentPathDir].nInode, true) == false)  //�ж��Ƿ��д
		return;
	cout << "�����½�Ŀ¼\"";
	shellOutput("�����½�Ŀ¼\"");
	//����i����
	char filePathName[MAX_NAME_LENGTH];
	strcpy(filePathName, inodeTable[vecDir[currentPathDir].nGroup][vecDir[currentPathDir].nInode].strName);
	strcat(filePathName, "/");
	strcat(filePathName, (char*)fileName);
	cout << filePathName << "\"\n";
	string strShellOutput = string(filePathName) + "\"\n";
	shellOutput(strShellOutput);
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
	shellOutput("��ɣ�\n");
	writeInode(0, vecDir[currentPathDir].nInode, false);
	return;
}

void cmd_rd(const char* path, int thisPath)
{
	int i, j, ifExist = 0;
	string strShellOutput;
	for (i = 0; i < vecDir[thisPath].nFile; i++)  //Ѱ��Ŀ¼
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
		shellOutput("·�������ڣ�\n");
		return;
	}

	//�洢i�ڵ��
	short tempNGroup = vecDir[thisPath].nSubInodenGroup[i],
		tempNInode = vecDir[thisPath].nSubInodenInode[i];
	short indexSub = i;
	int thisP = i;
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

	for (i = 0; i < vecDir.size(); i++)
	{
		if (vecDir[i].nGroup == tempNGroup && vecDir[i].nInode == tempNInode)
			break;
	}

	if (writeInode(0, vecDir[currentPathDir].nInode, true) == false)
		return;
	if (vecDir[i].nFile > 0)
	{
		cout << "���棡Ŀ¼��" << inodeTable[vecDir[i].nGroup][vecDir[i].nInode].strName << "�ǿգ��Ƿ��������Y/N��";
		strShellOutput = "���棡Ŀ¼��" + string(inodeTable[vecDir[i].nGroup][vecDir[i].nInode].strName) + "�ǿգ��Ƿ��������Y/N��";
		shellOutput(strShellOutput);
		string ifContinue;
		//cin >> ifContinue;
		char charifContinue[MAX_COMMAND_LENGTH];
		shellInput(charifContinue);
		ifContinue = charifContinue;
		if (ifContinue == "Y" || ifContinue == "y");
		else
		{
			cout << "ȡ��������\n";
			shellOutput("ȡ��������\n");
			//writeInode(0, vecDir[currentPathDir].nInode, false);
			return;
		}
	}
	for (j = 0; j < vecDir[i].nFile; j++)
	{
		//������ļ�����
		if (inodeTable[vecDir[i].nSubInodenGroup[j]][vecDir[i].nSubInodenInode[j]].type == typeFILE)
		{
			//�ݹ�ɾ���ļ�
			del(i, inodeTable[vecDir[i].nSubInodenGroup[j]][vecDir[i].nSubInodenInode[j]].strName);
		}
		//�����Ŀ¼����
		else if (inodeTable[vecDir[i].nSubInodenGroup[j]][vecDir[i].nSubInodenInode[j]].type == typeDIRECTORY)
		{
			//�ݹ�ɾ��Ŀ¼
			cmd_rd(inodeTable[vecDir[i].nSubInodenGroup[j]][vecDir[i].nSubInodenInode[j]].strName, i);
		}
		else;
	}
	//ɾ��Ŀ¼
	cout << "Ŀ¼��\"" << inodeTable[tempNGroup][tempNInode].strName << "\"\n";
	strShellOutput = "Ŀ¼��\"" + string(inodeTable[tempNGroup][tempNInode].strName) + "\"\n";
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
	vecDir[thisPath].nFile -= 1;
	//����ڴ��ӦԪ��
	vecDir.erase(vecDir.begin() + i);


	cout << "Ŀ¼��ɾ����\n";
	shellOutput("Ŀ¼��ɾ����\n");

	writeInode(0, vecDir[currentPathDir].nInode, true);
	saveDisk();
}