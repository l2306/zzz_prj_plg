
#include "plugin_msg.h"

Msg* MsgFactory::newMsg(enum_MsgIdx idx)
{
    switch (idx)
    {
    case eMSG_1:
        return new Msg_1();

    case eMSG_2:
        return new Msg_2();

    default:
        return NULL;
    }
    return NULL;
}