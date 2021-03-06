# 设计一

### 要求

在任一OS下，建立一个大文件，把它假象成一张盘，在其中实现一个简单的模拟Linux文件系统。

\1.   在现有机器硬盘上开辟100M的硬盘空间，作为设定的硬盘空间。

\2.   编写一管理程序simdisk对此空间进行管理，以模拟Linux文件系统，要求：

（1） 盘块大小1k 

（2） 空闲盘块的管理：Linux位图法

（3） 结构：超级块, i结点区, 根目录区

\3.   该simdisk管理程序的功能要求如下：

(1)  info: 显示整个系统信息(参考Linux文件系统的系统信息)，文件可以根据用户进行读写保护。目录名和文件名支持全路径名和相对路径名，路径名各分量间用“/”隔开。

(2)  cd …: 改变目录：改变当前工作目录，目录不存在时给出出错信息。

(3)  dir …: 显示目录：显示指定目录下或当前目录下的信息，包括文件名、物理地址、保护码、文件长度、子目录等（带/s参数的dir命令，显示所有子目录）。

(4)  md …: 创建目录：在指定路径或当前路径下创建指定目录。重名时给出错信息。

(5)  rd …: 删除目录：删除指定目录下所有文件和子目录。要删目录不空时，要给出提示是否要删除。

(6)  newfile …: 建立文件。

(7)  cat …: 打开文件。

(8)  copy …: 拷贝文件，除支持模拟Linux文件系统内部的文件拷贝外，还支持host文件系统与模拟Linux文件系统间的文件拷贝，host文件系统的文件命名为<host>…，如：将windows下D：盘的文件\data\sample\test.txt文件拷贝到模拟Linux文件系统中的/test/data目录，windows下D：盘的当前目录为D：\data，则使用命令：

simdisk copy <host>D：\data\sample\test.txt /test/data

或者：simdisk copy <host>D：sample\test.txt /test/data

(9)  del …: 删除文件：删除指定文件，不存在时给出出错信息。

(10) check: 检测并恢复文件系统：对文件系统中的数据一致性进行检测，并自动根据文件系统的结构和信息进行数据再整理。

\4.   程序的总体流程为：

(1)  初始化文件目录；

(2)  输出提示符，等待接受命令，分析键入的命令；

(3)  对合法的命令，执行相应的处理程序，否则输出错误信息，继续等待新命令，直到键入EXIT退出为止。

### 使用说明

1. 运行user\exe\virtualFileSystem.exe。
3. 根据提示执行命令。

#### 三、模拟文件系统相关说明

virtual_file_system.bin的用户数据如下：

| 用户名 | 密码   | 类型          |
| ------ | ------ | ------------- |
| SYSTEM | 123456 | SYSTEM        |
| ADMIN  | 123456 | ADMINISTRATOR |
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

