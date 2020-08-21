
#include "plg_0.h"
#include <iostream>

using namespace std;

Zz_Link void CreatePlg(PlgI** plg)    { *plg = new Plg_0(); }

class Plg_0::Impl {
public:
    Impl ();
    ~Impl ();
    int set(PlgCtl* p) { plgCtl_ = p; return 0;} 
    int tieOthPlg();
    int init() { return 0; }
    int work();
    int fini() { return 0; }

    int fun_0() { return 0; }
private:
    PlgCtl*                 plgCtl_;          //方便使用
    //RefPtr<Plg_3rd>         plg_3rd_;
};

Plg_0::Plg_0() :impl_(new Impl())  { printf("Plg_0() \n"); }
Plg_0::~Plg_0()                    { printf("~Plg_0() \n"); }
std::string Plg_0::getIdx()        { return PLG_IDX_Plg_0; }
int Plg_0::reg(PlgCtl* p)          { 
    plgCtl_ = p;
    impl_.get() && impl_.get()->set(p);
    plgCtl_->reg(this); 
    return 0;
};
int Plg_0::tieOthPlg()     { return !impl_.get()? -1: impl_.get()->tieOthPlg(); }
int Plg_0::init()          { return !impl_.get()? -1: impl_.get()->init();      }
int Plg_0::work()          { return !impl_.get()? -1: impl_.get()->work();      }
int Plg_0::fini()          { return !impl_.get()? -1: impl_.get()->fini();      }

int Plg_0::fun_0()         { return !impl_.get()? -1: impl_.get()->fun_0();     }

//-------impl

Plg_0::Impl::~Impl ()       {}
Plg_0::Impl::Impl ()        : plgCtl_(NULL) {}

int Plg_0::Impl::tieOthPlg()
{
    return 0;
}

int Plg_0::Impl::work()
{
    cout <<"  Plg_0::Impl::work()"<<endl;
    return 0;
}