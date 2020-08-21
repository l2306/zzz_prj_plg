
#include "plugin_one.h"


Plugin_1::Plugin_1()
{
    printf("Plugin_1() \n");
}

Plugin_1::~Plugin_1()
{
    printf("~Plugin_1() \n");
}

std::string Plugin_1::getIdx()
{
    return "Plugin_1";
}

int Plugin_1::regMsg(MsgCtl * msgCtl)
{
    msgCtl->regMsgRcv(eMSG_1, this);
    return 0;
}

int Plugin_1::rcvMsg(RefPtr<Msg> p) {
    printf("Plugin_1()::rcvMsg() \n");
    if (NULL != p.get()) {
        printf("\t 中文 \n");
        printf("\t msg.idx = %x \n", p.get()->getIdx());
    }
    return 0;
};

int Plugin_1::Load()
{
    return 0;
}
