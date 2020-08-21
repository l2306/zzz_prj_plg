
#ifndef _PLUGIN_LOAD_
#define _PLUGIN_LOAD_

#include "plugin_itfc.h"

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
//  出   o_plugin:   插件类地址
//回 0:加载成功  -1:加载文件失败  -2:查找函数地址失败
int loadPlugin(const char* path, handle* p_hdl, PluginI** o_plugin);

//能 卸载插件        
//  入   path:       插件的路径
//回 0:卸载成功  -1:卸载失败
int unloadPlugin(handle hdl);

//注意：
//  动态库中接口函数定义原则
//Zz_Link CreatePlugin(PluginI** outPlugin);
#if defined(_WIN32) || defined(_WIN64)
#define Zz_Link  extern "C" __declspec(dllexport) 
#endif

#if defined(__unix__) || defined(unix)  
#define Zz_Link  extern "C" 
#endif


#endif
