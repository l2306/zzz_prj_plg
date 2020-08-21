
#ifndef _obj_vir_
#define _obj_vir_

class ObjVir
{
public:
    void addref()   {};
    void befree()   {};
};

template<typename T>
class ObjVirCom : public ObjVir
{
public:
    ObjVirCom() {};
    virtual ~ObjVirCom() {};
    static T* newObj()  { return dynamic_cast<T*>(new Obj()); }
    class Obj : public T 
    {   
        public:
        Obj() {};
        virtual ~Obj() {};
        void addref()   {};
        void befree()   {};
    };
};

#endif
