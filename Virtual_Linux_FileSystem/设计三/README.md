# 设计三

### 要求

实现多个进程同时对模拟文件系统进行操作。设计管理程序simdisk的用户访问权限管理。访问模拟文件系统的每个进程都属于某个用户，管理程序simdisk根据其访问权限决定其对模拟文件系统的操作。

对模拟文件系统的操作要求做到：共享读，互斥写。

本设计任务在于学会如何实现信息的安全管理和进程同步。

### 使用说明

#### 一、两个用户同时使用虚拟文件系统

1. 运行userOne/exe/shell1，键入Y。
2. 运行userOne/exe/simdisk1。
3. 运行userTwo/exe/shell2，键入Y。
4. 运行userTwo/exe/simdisk2。
5. 根据提示在shell1和shell2的界面输入命令和参数。

#### 二、多个用户同时使用虚拟文件系统

- 生成新的shellx.exe：

  1. 打开sourceCode/shell/shell.cpp，更改szName，szName不能与之前其他的共享内存名相同（已有的共享内存名有：ShareMemory1和ShareMemory2）。

     ```C++
     char szName[] = "ShareMemory2";
     // 更改为——
     char szName[] = "......";
     ```

  2. 为了区别不同的shell，更改system()函数内的shell终端名。

     ```C++
     system("title shell2");
     //更改为
     system("title ......");
     ```

  3. 用C++编译器生成shell.exe，可以自行更名。

- 生成新的simdisk.exe：

  1. 打开sourceCode/simdisk/simdisk.cpp，更改szName，szName和上述所更改的szName相同。
  2. 为了区别不同的simdisk，更改system()函数内的simdisk终端名。
  3. 用C++编译器生成simdisk.exe，可以自行更名。

- 在当前文件夹下新建文件夹userThree，可以自行更名，打开userThree文件夹，新建exe文件夹，将上述生成的shell.exe文件和simdisk.exe文件移动到该文件夹下。然后按“一、两个用户同时使用虚拟文件系统”中步骤运行即可。

#### 三、模拟文件系统相关说明

virtual_file_system.bin的用户数据如下：

| 用户名 | 密码   | 类型          |
| ------ | ------ | ------------- |
| SYSTEM | 123456 | SYSTEM        |
| ADMIN  | 123456 | ADMINISTRATOR |
| V1     | 123456 | VISITOR       |
| V2     | 123456 | VISITOR       |

virtual_file_system.bin的root目录下有以下文件：

| 文件名   | 文件类型  | 权限       | 所有者 |
| -------- | --------- | ---------- | ------ |
| s_f      | FILE      | PROTECTED  | SYSTEM |
| s_folder | DIRECTORY | PROTECTED  | SYSTEM |
| f1       | FILE      | READ_WRITE | V1     |
| folder1  | DIRECTORY | READ_WRITE | V1     |

模拟文件系统命令说明：

| 命令    | 参数1        | 参数2      | 含义                                    |
| ------- | ------------ | ---------- | --------------------------------------- |
| ver     | /            | /          | 显示模拟文件系统版本和作者              |
| login   | /            | /          | 切换用户                                |
| info    | /            | /          | 显示模拟文件系统信息                    |
| cd      | directory    | /          | 进入目标目录                            |
| dir     | /; directory | /          | 显示当前目录信息；显示目标目录信息      |
| md      | directory    | /s；/r；/a | 新建目录：directory（系统；只读；读写） |
| rd      | directory    | /          | 删除目录：directory                     |
| newfile | file         | /s；/r；/a | 新建文件：file（系统；只读；读写）      |
| cat     | file         | /          | 打开并修改文件：file                    |
| copy    | file         | directory  | 将文件file复制到directory               |
| del     | file         | /          | 删除文件：file                          |
| check   | /            | /          | 检查文件系统一致性并恢复                |
| exit    | /            | /          | 退出模拟文件系统                        |

