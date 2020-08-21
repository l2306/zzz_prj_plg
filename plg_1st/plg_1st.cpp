
#include "plg_1st.h"
#include "../plg_0/plg_0.h"
#include "../plg_2nd/plg_2nd.h"

Zz_Link void CreatePlg(PlgI** plg)    { *plg = new Plg_1st(); }

class Plg_1st::Impl {
public:
    Impl ();
    ~Impl ();
    int set(PlgCtl* p) { plgCtl_ = p; return 0;} 
    int tieOthPlg();
    int init() { return 0; }
    int work();
    int fini() { return 0; }

    int fun_1st();
private:
    PlgCtl*                 plgCtl_;          //方便使用
    RefPtr<PlgI_0>          plg_0_;
    RefPtr<PlgI_2nd>        plg_2nd_;
};

Plg_1st::Plg_1st() :impl_(new Impl())  { printf("Plg_1st() \n"); }
Plg_1st::~Plg_1st()                    { printf("~Plg_1st() \n"); }
std::string Plg_1st::getIdx()          { return PLG_IDX_Plg_1st; }
int Plg_1st::reg(PlgCtl* p)            { 
    plgCtl_ = p;
    impl_.get() && impl_.get()->set(p);
    plgCtl_->reg(this); 
    return 0;
};
int Plg_1st::tieOthPlg()     { return !impl_.get()? -1: impl_.get()->tieOthPlg(); }
int Plg_1st::init()          { return !impl_.get()? -1: impl_.get()->init();      }
int Plg_1st::work()          { return !impl_.get()? -1: impl_.get()->work();      }
int Plg_1st::fini()          { return !impl_.get()? -1: impl_.get()->fini();      }

int Plg_1st::fun_1st_1()     { return !impl_.get()? -1: impl_.get()->fun_1st(); }
int Plg_1st::fun_1st_2()     { return !impl_.get()? -1: impl_.get()->fun_1st(); }


//-------impl
Plg_1st::Impl::~Impl ()     {}
Plg_1st::Impl::Impl ()      : plgCtl_(NULL), plg_0_(NULL), plg_2nd_(NULL) {}
int Plg_1st::Impl::fun_1st()     { cout <<"    Plg_1st::Impl::fun_1st()"<<endl; return 0; }

int Plg_1st::Impl::tieOthPlg()
{
    printf("  Plg_1st::Impl::tieOthPlg() \n");
    plg_0_   = dynamic_cast<PlgI_0*>(plgCtl_->get(PLG_IDX_Plg_0));
    plg_2nd_ = dynamic_cast<PlgI_2nd*>(plgCtl_->get(PLG_IDX_Plg_2nd));
    cout <<"    plg_0_   " <<plgCtl_->get(PLG_IDX_Plg_0)  <<" 内 "<<plg_0_.get()  <<endl;
    cout <<"    plg_2nd_ " <<plgCtl_->get(PLG_IDX_Plg_2nd)<<" 内 "<<plg_2nd_.get()<<endl;
    return 0;
}

int Plg_1st::Impl::work()
{   
    cout <<"  Plg_1st::Impl::work()"<<endl;
    plg_2nd_.get()&&plg_2nd_.get()->fun_2nd_1();
    return 0;
}