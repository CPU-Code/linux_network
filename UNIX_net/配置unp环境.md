
 * @Author: cpu_code
 * @Date: 2020-05-04 15:27:19
 * @LastEditTime: 2020-05-09 19:07:01
 * @FilePath: \linux_network\UNIX_net\配置unp环境.md
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 

 [详细步骤：https://blog.csdn.net/qq_44226094/article/details/103721083](https://blog.csdn.net/qq_44226094/article/details/103721083)

 
2.解压
```sell
tar -zxvf unpv13e.tar.gz
cd unpv13e/
```

3 配置
```
  ./configure    # try to figure out all implementation differences

   cd lib         # build the basic library that all programs need
    make           # use "gmake" everywhere on BSD/OS systems

cd ../libfree  # continue building the basic library
    make

cd ../libroute # only if your system supports 4.4BSD style routing sockets
    make           # only if your system supports 4.4BSD style routing sockets

cd ../libxti   # only if your system supports XTI
    make           # only if your system supports XTI

    cd ../intro    # build and test a basic client program
    make daytimetcpcli

    ./daytimetcpcli 127.0.0.1
```
这样配置就可以，不管错误信息。

```
cd ..   //回到 unpv13e 目录 

sudo cp libunp.a /usr/lib 

sudo cp libunp.a /usr/lib64
```
```
vim lib/unp.h //将 unp.h 中#include "../config.h"修改为#include "config.h" 

sudo cp lib/unp.h /usr/include 

sudo cp config.h /usr/include
```
这里就是apue.h及err_sys的错误了
: undefined reference to `err_quit'
: undefined reference to`err_sys'

1. 下载 src.3e.tar.gz

2.解压
```
tar -zvx -f src.3e.tar.gz
```
3.进入文件
```
cd apue.3e
```
 4.安装
```
make
```
5.进入include 查看apue.h
```
cd include/
ls
```

 
```
sudo cp apue.h /usr/include
```
4.在/usr/include 下新建一个名的文件
```
cd /usr/include/
vim my_err.h
```
每个项目添加一下头文件my_err.h

5.  内容：
```C
#include "apue.h"
#include <errno.h> /* for definition of errno */
#include <stdarg.h> /* ISO C variable aruments */
static void err_doit(int, int, const char *, va_list);

/*
* Nonfatal error related to a system call.
* Print a message and return.
*/
void err_ret(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    err_doit(1, errno, fmt, ap);
    va_end(ap);
}
/*
* Fatal error related to a system call.
* Print a message and terminate.
*/
void err_sys(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    err_doit(1, errno, fmt, ap);
    va_end(ap);
    exit(1);
}
/*
* Fatal error unrelated to a system call.
* Error code passed as explict parameter.
* Print a message and terminate.
*/
void err_exit(int error, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    err_doit(1, error, fmt, ap);
    va_end(ap);
    exit(1);
}
/*
* Fatal error related to a system call.
* Print a message, dump core, and terminate.
*/
void err_dump(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    err_doit(1, errno, fmt, ap);
    va_end(ap);
    abort(); /* dump core and terminate */
    exit(1); /* shouldn't get here */
}
/*
* Nonfatal error unrelated to a system call.
* Print a message and return.
*/
void err_msg(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    err_doit(0, 0, fmt, ap);
    va_end(ap);
}
/*
* Fatal error unrelated to a system call.
* Print a message and terminate.
*/
void err_quit(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    err_doit(0, 0, fmt, ap);
    va_end(ap);
    exit(1);
}
/*
* Print a message and return to caller.
* Caller specifies "errnoflag".
*/
static void err_doit(int errnoflag, int error, const char *fmt, va_list ap)
{
    char buf[MAXLINE];
   vsnprintf(buf, MAXLINE, fmt, ap);
   if (errnoflag)
       snprintf(buf+strlen(buf), MAXLINE-strlen(buf),": %s",
         strerror(error));
   strcat(buf, " ");
   fflush(stdout); /* in case stdout and stderr are the same */
   fputs(buf, stderr);
   fflush(NULL); /* flushes all stdio output streams */
}
```