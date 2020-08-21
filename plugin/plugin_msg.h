
#ifndef _PLUGIN_MSG_
#define _PLUGIN_MSG_


#include <map>
#include <vector>
#include <iostream>
#include "../0utils/ref_ptr.h"
#include "plugin_itfc.h"

using namespace std;

//用于区分不同的协议 
typedef enum MsgIdx{
    eMSG_1 =0x1111,
    eMSG_2 =0x2222,
    eMSG_3 =0x3333,
}enum_MsgIdx;


class Msg{
public:
    Msg()                   { /* cout << "Msg() \n"; */  }
    Msg(int idx)  :idx(idx) { /* cout << "Msg() \n"; */  }
    virtual ~Msg()          { /* cout << "~Msg() \n";*/  }
    virtual int getIdx()    { return idx; }
protected:
    int idx;    //索引        :标识消息 
private:
    int src;    //发送方 
    int dst;    //接收方   
};

typedef struct {
    int val1;
    int val2;
} Stt_Msg_1;

class Msg_1:public Msg{
public:
    Msg_1()
        :Msg(eMSG_1)
    { 
        cout << "Msg_1() \n";
    }
    virtual ~Msg_1(){ 
        cout << "~Msg_1() \n";
    }
    Stt_Msg_1  stt_msg_1;
    int msg_1;
private:
    int src;    //发送方 
    int dst;    //接收方   
};


typedef struct {
    long val1;
    long val2;
} Stt_Msg_2;

class Msg_2:public Msg{
public:
    Msg_2()
        :Msg(eMSG_2)
    { 
        cout << "Msg_2() \n";
    }
    virtual ~Msg_2(){ 
        cout << "~Msg_2() \n";
    }
    Stt_Msg_2 stt_msg_2;
private:
    int src;    //发送方 
    int dst;    //接收方 
};


class MsgFactory
{
public:
//  MsgFactory(){ 
//  }
//  ~MsgFactory(){ 
//  } 
    //简单工厂  
    static Msg* newMsg(enum_MsgIdx idx);
};

#endif
