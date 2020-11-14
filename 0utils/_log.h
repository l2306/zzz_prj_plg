#ifndef _log_H_
#define _log_H_
//#define  _CRT_SECURE_NO_WARNINGS 
//不想win下用_s函数 就定义上面变量
#include <stdarg.h>
#include <stdio.h>
#include "_singleton.h"

#define LOG(str)                   Singleton<Log>::instance().open(str);                                                // 文件   
#define LOG_FILE(level,...)        Singleton<Log>::instance().LogFileLine(level, __FILE__, __LINE__, ##__VA_ARGS__);    // 写log     带文件名、行号、时间和信息
#define LOG_FUN(level,...)         Singleton<Log>::instance().LogFunction(level, __FUNCTION__, ##__VA_ARGS__);          // 写log     带模块名，时间和信息的 log
#define LOG_PRINTF(fmt,...)        Singleton<Log>::instance().LogPrintf(fmt,##__VA_ARGS__);                             // 写log     类似printf，具体格式由用户定义；            

typedef enum {
    /** fatal   */      LOG_LEVEL_FATAL = 000,
    /** alert   */      LOG_LEVEL_ALERT,
    /** crit    */      LOG_LEVEL_CRIT,
    /** error   */      LOG_LEVEL_ERROR,
    /** warn    */      LOG_LEVEL_WARN,
    /** notice  */      LOG_LEVEL_NOTICE,
    /** info    */      LOG_LEVEL_INFO,
    /** debug   */      LOG_LEVEL_DEBUG,
    /** trace   */      LOG_LEVEL_TRACE,
    /** notset  */      LOG_LEVEL_NOTSET,
    /** unknown */      LOG_LEVEL_UNKNOWN
}enum_LOG_LEVEL;

#define LOG_INFO    LOG_LEVEL_INFO
#define LOG_WARN    LOG_LEVEL_WARN
#define LOG_ERROR   LOG_LEVEL_ERROR
#define LOG_DEBUG   LOG_LEVEL_DEBUG

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#define xxx_fopen(pFILE,fileName,op)                    fopen_s(&pFILE, fileName, op);
#define xxx_vsnprintf(logstr, max_siz, format, argp)    vsnprintf_s(logstr, max_siz, ((size_t)-1), format, argp);
#define xxx_sprintf                                     sprintf_s
#define xxx_fprintf                                     fprintf_s
#endif

#if defined(__unix__) || defined(unix)  
#include <time.h>
#define xxx_fopen(pFILE,fileName,op)                    pFILE=fopen(fileName, op);
#define xxx_vsnprintf(logstr, max_siz, format, argp)    vsnprintf(logstr, max_siz, format, argp);
#define xxx_sprintf                                     sprintf
#define xxx_fprintf                                     fprintf
#endif

const char *Level[] = {
    "FATAL","ALERT","CRIT","ERROR","WARN","NOTICE",
    "INFO","DEBUG","TRACE","NOTSET","UNKNOWN"
};

#define MAXLINE_SIZE 1024
#define LEN_STR_TIME 32

class Log
{
    FILE*   fp;
    char*   _szFileName;
public:
/*
    // 带时间文件路径和行号的写日志格式；
    bool LogFileLine(
        const int & iLevel,
        const char* strFile,
        const int & iLineNum,
        const char* format,
        ...
    );
    // 带时间和功能模块名的日志；
    bool LogFunction(
        const int&  iLevel,
        const char* strFun,
        const char* format,
        ...
    );
    // 日志内容由客户自己定义 ;
    bool LogPrintf(
        const char* format,
        ...
    );
*/

    Log(void)
        :_szFileName(NULL)
    {
        _szFileName =(char*) "log/default.log";
        FILE* fp = NULL;
    }
    ~Log(void)
    {
        close();
    }
    bool open(const char* szFileName)
    {   
    //#if defined(_WIN32) || defined(_WIN64)
    //  fopen_s(&fp, szFileName, "at+");
    //#endif
    //#if defined(__unix__) || defined(unix) 
    //  fp =fopen(szFileName, "at+");
    //#endif
        xxx_fopen(fp, szFileName, "at+");
    
        if (NULL == fp)
            return false;
        return true;    
    }
    
    bool close()
    {
        if(NULL == fp)
            return false;
        fflush(fp);
        fclose(fp);
        return true;
    }
    
    const char* getStrTime(char* str)
    {
    #if defined(_WIN32) || defined(_WIN64)
        SYSTEMTIME  systime;
        GetLocalTime(&systime);
        sprintf_s(str, LEN_STR_TIME, "%04d-%02d-%02d %02d:%02d:%02d",
            systime.wYear, systime.wMonth, systime.wDay,
            systime.wHour, systime.wMinute, systime.wSecond);
    #endif
    
    #if defined(__unix__) || defined(unix)  
        time_t  tSetTime;
        time(&tSetTime);
        struct  tm* ptm = localtime(&tSetTime);
        sprintf(str, "%04d-%02d-%02d %02d:%02d:%02d",
            ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday,
            ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
    #endif  
        return str;
    }
    
    bool LogFileLine(const int &iLevel, const char *strFile, const int &iLineNum,
        const char *format, ...)
    {
        char        logstr[MAXLINE_SIZE + 1] = {0};
        va_list     argp;
        va_start(argp,format);
        if (NULL == format || 0 == format[0])
            return false;
    
        xxx_vsnprintf(logstr, MAXLINE_SIZE, format, argp);
        va_end(argp);
    
        char        logMsg[2*MAXLINE_SIZE];
        char        strTime[LEN_STR_TIME] ;
        xxx_sprintf(logMsg, "[%s] %s | %s | %d | %s ", getStrTime(strTime),
            Level[iLevel], strFile, iLineNum, logstr);
    
        if (NULL == fp)
            return false;
        xxx_fprintf(fp,"%s\r\n",logMsg);
        fflush(fp);
        return true;
    }
    
    bool LogFunction(const int &iLevel, const char *strFun,
        const char* format, ...)
    {
        char        logstr[MAXLINE_SIZE + 1] ;
        va_list     argp;
        va_start(argp, format);
        if (NULL == format || 0 == format[0])
            return false;
    
        xxx_vsnprintf(logstr, MAXLINE_SIZE, format, argp);
        va_end(argp);
        
        char        logMsg[2 * MAXLINE_SIZE];
        char        strTime[LEN_STR_TIME];
        xxx_sprintf(logMsg, "[%s] %s | %s | %s", getStrTime(strTime),
            Level[iLevel], strFun, logstr);
            
        if (NULL == fp)
            return false;
        xxx_fprintf(fp,"%s\r\n",logMsg);
        fflush(fp);
        return true;
    }
    
    bool LogPrintf( const char* format, ...)
    {
        char        logstr[MAXLINE_SIZE + 1];
        va_list     argp;
        va_start(argp, format);
        if (NULL == format || 0 == format[0])
            return false;
    
        xxx_vsnprintf(logstr, MAXLINE_SIZE, format, argp);
    
        va_end(argp);
        
        if (NULL == fp)
            return false;
        xxx_fprintf(fp,"%s\r\n",logstr);
        fflush(fp);
        return true;
    }
    
};

#endif

