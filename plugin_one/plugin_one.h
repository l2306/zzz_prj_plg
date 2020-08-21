
#include <stdio.h>
#include "../plugin/plugin_define.h"
#include "../plugin/plugin_itfc.h"
#include "../plugin/z_plugin_msg_ctrl.h"

class Plugin_1:public PluginI{

public:
    Plugin_1();
    ~Plugin_1();
    string getIdx();
    int regMsg(MsgCtl* msgCtl);
    int rcvMsg(RefPtr<Msg> p);
    int Load();
};

Zz_Link int CreatePlugin(PluginI** plugin)
{
    *plugin = new Plugin_1();
    return 0;
}
