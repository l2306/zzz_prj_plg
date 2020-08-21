
#ifndef _PLUGIN_CTRL_
#define _PLUGIN_CTRL_

#include "../3rd/tinyXML2/tinyxml2.h"
#include "../3rd/md5/md5.h"
#include <iostream>
#include <string>
#include "plugin_load.h"
#include "z_plugin_msg_ctrl.h"
#include "../0utils/_tools.h"


using namespace std;
using namespace tinyxml2;

string getMD5(string str){
   MD5      md5(str);
   return md5.md5();
}

int load_plugins(const char* path, MsgCtl* pMsgCtl)
{
    //首先获取 机器唯一标示   用于计算授权
    string strMac = get_if_mac_2_str();
    string s=getMD5(strMac);
    cout << strMac << endl << s <<endl;

    tinyxml2::XMLDoc doc;
    if ( doc.LoadFile(path) )  
    {  
        doc.PrintError();  
        exit( 1 );  
    }   
    XMLElement* root = doc.RootElement();
    XMLElement* plugins = root->FirstChildElement("plugins");  
    if (plugins)  
    {
        //是否激活全部授权
        bool flag_auth_all = (getMD5(strMac)==plugins->Attribute("auth"));
        //cout << " getMD5( strMac ) 授权(全局)：" << getMD5(strMac) << endl;

        XMLElement *plug = plugins->FirstChildElement();  
        while(plug)
        {
            //加载插件库中 插件类中实例
            handle    hdl;
            PluginI* plg=NULL;
            int rtn = loadPlugin(plug->Attribute("path"), &hdl, &plg);
            switch (rtn)
            {
            case 0:
            {
                //cout << " getMD5(strMac+plg->getIdx()) 授权：" << getMD5(strMac + plg->getIdx()) << endl;
                bool auth_single = getMD5(strMac + plg->getIdx()) == plug->Attribute("auth");
                if (flag_auth_all || auth_single) {
                    plg->regMsg(pMsgCtl);
                    break;
                }
                else {
                    cout << "加载成功，但未经授权\n";
                    delete plg;
                    unloadPlugin(hdl);
                }break;
            }
            case -1:  cout << "  加载插件文件失败\n";  unloadPlugin(hdl);  break;
            case -2:  cout << "  查找函数符号失败\n";  unloadPlugin(hdl);  break;
            case -3:  cout << "  找到函数创建失败\n";  unloadPlugin(hdl);  break;
            default:  unloadPlugin(hdl);
            }
            plug = plug->NextSiblingElement();  
        }  
    } 
    return 0;
}

#endif
