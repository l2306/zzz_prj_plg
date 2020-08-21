

#ifdef  __cplusplus  
//extern "C" {
#endif

// 返回：0 正常创建 1 已经存在 -1 创建失败
int already_running();

#if defined(_WIN32) || defined(_WIN64)

#include <windows.h>
#include <iostream>

// 返回：0 正常创建 1 已经存在 -1 创建失败
int already_running()
{
        HANDLE hMutex = NULL;
        TCHAR * lpszName = "zzz_one";
        int nRet = 0;
        do{
            hMutex = ::CreateMutex(NULL, FALSE, lpszName);
            DWORD dwRet = ::GetLastError();
            switch (dwRet)
            {
                case 0:     {}break;
                case ERROR_ALREADY_EXISTS: {
                    MessageBox(NULL, "通知：该应用程序已经开启。\n", "通知", MB_OK);
                    nRet = 1;
                }   break;
                default:{
                    MessageBox(NULL, "提示：该应用程序创建互斥对象失败。\n", "错误", MB_OK);
                    nRet = -1;   
                }   break;
            }
        } while (FALSE);
        return nRet;
}

#endif

#if defined(__unix__) || defined(unix)  

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

    //若文件已经上锁   说明程序已经在运行 直接调用exit退出
    //回 true    正常上锁
    //      false   未能上锁

    int already_running()
    {
        //  或者
        //char lock_file[128];
        //snprintf(lock_file, sizeof(lock_file), "/var/tmp/%s.lock", proc_name);
        //  可注册退出函数 对创建的文件进行清理
        const char * pFileName = "/tmp/zzz_one.lck";
        int fd = open(pFileName, O_CREAT | O_RDWR);
        if (fd<0)
        {
            printf("open file failed.\n");
            printf("errno = %d， %s \n", errno, strerror(errno));
            return -1;
        }
        //0 未加锁或者被当前进程加锁；-1 被其他进程加锁
        int lock_result = lockf(fd, F_TEST, 0);
        if (lock_result<0)
        {
            perror("another process holds a lock.\n");
            exit(1);
            return 1;
        }
        //F_LOCK，若已经加锁则阻塞到前一个进程释放锁为止
        lock_result = lockf(fd, F_LOCK, 0);
        if (lock_result<0)              //0 加锁成功 -1 加锁失败
        {
            perror("exec lockf function failed.\n");
            printf("errno = %d， %s \n", errno, strerror(errno));
            return -1;
        }

        return 0;
    }
/*
保证服务进程只有一个实例在运行。        常见实现方式：
1. 通过已知的进程名，来查询是否有同名的进程正在运行。
    可以利用proc，也可以读取ps的输出等；
2. 利用pid文件，这也是linux各种服务常见的实现方式：
    服务进程启动的时候，首先在指定目录下，一般为/var/run/，查找是否已经存在对应该进程的pid文件。
    如果已经存在，表明有同样的进程在运行。
    但是也许该进程意外崩溃，所以需要进一步检查。读取该pid文件，获得pid。
    然后再利用确定该pid的进程是否存在。如存在，是否为同名进程
3.利用非阻塞的文件锁，对相应的文件进行上锁
    在正常退出或意外崩溃时，Linux内核本身都会关闭该文件描述符
    此种状态感觉 flock没有lockf好用   flock(fd_lck, LOCK_EX | LOCK_NB)
*/
#endif

#ifdef  __cplusplus  
//}
#endif
