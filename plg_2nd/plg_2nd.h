
#include <stdio.h>
#include <iostream>
#include "../plg/plg_itfc.h"

using namespace std;

#define PLG_IDX_Plg_2nd "Plg_2nd"

class PlgI_2nd {
public:
    virtual int fun_2nd_1() =0;             //业务代码
    virtual int fun_2nd_2() =0;             //业务代码
};

class Plg_2nd :public PlgI, public PlgI_2nd {
public:
    Plg_2nd();
    ~Plg_2nd();
    virtual int reg(PlgCtl* p);
    virtual string getIdx();
    virtual int tieOthPlg();
    virtual int init();
    virtual int work();
    virtual int fini();
public:
    virtual int fun_2nd_1();                //业务代码
    virtual int fun_2nd_2();                //业务代码
private:
    class Impl;             //无需具化名称，       
    RefPtr<Impl>    impl_;
};
