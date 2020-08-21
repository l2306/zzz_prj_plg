

#ifndef _PLG_ITFC_
#define _PLG_ITFC_

#include <map>
#include <string>
#include <iostream>
#include "../0utils/ref_ptr.h"
using namespace std;

//  插件基类PlgI
class PlgI;
class PlgCtl;

class PlgI {
public:
    PlgI()          { }
    virtual ~PlgI() { }
    virtual int reg(PlgCtl* p) =0;
    virtual string getIdx()    =0;              //标识插件信息    授权
    virtual int tieOthPlg()    =0;              //连接其他插件
    virtual int init()         =0;              //初始化内部实现
    virtual int work()         =0;              //开工
    virtual int fini()         =0;              //
protected:
    PlgCtl* plgCtl_;
};

class PlgCtl {
public:
    PlgCtl()  { }
    ~PlgCtl() { }
    void prt() ;
    void reg(PlgI* p);
    PlgI* get(string s);
    void tieOthPlg();
    void init();
    void work();
    void fini();
private:
    map<string, PlgI*> mapPlgCls;
};

//注意：
//  动态库中接口函数定义原则（让显式加载动态库可以找到创建函数）
//  Zz_Link void CreatePlg(PlgI** outPlg);
//  能      返回 插件指针
//      出  outPlg: 插件库返回插件指针
//  回       0: 创建成功
//  注：  必须加extern "C" 不然dlsym和GetProcAddress无法通过函数名找到函数地址     
//          原因：c++编译后，函数名改变

#if defined(_WIN32) || defined(_WIN64)
#define Zz_Link  extern "C" __declspec(dllexport) 
#endif

#if defined(__unix__) || defined(unix)  
#define Zz_Link  extern "C" 
#endif

#endif
