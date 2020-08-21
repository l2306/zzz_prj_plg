//插件式动态库中必要元素 (通过消息总线)
//  1，插件接口 PluginI
//  2，导出函数 int CreatePlugin(PluginI** outPlugin)
//  3，插件测试 TestPlugin

#ifndef _PLUGIN_ITFC_
#define _PLUGIN_ITFC_

#include <stdio.h>
#include "../0utils/ref_ptr.h"
#include "plugin_msg.h"
#include <string>
using namespace std;

//  插件基类PluginI     通过消息总线实现插件库间通信
class Msg;
class MsgCtl;

class PluginI {
public:
    PluginI() { printf("PluginI() \n"); }
    virtual ~PluginI(){ printf("~PluginI() \n"); }
public:
    virtual string getIdx() { return "PluginI"; }           //标识插件信息    授权
    virtual int regMsg(MsgCtl* msgCtl) { return 0; }        //注册接受消息    msgCtl
    int sndMsg() { return 0; };                             //基类实现统一的发送接口 //从msgctl发送消息 这个基本没用
    virtual int rcvMsg(RefPtr<Msg> p) { return 0; };        //不同插件对消息的处理不同 
    virtual int Load() { printf("load \n"); return 0; }     //可根据业务需要添加统一的接口 
};

/*
typedef int(*CreatePlugin)(PluginI** outPlugin);

//下列函数  插件中必实现项
int CreatePlugin(PluginI** plugin){
    *plugin = new Plugin_实现();
    return 0;
}
*/

#endif
