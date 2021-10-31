#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>
#include <conio.h>
using namespace std;

# pragma once

#define BLOCK_SIZE 1024  //盘块大小1KB
#define BLOCK_NUM_EACH_GROUP 1024  //每个数据块组的盘块数
#define BLOCK_GROUPS_NUM 100  //数据块组数
#define INODES_NUM_EACH_GROUP 1024  //每个数据块组的i结点数
#define TOTAL_BLOCKS_NUM (BLOCK_NUM_EACH_GROUP * BLOCK_GROUPS_NUM)  //总的盘块数
#define TOTAL_INODES_NUM (INODES_NUM_EACH_GROUP * BLOCK_GROUPS_NUM)  //总的i-结点数

#define DATA_AREA_ADDR  (sizeof(block_group) * BLOCK_GROUPS_NUM + sizeof(bitmap) * (TOTAL_BLOCKS_NUM + TOTAL_INODES_NUM) + sizeof(i_node) * TOTAL_INODES_NUM)  //文件数据在虚拟磁盘中的地址

#define MAX_NAME_LENGTH  128  //最长文件名
#define MAX_FILES_OF_DIR  256  //一个目录下最大的文件数量
#define MAX_PASSWORD_LENGTH 16  //密码最大长度
#define MAX_PATH_LENGTH 256  //路径最大长度
#define MAX_COMMAND_LENGTH 128  //命令最大长度
#define COMMAND_COUNT 16  //命令数量

//位图元素
enum bitmap
{
	EMPTY = 0,
	OCCUPIED = 1
};

//文件类型
enum file_type
{
	typeFILE,  //文件类型
	typeDIRECTORY,  //目录类型
	typeBlock  //空块类型
};

//读写权限
enum file_authority
{
	PROTECTED,	//系统
	READ_WRITE,	//读写
	READ_ONLY,	//只读
};

//用户类型
enum user_type
{
	VISITOR,  //游客
	ADMINISTRATOR,  //管理员
	SYSTEM  //系统
};


//超级块类
struct super_block {
	unsigned int nFreeBlocks;  //空闲块总数
	unsigned int nFreeInodes;  //空闲i结点总数
};

//数据块组信息类
struct group_info {
	unsigned int indexBlockBmp;  //数据块位图索引
	unsigned int indexInodeBmp;  //i-结点位图索引
	unsigned int indexInodeTable;  //i-结点表索引
	unsigned long blockAddr;  //所在数据区地址
	unsigned int nFreeBlocksNum;  //空闲块的数目
	unsigned int nFreeInodesNum;  //空闲i-结点的数目
};

//数据块组类
struct block_group
{
	//group_info groupInfo;  //数据块组信息
	super_block superBlock;  //超级块
};

//i节点
struct i_node
{
	char strName[MAX_NAME_LENGTH];  //目录名和文件名
	time_t createTime;  //创建时间
	int id;  //所有者账号

	//时间例子
	/*
		time_t t;
		t = time(&t);
		char* pt = ctime(&t);
		cout << pt;
	*/

	file_authority authority;  //读写权限
	file_type type;  //文件类型：目录/（二进制）文件
	long addrBlock;  //磁盘块起始地址
	long size;  //大小
	int nBlock;//默认只占用一个磁盘块
};

//空块
struct emptyBlock
{
	char content[1024];
};

//目录块 1024B
struct dir  
{
	dir(){}
	dir(short ng, short ni)
	{
		nGroup = ng;
		nInode = ni;
		nFile = 0;
	}
	short nGroup;  //组号 2B
	short nInode;  //i结点号 2B
	int nFile;  //该目录下：文件数+目录数 4B
	short nSubInodenGroup[MAX_FILES_OF_DIR - 2];  //存储该目录下i结点的i结点号 1016B
	short nSubInodenInode[MAX_FILES_OF_DIR - 2];
};

//文件块 1024B
struct file  
{
	file() {}
	file(short ng, short ni, char* c)
	{
		nGroup = ng;
		nInode = ni;
		strcpy(content, c);
	}
	short nGroup;  //组号 2B
	short nInode;  //i结点号 2B
	char content[1020];  //文件内容 1020B
};

//用户
struct user
{
	int id;  //账号
	char name[32];
	user_type type;  //类型
	char password[32];  //密码
	user() {
		strcpy(password, "123456");  //默认密码为123456
		id = -1;
		type = VISITOR;  //默认用户类型为游客
		strcpy(name, "Anonymous");
	}
	user(char* s, int i, user_type t, char* pwd) {
		strcpy(password, pwd);
		id = i;
		type = t;
		strcpy(name, s);
	}
};

//更新磁盘信息
void updateFileInfo(int indexGroup, int index, const char* fileName, 
	file_type type, file_authority authority, int blockAddr, int size);  
//加载磁盘
void loadDisk();  
//初始化模拟文件系统
void initFileSystem(); 
//保存磁盘
void saveDisk();  

//全部命令
void execute_cmd(const char* strCommand, const char* strPath1, const char* strPath2);
void cmd_ver();  //显示版本和作者
bool cmd_login();  //切换当前用户
void cmd_info();  //显示当前模拟文件系统的信息
void cmd_cd(const char* path);  //进入目录
void cmd_dir(const char* path);   //显示目录下文件信息
void cmd_md(const char* path, file_authority authority);  //建立目录
void cmd_rd(const char* path, int thisPath);  //删除目录
void cmd_newfile(const char* path, file_authority authority);
void cmd_cat(const char* path);  //打开并显示文件内容
void cmd_copy(const char* srcPath, const char* desPath);  //复制文件
void cmd_del(const char* path);  //删除文件
void cmd_check();  //检查模拟文件系统一致性并恢复
void cmd_exit();  //退出模拟文件系统
void cmd_show_files();  ///显示所有文件

void del(int thisPath, const char* fileName);

//记录所有命令的输入
static const char* commands[COMMAND_COUNT] =
{
	"ver", "login", "init", "info", "cd",
	"dir", "md", "rd", "newfile", "cat",
	"copy", "del", "check", "register", "exit",
	"show_all"
};

extern int emptyDirBlockIndex;  //当前第一个空闲的磁盘块位置
extern int emptyFileBlockIndex;  //当前第一个空闲的i节点位置

extern block_group blockGroup[BLOCK_GROUPS_NUM];  //磁盘块组
extern i_node inodeTable[BLOCK_GROUPS_NUM][INODES_NUM_EACH_GROUP];  //i结点表
extern bitmap inodeBitmap[BLOCK_GROUPS_NUM][INODES_NUM_EACH_GROUP];  //i结点位图
extern bitmap blockBitmap[BLOCK_GROUPS_NUM][BLOCK_NUM_EACH_GROUP];  //磁盘位图

extern vector<user> users;  //记录用户
extern user presentUser;  //记录当前用户

extern fstream fsVirtualDisk;

extern int currentPathDir;  //当前目录
extern vector<dir> vecDir;  //记录已有目录
extern vector<file> vecFile;  //记录已有文件
extern char currentPath[MAX_PATH_LENGTH];  //记录当前路径名
