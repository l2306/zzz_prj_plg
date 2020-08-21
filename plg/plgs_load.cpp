//动态库中元素
//  1，插件接口PluginI
//  2，导出函数int CreatePlugin(PluginI** outPlugin)
//  3，插件实现类TestPlugin

//可配置插件 以目录为最划分好呢 还是以文件名呢

//插件库源文件

#include "plgs_load.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include "../3rd/tinyXML2/tinyxml2.h"
#include "../3rd/md5/md5.h"
#include "../0utils/_tools.h"

using namespace std;
using namespace tinyxml2;

inline static string getMD5(string str){
    MD5     md5(str);
    return md5.md5();
}

int load_plgs(const char* path, PlgCtl* pPlgCtl)
{
    cout <<endl<< __FUNCTION__ <<endl;
    //首先获取 机器唯一标示   用于计算授权
    string strMac = get_if_mac_2_str();
    string s=getMD5(strMac);
    cout <<" "<< strMac << "  " << s <<endl;

    tinyxml2::XMLDoc doc;
    if ( doc.LoadFile(path) )  
    {  
        doc.PrintError();  
        exit( 1 );  
    }   
    XMLElement* root = doc.RootElement();
    XMLElement* plugins = root->FirstChildElement("plgs");  
    if (plugins)  
    {
        //是否激活全部授权
        bool flag_auth_all = (getMD5(strMac)==plugins->Attribute("auth"));
        //cout << " getMD5( strMac ) 授权(全局)：" << getMD5(strMac) << endl;

        XMLElement *plug = plugins->FirstChildElement();  
        while(plug)
        {
            //加载插件库中 插件类中实例
            handle  hdl;
            PlgI*    plg=NULL;
            int rtn = loadPlg(plug->Attribute("path"), &hdl, &plg);
            switch (rtn)
            {
            case 0:
            {
                //cout << "    getMD5(strMac+plg->getIdx()) 授权：" << getMD5(strMac + plg->getIdx()) << endl;
                bool auth_single = getMD5(strMac + plg->getIdx()) == plug->Attribute("auth");
                if (flag_auth_all || auth_single) {
                    plg->reg(pPlgCtl);
                    cout << "  加载成功，已被授权\n";
                    break;
                } else {
                    cout << "  加载成功，未经授权\n";
                    delete plg;
                    unloadPlg(hdl);
                } break;
            }
            case -1:  cout << "  加载插件文件失败\n";  unloadPlg(hdl);  break;
            case -2:  cout << "  查找函数符号失败\n";  unloadPlg(hdl);  break;
            case -3:  cout << "  找到函数创建失败\n";  unloadPlg(hdl);  break;
            default:  unloadPlg(hdl);
            }
            plug = plug->NextSiblingElement();  
        }  
    }
    return 0;
}

//-------------基础功能

typedef void (*CreatePlg)(PlgI** outPlg);

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>  
HMODULE hdl;
#endif

#if defined(__unix__) || defined(unix)  
#include <dlfcn.h>
void* hdl;  //保存打开的库指针，以便鉴权不成时卸载该库
#endif

//功能 加载插件       
//入 path:插件的路径  
//出 0:加载成功  -1:加载指定文件失败  -2:查找函数符号失败  -3:找到函数但未能创建
int loadPlg(const char* path, handle* o_hdl, PlgI** outPlg) {

    //windows
    //　LoadLibrary          加载指定的动态链接库获得对应的模块句柄（还有其他高级版本）
    //　GetProcAddress       按照指定的符号名称在加载的动态链接库中查找函数地址
    //　FreeLibrary          释放加载的动态链接库

#if defined(_WIN32) || defined(_WIN64)

    *o_hdl = LoadLibrary(path);

    if (nullptr == *o_hdl)
    {
        printf_s("load dll fail \n");
        return -1;
    }

    CreatePlg create = (CreatePlg)GetProcAddress(*o_hdl, "CreatePlg");

    if (nullptr == create)
    {
        printf_s("get proc address fail \n");
        return -2;
    }

    PlgI* plugin = nullptr;
    create(&plugin);

    if (nullptr == plugin)
    {
        printf_s("load plugin fail \n");
        return -3;
    }
    *outPlg = plugin;
    
//  plugin->Load();
//  FreeLibrary(hdl);

    return 0;

#endif

    //linux 
    //  dlopen      按指定模式加载指定路径下的动态库。
    //  dlsym       按照指定的符号名称在加载的动态链接库中查找函数地址
    //  dlclose     关闭释放动态链接库
#if defined(__unix__) || defined(unix)  

    *o_hdl = NULL;
    *o_hdl = dlopen(path, RTLD_LAZY);

    if (NULL == *o_hdl)
    {
        printf("load .so fail   %s %s\n", path, dlerror());
        return -1;
    }

    //CreatePlugin 插件创建接口
    CreatePlg create = (CreatePlg)dlsym(*o_hdl, "CreatePlg");

    char*  err = dlerror();  
    if(err) {  
        printf("can't find sym fcn! \n  %s \n",err);  
        return -2;
    }  
    if (NULL == create)
    {
        printf("get proc sym fail    %s \n", "CreatePlg");
        return -2;
    }

    //PlgI      插件基类
    PlgI* plugin = NULL;
    create(&plugin);

    if (NULL == plugin)
    {
        printf("load plugin fail \n");
        return -3;
    }
    *outPlg = plugin;

//printf("load plugin %s\n",(*o_plugin)->getIdx().c_str()); 
//  plugin->Load();
//  dlclose(hdl);       //这里不能关闭  后面还要使用 从此库中的代码段
//printf("load plugin %s\n",(*o_plugin)->getIdx().c_str()); 

    return 0;

#endif

}


int unloadPlg(handle hdl) {

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
