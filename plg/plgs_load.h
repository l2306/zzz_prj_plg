
#ifndef _PLG_LOAD_
#define _PLG_LOAD_

#include "plg_itfc.h"

//保存打开的库指针，以便鉴权不成时卸载该库
#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>  
typedef HMODULE  handle;
#endif

#if defined(__unix__) || defined(unix)  
#include <dlfcn.h>
typedef void*    handle;
#endif

//能 加载插件        
//  入   path:       插件的路径
//  出   o_hdl:      插件库句柄
//  出   o_plg:      插件类地址
//回 0:加载成功  -1:加载文件失败  -2:查找函数地址失败
int loadPlg(const char* path, handle* o_hdl, PlgI** o_plg);

//能 卸载插件        
//  入   path:       插件的路径
//回 0:卸载成功  -1:卸载失败
int unloadPlg(handle hdl);

//对外接口
int load_plgs(const char* path, PlgCtl* pPlgCtl);

#endif
