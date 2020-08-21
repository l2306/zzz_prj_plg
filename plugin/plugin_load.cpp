//动态库中元素
//  1，插件接口PluginI
//  2，导出函数int CreatePlugin(PluginI** outPlugin)
//  3，插件实现类TestPlugin



//可配置插件 以目录为最划分好呢 还是以文件名呢

//插件库源文件

#include <stdio.h>
#include "plugin_itfc.h"
#include "plugin_load.h"

typedef int(*CreatePlugin)(PluginI** outPlugin);

//功能 加载插件       
//入 path:插件的路径  
//出 0:加载成功  -1:加载指定文件失败  -2:查找函数符号失败  -3:找到函数但未能创建
int loadPlugin(const char* path, handle* p_hdl, PluginI** o_plugin) {

    //windows
    //　LoadLibrary          加载指定的动态链接库获得对应的模块句柄（还有其他高级版本）
    //　GetProcAddress       按照指定的符号名称在加载的动态链接库中查找函数地址
    //　FreeLibrary          释放加载的动态链接库

#if defined(_WIN32) || defined(_WIN64)

    *p_hdl = LoadLibrary(path);

    if (*p_hdl == nullptr)
    {
        printf_s("load dll fail \n");
        return -1;
    }

    CreatePlugin create = (CreatePlugin)GetProcAddress(*p_hdl, "CreatePlugin");

    if (create == nullptr)
    {
        printf_s("get proc address fail \n");
        return -2;
    }

    PluginI* plugin = nullptr;
    int res = create(&plugin);

    if (res != 0 || plugin == nullptr)
    {
        printf_s("load plugin fail \n");
        return -3;
    }
    *o_plugin = plugin;
    
//  plugin->Load();
//  FreeLibrary(handle);

    return 0;

#endif

    //linux 
    //  dlopen      按指定模式加载指定路径下的动态库。
    //  dlsym       按照指定的符号名称在加载的动态链接库中查找函数地址
    //  dlclose     关闭释放动态链接库
#if defined(__unix__) || defined(unix)  

    *p_hdl = NULL;
    *p_hdl = dlopen(path, RTLD_NOW);

    if (NULL == *p_hdl)
    {
        printf("load .so fail \n");
        return -1;
    }

    //CreatePlugin 插件创建接口
    CreatePlugin create = (CreatePlugin)dlsym(*p_hdl, "CreatePlugin");

    char*  err = dlerror();  
    if(err) {  
        printf("can't find sym fcn! \n  %s \n",err);  
        return -2;
    }  
    if (create == NULL)
    {
        printf("get proc sym fail \n");
        return -2;
    }

    //PluginI       插件基类
    PluginI* plugin = NULL;
    int rtn = create(&plugin);

    if (rtn != 0 || plugin == NULL)
    {
        printf("load plugin fail \n");
        return -3;
    }
    *o_plugin = plugin;

//printf("load plugin %s\n",(*o_plugin)->getIdx().c_str()); 
//  plugin->Load();
//  dlclose(handle);        //这里不能关闭  后面还要使用 从此库中的代码段
//printf("load plugin %s\n",(*o_plugin)->getIdx().c_str()); 

    return 0;

#endif

}

int unloadPlugin(handle hdl) {

#if defined(_WIN32)||defined(_WIN64)
    if (0 == FreeLibrary(hdl))
        return 0;
    else
        return 1;   
#endif

#if defined(__unix__)||defined(unix)  
    if(hdl)
        if (0 == dlclose(hdl))
            return 0;
        else
            return 1;
    return 0;
#endif

}
