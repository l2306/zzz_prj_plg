
#include "plugin_two.h"

Plugin_2::Plugin_2()
{
    printf("Plugin_2() \n");
}

Plugin_2::~Plugin_2()
{
    printf("~Plugin_2() \n");
}

std::string Plugin_2::getIdx()
{
    return "Plugin_2";
}

int Plugin_2::regMsg(MsgCtl * msgCtl)
{
    msgCtl->regMsgRcv(eMSG_1, this);
    msgCtl->regMsgRcv(eMSG_2, this);
    return 0;
}

int Plugin_2::rcvMsg(RefPtr<Msg> p) {
    printf("Plugin_2()::rcvMsg() \n");
    Msg* msgI = p.get();
    if (msgI) {
        switch (msgI->getIdx())
        {
        case eMSG_1: {
            //  int* p1=NULL;    *p1=1;
            Msg_1* msg_1 = dynamic_cast<Msg_1*>(msgI);
            cout << msg_1 << endl;
            cout << "\t msg_1->stt_msg_1 " <<
                msg_1->stt_msg_1.val1 << "," << msg_1->stt_msg_1.val2 << endl;
        }   break;
        case eMSG_2: {
            Msg_2* msg_2 = dynamic_cast<Msg_2*>(msgI);
            cout << msg_2 << endl;
            cout << "\t msg_2->stt_msg_2 " <<
                msg_2->stt_msg_2.val1 << "," << msg_2->stt_msg_2.val2 << endl;
        }   break;

        default:
        {
            break;
        }
        }
        //  cout << ends <<"\t msg.idx = "<< p.get()->getIdx() << endl;
    }
    return 0;
};


int Plugin_2::Load()
{
    printf("Plugin_2()::Load() two\n");
    return 0;
}
