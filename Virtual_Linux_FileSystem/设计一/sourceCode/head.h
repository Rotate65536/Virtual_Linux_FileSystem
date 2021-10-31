#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>
#include <conio.h>
using namespace std;

# pragma once

#define BLOCK_SIZE 1024  //�̿��С1KB
#define BLOCK_NUM_EACH_GROUP 1024  //ÿ�����ݿ�����̿���
#define BLOCK_GROUPS_NUM 100  //���ݿ�����
#define INODES_NUM_EACH_GROUP 1024  //ÿ�����ݿ����i�����
#define TOTAL_BLOCKS_NUM (BLOCK_NUM_EACH_GROUP * BLOCK_GROUPS_NUM)  //�ܵ��̿���
#define TOTAL_INODES_NUM (INODES_NUM_EACH_GROUP * BLOCK_GROUPS_NUM)  //�ܵ�i-�����

#define DATA_AREA_ADDR  (sizeof(block_group) * BLOCK_GROUPS_NUM + sizeof(bitmap) * (TOTAL_BLOCKS_NUM + TOTAL_INODES_NUM) + sizeof(i_node) * TOTAL_INODES_NUM)  //�ļ���������������еĵ�ַ

#define MAX_NAME_LENGTH  128  //��ļ���
#define MAX_FILES_OF_DIR  256  //һ��Ŀ¼�������ļ�����
#define MAX_PASSWORD_LENGTH 16  //������󳤶�
#define MAX_PATH_LENGTH 256  //·����󳤶�
#define MAX_COMMAND_LENGTH 128  //������󳤶�
#define COMMAND_COUNT 16  //��������

//λͼԪ��
enum bitmap
{
	EMPTY = 0,
	OCCUPIED = 1
};

//�ļ�����
enum file_type
{
	typeFILE,  //�ļ�����
	typeDIRECTORY,  //Ŀ¼����
	typeBlock  //�տ�����
};

//��дȨ��
enum file_authority
{
	PROTECTED,	//ϵͳ
	READ_WRITE,	//��д
	READ_ONLY,	//ֻ��
};

//�û�����
enum user_type
{
	VISITOR,  //�ο�
	ADMINISTRATOR,  //����Ա
	SYSTEM  //ϵͳ
};


//��������
struct super_block {
	unsigned int nFreeBlocks;  //���п�����
	unsigned int nFreeInodes;  //����i�������
};

//���ݿ�����Ϣ��
struct group_info {
	unsigned int indexBlockBmp;  //���ݿ�λͼ����
	unsigned int indexInodeBmp;  //i-���λͼ����
	unsigned int indexInodeTable;  //i-��������
	unsigned long blockAddr;  //������������ַ
	unsigned int nFreeBlocksNum;  //���п����Ŀ
	unsigned int nFreeInodesNum;  //����i-������Ŀ
};

//���ݿ�����
struct block_group
{
	//group_info groupInfo;  //���ݿ�����Ϣ
	super_block superBlock;  //������
};

//i�ڵ�
struct i_node
{
	char strName[MAX_NAME_LENGTH];  //Ŀ¼�����ļ���
	time_t createTime;  //����ʱ��
	int id;  //�������˺�

	//ʱ������
	/*
		time_t t;
		t = time(&t);
		char* pt = ctime(&t);
		cout << pt;
	*/

	file_authority authority;  //��дȨ��
	file_type type;  //�ļ����ͣ�Ŀ¼/�������ƣ��ļ�
	long addrBlock;  //���̿���ʼ��ַ
	long size;  //��С
	int nBlock;//Ĭ��ֻռ��һ�����̿�
};

//�տ�
struct emptyBlock
{
	char content[1024];
};

//Ŀ¼�� 1024B
struct dir  
{
	dir(){}
	dir(short ng, short ni)
	{
		nGroup = ng;
		nInode = ni;
		nFile = 0;
	}
	short nGroup;  //��� 2B
	short nInode;  //i���� 2B
	int nFile;  //��Ŀ¼�£��ļ���+Ŀ¼�� 4B
	short nSubInodenGroup[MAX_FILES_OF_DIR - 2];  //�洢��Ŀ¼��i����i���� 1016B
	short nSubInodenInode[MAX_FILES_OF_DIR - 2];
};

//�ļ��� 1024B
struct file  
{
	file() {}
	file(short ng, short ni, char* c)
	{
		nGroup = ng;
		nInode = ni;
		strcpy(content, c);
	}
	short nGroup;  //��� 2B
	short nInode;  //i���� 2B
	char content[1020];  //�ļ����� 1020B
};

//�û�
struct user
{
	int id;  //�˺�
	char name[32];
	user_type type;  //����
	char password[32];  //����
	user() {
		strcpy(password, "123456");  //Ĭ������Ϊ123456
		id = -1;
		type = VISITOR;  //Ĭ���û�����Ϊ�ο�
		strcpy(name, "Anonymous");
	}
	user(char* s, int i, user_type t, char* pwd) {
		strcpy(password, pwd);
		id = i;
		type = t;
		strcpy(name, s);
	}
};

//���´�����Ϣ
void updateFileInfo(int indexGroup, int index, const char* fileName, 
	file_type type, file_authority authority, int blockAddr, int size);  
//���ش���
void loadDisk();  
//��ʼ��ģ���ļ�ϵͳ
void initFileSystem(); 
//�������
void saveDisk();  

//ȫ������
void execute_cmd(const char* strCommand, const char* strPath1, const char* strPath2);
void cmd_ver();  //��ʾ�汾������
bool cmd_login();  //�л���ǰ�û�
void cmd_info();  //��ʾ��ǰģ���ļ�ϵͳ����Ϣ
void cmd_cd(const char* path);  //����Ŀ¼
void cmd_dir(const char* path);   //��ʾĿ¼���ļ���Ϣ
void cmd_md(const char* path, file_authority authority);  //����Ŀ¼
void cmd_rd(const char* path, int thisPath);  //ɾ��Ŀ¼
void cmd_newfile(const char* path, file_authority authority);
void cmd_cat(const char* path);  //�򿪲���ʾ�ļ�����
void cmd_copy(const char* srcPath, const char* desPath);  //�����ļ�
void cmd_del(const char* path);  //ɾ���ļ�
void cmd_check();  //���ģ���ļ�ϵͳһ���Բ��ָ�
void cmd_exit();  //�˳�ģ���ļ�ϵͳ
void cmd_show_files();  ///��ʾ�����ļ�

void del(int thisPath, const char* fileName);

//��¼�������������
static const char* commands[COMMAND_COUNT] =
{
	"ver", "login", "init", "info", "cd",
	"dir", "md", "rd", "newfile", "cat",
	"copy", "del", "check", "register", "exit",
	"show_all"
};

extern int emptyDirBlockIndex;  //��ǰ��һ�����еĴ��̿�λ��
extern int emptyFileBlockIndex;  //��ǰ��һ�����е�i�ڵ�λ��

extern block_group blockGroup[BLOCK_GROUPS_NUM];  //���̿���
extern i_node inodeTable[BLOCK_GROUPS_NUM][INODES_NUM_EACH_GROUP];  //i����
extern bitmap inodeBitmap[BLOCK_GROUPS_NUM][INODES_NUM_EACH_GROUP];  //i���λͼ
extern bitmap blockBitmap[BLOCK_GROUPS_NUM][BLOCK_NUM_EACH_GROUP];  //����λͼ

extern vector<user> users;  //��¼�û�
extern user presentUser;  //��¼��ǰ�û�

extern fstream fsVirtualDisk;

extern int currentPathDir;  //��ǰĿ¼
extern vector<dir> vecDir;  //��¼����Ŀ¼
extern vector<file> vecFile;  //��¼�����ļ�
extern char currentPath[MAX_PATH_LENGTH];  //��¼��ǰ·����
