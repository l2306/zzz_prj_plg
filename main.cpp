

#include <stdio.h>
#include "0utils/only_one_app.h"
#include "0utils/_log.h"
#include "0utils/_tools.h"
#include "0utils/singleton.h"

#include "plugin/z_plugin_load_ctrl.h"
void test_MsgCtl();   //使用消息总线

#include "plg/plg_itfc.h"
#include "plg/plgs_load.h"
void test_PlgCtl();     //使用声明接口
#define instance_PlgCtl (singleton<PlgCtl>::instance())

int main(){

	LOG("log/ssss.log");
	LOG_PRINTF("===>>>  begain ...%s ", __FILE__);
	LOG_FUN(LOG_LEVEL_ALERT,"11 %d ", 2222);
	LOG_FILE(LOG_LEVEL_ALERT,"7890876543");
	
	//inst_exit_func();
	//if (0 != already_running())
	//	return -1;

    test_PlgCtl();

    //test_MsgCtl();

	LOG_PRINTF("<<<=== started %s ", __FILE__);

	while(1)
		;

	return 0;
}

void test_PlgCtl()
{
	PlgCtl* pPlgCtl = instance_PlgCtl;	
	load_plgs("cfg/auth_plgs.xml", pPlgCtl);
	pPlgCtl->prt();
	pPlgCtl->tieOthPlg();
	pPlgCtl->work();
}


void test_MsgCtl() 
{
	MsgCtl* pMsgCtl = instance_MsgCtl; /* new MsgCtl(); */

	load_plugins("cfg/auth_plugin.xml", pMsgCtl);

	{	//PROFILER();
		//RefPtr<Msg> p= new Msg_1();
		Msg_1* p1= new Msg_1(); 
		Msg_1* p2= new Msg_1(); 

		cout<< "-----" << p1 ;
		p1->stt_msg_1.val1=12;
		p1->stt_msg_1.val2=34;
		pMsgCtl->pubMsg(p1);

		cout<< "-----" << p2 ;
		p2->stt_msg_1.val1=99;
		p2->stt_msg_1.val2=99;
		pMsgCtl->pubMsg(p2);
	}{
		Msg_2* p= new Msg_2(); 
		cout<< "-----" << p ;
		p->stt_msg_2.val1=56;
		p->stt_msg_2.val2=78;
		pMsgCtl->pubMsg(p);
	}{
		
		Msg* p= MsgFactory::newMsg(eMSG_1); 
		cout<< "----" << p ;
		pMsgCtl->pubMsg(p);
	}
	{
		Msg* p= MsgFactory::newMsg(eMSG_2);
		cout<< "----" << p ; 
		pMsgCtl->pubMsg(p);
	}
}