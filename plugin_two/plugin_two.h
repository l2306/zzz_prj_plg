
#include <stdio.h>
#include <iostream>

#include "../plugin/plugin_define.h"
#include "../plugin/plugin_itfc.h"
#include "../plugin/z_plugin_msg_ctrl.h"

using namespace std;

class Plugin_2:public PluginI{

public:
    Plugin_2();
    ~Plugin_2();
    string getIdx();
    int regMsg(MsgCtl* msgCtl);
    int rcvMsg(RefPtr<Msg> p);
    int Load();
};

Zz_Link int CreatePlugin(PluginI** plugin)
{
    *plugin = new Plugin_2();
    return 0;
}



