
#ifndef _0utils_tools_
#define _0utils_tools_

#include <string>
#include <iostream>

using namespace std;

// 获取 网卡mac地址传入str
string get_if_mac_2_str();

string get_mac();

#define PROFILER() profiler(__FUNCTION__);  //用栈计算某个函数体的执行时间


// -------------------------------- 隐藏

#if defined(_WIN32) || defined(_WIN64)
#include <time.h>
struct profiler {
	profiler(const char* func_name) {
		cout << "--------------------------------------------->>> ";
		start = clock();
	}
	~profiler() {
		finish = clock();
		double duration = (double)(finish - start) / CLOCKS_PER_SEC;
		cout << "--------------------------------------------->>>  " << duration << "secs  or  " << finish - start << "msecs" << endl;
	}
	clock_t start, finish;
};
#endif

#if defined(__unix__) || defined(unix)  
#include <sys/time.h>
struct profiler{
	profiler(const char* func_name){
		gettimeofday(&tv, NULL);
	}
	~profiler(){
		struct timeval tv2;
		gettimeofday(&tv2, NULL);
		long cost = (tv2.tv_sec - tv.tv_sec) * 1000000 + (tv2.tv_usec - tv.tv_usec);
		cout << "-->> " << tv2.tv_sec - tv.tv_sec << "secs =and " << tv2.tv_usec - tv.tv_usec << "msecs" << endl;
	}
	struct timeval tv, tv2;
};
#endif


#endif