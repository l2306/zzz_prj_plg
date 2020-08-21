
#include <stdio.h>
#include "../plg/plg_itfc.h"

#define PLG_IDX_Plg_0 "Plg_0"

class PlgI_0{
public:                                     // 对外接口
    virtual int fun_0() =0;                 // 业务代码
};

class Plg_0 :public PlgI, public PlgI_0 {
public:
    Plg_0();
    ~Plg_0();
    virtual int reg(PlgCtl* p);
    virtual string getIdx();
    virtual int tieOthPlg();
    virtual int init();
    virtual int work();
    virtual int fini();
public:                             // 对外接口
    virtual int fun_0();            // 业务代码
private:
    class Impl;                     // 无具化名 被隐藏的实现
    RefPtr<Impl>    impl_;          
};
