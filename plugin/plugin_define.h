
#ifndef _PLUGIN_PART_NEED_
#define _PLUGIN_PART_NEED_

//注意：
//  动态库中接口函数定义原则（让显式加载动态库可以找到创建函数）
//  Zz_Link int CreatePlugin(PluginI** outPlugin);

/*
能       返回Plugin_1插件指针
    出   plugin: 插件库返回插件指针
回       0: 创建成功
注：  必须加extern "C"
        不然dlsym和GetProcAddress无法通过函数名找到函数地址     
        原因：c++编译后，函数名改变
*/

#if defined(_WIN32) || defined(_WIN64)
#define Zz_Link  extern "C" __declspec(dllexport) 
#endif

#if defined(__unix__) || defined(unix)  
#define Zz_Link  extern "C" 
#endif

#endif
