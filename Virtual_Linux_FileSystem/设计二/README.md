# 设计二

### 要求

实现一个简单的shell（命令行解释器）。

将设计一的管理程序simdisk作为后台进程运行，利用本设计任务的shell操作simdisk。

本设计任务在于学会如何实现在前端的shell进程和后端的simdisk进程之间利用共享内存进行进程间通信(IPC)。

### 使用说明

1. 运行user\exe\newShell.exe，键入Y；
2. 运行\user\exe\newSimdisk.exe。
3. 根据提示执行命令。

#### 三、模拟文件系统相关说明

virtual_file_system.bin的用户数据如下：

| 用户名 | 密码   | 类型          |
| ------ | ------ | ------------- |
| SYSTEM | 123456 | SYSTEM        |
| ADMIN  | ADMIN  | ADMINISTRATOR |
| V1     | 123456 | VISITOR       |
| V2     | 123456 | VISITOR       |

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

