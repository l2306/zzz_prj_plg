
#ifndef _PLUGIN_MSG_CTRL_
#define _PLUGIN_MSG_CTRL_

#include <map>
#include <vector>
#include <iostream>
#include "../0utils/ref_ptr.h"
#include "plugin_itfc.h"
#include "plugin_msg.h"

using namespace std;

#define instance_MsgCtl (singleton<MsgCtl>::instance())

class MsgCtl{
public:
    //为插件类注册消息
    void regMsgRcv(int msgTyp, PluginI* rcver)
    {
    //  cout << __FUNCTION__<< endl;
        map<int, vector<PluginI*> >::iterator it = mapMsgRcv.find(msgTyp);
        if (mapMsgRcv.end() == it){
            mapMsgRcv[msgTyp].push_back(rcver);
        }else{
            mapMsgRcv[msgTyp].push_back(rcver);
        }
    }
    //为插件类注册消息
    void regMsgRcv(int msgTyp, vector<PluginI*>& vct_rcver)
    {
        map<int, vector<PluginI*> >::iterator it = mapMsgRcv.find(msgTyp);
        if (mapMsgRcv.end() == it)
            for (int i = 0; i < vct_rcver.size();i++)
                mapMsgRcv[msgTyp].push_back(vct_rcver.at(i));
        else
            for (int i = 0; i < vct_rcver.size(); i++)
                mapMsgRcv[msgTyp].push_back(vct_rcver.at(i));
    }
    
    //给插件库发布消息
    void pubMsg(RefPtr<Msg> p){
        if( NULL!=p.get())
        {
            cout <<"   "<< __FUNCTION__<< endl;
            map< int, vector<PluginI*> >::iterator it = mapMsgRcv.find(p.get()->getIdx());
            if (mapMsgRcv.end() != it){
                vector<PluginI*>& rcvObj = it->second;
                for (int i = 0; i < it->second.size(); i++)
                    rcvObj[i]->rcvMsg(p);
            }
        }
    }
private:
    map< int, vector<PluginI*> > mapMsgRcv;
};

#endif
