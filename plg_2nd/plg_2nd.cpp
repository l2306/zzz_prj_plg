
#include "plg_2nd.h"    
#include "../plg_1st/plg_1st.h"

Zz_Link void CreatePlg(PlgI** plg)    { *plg = new Plg_2nd(); }

// 插件所需接口实现

class Plg_2nd::Impl {
public:
    Impl ();
    ~Impl ();
    int set(PlgCtl* p) { plgCtl_ = p; return 0; } 
    int tieOthPlg();
    int init() { return 0; }
    int work();
    int fini() { return 0; }

    int fun_2nd_1();
    int fun_2nd_2();
private:
    PlgCtl*                  plgCtl_;         //方便使用
    RefPtr<PlgI_1st>         plg_1st_;
};

Plg_2nd::Plg_2nd() :impl_(new Impl())     { printf("Plg_2nd() \n"); }
Plg_2nd::~Plg_2nd()                       { printf("~Plg_2nd() \n"); }
std::string Plg_2nd::getIdx()             { return PLG_IDX_Plg_2nd; }
int Plg_2nd::reg(PlgCtl* p) { 
    plgCtl_ = p;  
    impl_.get() && impl_.get()->set(p);
    plgCtl_->reg(this);
    return  0;
};
int Plg_2nd::tieOthPlg()               { return !impl_.get()? -1: impl_.get()->tieOthPlg(); }
int Plg_2nd::init()                    { return !impl_.get()? -1: impl_.get()->init();      }
int Plg_2nd::work()                    { return !impl_.get()? -1: impl_.get()->work();      }
int Plg_2nd::fini()                    { return !impl_.get()? -1: impl_.get()->fini();      }

int Plg_2nd::fun_2nd_1()     { return !impl_.get()? -1: impl_.get()->fun_2nd_1(); }
int Plg_2nd::fun_2nd_2()     { return !impl_.get()? -1: impl_.get()->fun_2nd_2(); }

Plg_2nd::Impl::Impl ()      : plgCtl_(NULL), plg_1st_(NULL)  {}
Plg_2nd::Impl::~Impl ()     {}
int Plg_2nd::Impl::fun_2nd_1()     { cout <<"    Plg_2nd::Impl::fun_2nd_1()"<<endl; return 0; }
int Plg_2nd::Impl::fun_2nd_2()     { cout <<"    Plg_2nd::Impl::fun_2nd_2()"<<endl; return 0; }

int Plg_2nd::Impl::tieOthPlg()
{
    cout << "  Plg_2nd::Impl::tieOthPlg()"<<endl;
    plg_1st_ = dynamic_cast<PlgI_1st*>(plgCtl_->get(PLG_IDX_Plg_1st));
    cout <<"    plg_1st_ "<<plgCtl_->get(PLG_IDX_Plg_1st)<< " 内 " <<plg_1st_.get() <<endl;
    return 0;
}

int Plg_2nd::Impl::work() {
    cout << "  Plg_2nd::Impl::work()" <<endl;
    plg_1st_.get()&&plg_1st_.get()->fun_1st_1();
    return 0;
}