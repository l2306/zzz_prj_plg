
#include <stdio.h>
#include "../plg/plg_itfc.h"

#define PLG_IDX_Plg_1st "Plg_1st"

class PlgI_1st {
public:                                 // 对外接口
    virtual int fun_1st_1() =0;             // 业务代码
    virtual int fun_1st_2() =0;             // 业务代码 
};

class Plg_1st :public PlgI, public PlgI_1st {
public:
    Plg_1st();
    ~Plg_1st();
    virtual int reg(PlgCtl* p);
    virtual string getIdx();
    virtual int tieOthPlg();
    virtual int init();
    virtual int work();
    virtual int fini();
public:                                 // 对外接口
    virtual int fun_1st_1();                // 业务代码
    virtual int fun_1st_2();                // 业务代码
private:
    class Impl;                         // 无具化名 被隐藏的实现      
    RefPtr<Impl>    impl_;          
};
