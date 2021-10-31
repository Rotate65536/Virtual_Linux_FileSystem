#include<iostream>
#include<windows.h>
#include<string>
#include<Winbase.h>

using namespace std;

#define MAX_COMMAND_LENGTH 128
#define MAX_PATH_LENGTH 256
#define MAX_CONTENT_LENGTH 1024

const int lengthCmd = MAX_COMMAND_LENGTH + MAX_PATH_LENGTH * 2 + 2;

//共享内存
class ShareMemory
{
public:
    bool toShell;  //simdisk给shell的通知
    bool toSimdisk;  //shell给simdisk的通知
    bool shellOutput;
    char contents[MAX_CONTENT_LENGTH];
    ShareMemory() {
        strcpy(contents, "");
    }
    void clear() {
        strcpy(contents, "");
    }
};

char szName[] = "ShareMemory2";

int main()
{
    system("title shell2");
    system("COLOR e1");
    HANDLE hMapFile = CreateFileMapping(
        INVALID_HANDLE_VALUE,
        NULL,
        PAGE_READWRITE,
        0,
        sizeof(ShareMemory),
        (LPCWSTR)szName
    );

    ShareMemory* pBuf = (ShareMemory*)MapViewOfFile(
        hMapFile,
        FILE_MAP_ALL_ACCESS,
        0,
        0,
        sizeof(ShareMemory)
    );
    pBuf->toShell = true;
    pBuf->toSimdisk = false;
    pBuf->shellOutput = false;

    cout << "SHELL virtualFS. " <<
        "Copyright 2021 Rotate65536. All rights reserved.\n"
        << "Continue（Y/N）";

    while (1)
    {
        if (pBuf->toShell == true)
        {
            string input;
            char strCommandLine[lengthCmd];
            cin.getline(strCommandLine, lengthCmd);
            strcpy(pBuf->contents, strCommandLine);
            pBuf->toShell = false;
            pBuf->toSimdisk = true;
            pBuf->shellOutput == false;
            if (strcmp(strCommandLine, "exit") == 0)
                exit(0);
            while (1)  //忙等待
            {
                if (pBuf->toShell == true)
                    break;
                if (pBuf->shellOutput == true)
                {
                    cout << pBuf->contents;
                    pBuf->clear();
                    pBuf->shellOutput = false;
                }
            }
        }
    }
}