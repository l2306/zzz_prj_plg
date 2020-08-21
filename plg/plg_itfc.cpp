
#include "plg_itfc.h"

using namespace std;

#if defined(_WIN32) || defined(_WIN64)
#define __FUNCTION__ __FUNCTION__
#endif

#if defined(__unix__) || defined(unix)  
#define __FUNCTION__ __PRETTY_FUNCTION__
#endif

void PlgCtl::reg(PlgI* p) { 
    mapPlgCls[p->getIdx()] = p; 
};

PlgI* PlgCtl::get(string s) {
    map<string, PlgI*>::iterator it;
    return  (mapPlgCls.end() != (it = mapPlgCls.find(s))) ? it->second : NULL ;
};

void PlgCtl::prt() {
    cout << endl << __FUNCTION__ << endl;
    map<string, PlgI*>::iterator it = mapPlgCls.begin();
    while ( mapPlgCls.end() != it){
        cout <<"  -  "<< it->second <<" "<< it->first <<endl;  it++;
    }
}

void PlgCtl::tieOthPlg() {
    cout << endl << __FUNCTION__ << endl;
    map<string, PlgI*>::iterator it = mapPlgCls.begin();
    while ( mapPlgCls.end() != it)
        (it++)->second->tieOthPlg();
}

void PlgCtl::init() {
    cout << endl << __FUNCTION__ << endl;
    map<string, PlgI*>::iterator it = mapPlgCls.begin();
    while ( mapPlgCls.end() != it)
        (it++)->second->init();
}

void PlgCtl::work() {
    cout << endl << __FUNCTION__ << endl;
    map<string, PlgI*>::iterator it = mapPlgCls.begin();
    while ( mapPlgCls.end() != it)
        (it++)->second->work();
}

void PlgCtl::fini() {
    cout <<endl<< __FUNCTION__ <<endl;
    map<string, PlgI*>::iterator it = mapPlgCls.begin();
    while ( mapPlgCls.end() != it)
        (it++)->second->fini();
}
