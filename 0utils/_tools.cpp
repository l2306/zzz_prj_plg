
#include "_tools.h"


#if defined(__unix__) || defined(unix)  

#include <net/if.h>         /* for ifconf */
#include <linux/sockios.h>  /* for net status mask */
#include <netinet/in.h>     /* for sockaddr_in */
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include <string>
#include <iostream>

using namespace std;

#define MAX_INTERFACE   (16)

//获取指定网卡的 的mac地址
string getLocalMacAddr(const char* name_net_dev)
{
    int sock_mac;
    
    struct ifreq ifr_mac;
    char mac_addr[30];   
    
    sock_mac = socket( AF_INET, SOCK_STREAM, 0 );
    if( sock_mac == -1)
    {
        perror("create socket falise...mac \n");
        return "";
    }
    
    memset(&ifr_mac,0,sizeof(ifr_mac));   
    strncpy(ifr_mac.ifr_name, name_net_dev, sizeof(ifr_mac.ifr_name)-1);  


    if( (ioctl( sock_mac, SIOCGIFHWADDR, &ifr_mac)) < 0)
    {
        printf("mac ioctl error \n");
        return "";
    }
    
    sprintf(mac_addr,"%02x%02x%02x%02x%02x%02x",
            (unsigned char)ifr_mac.ifr_hwaddr.sa_data[0],
            (unsigned char)ifr_mac.ifr_hwaddr.sa_data[1],
            (unsigned char)ifr_mac.ifr_hwaddr.sa_data[2],
            (unsigned char)ifr_mac.ifr_hwaddr.sa_data[3],
            (unsigned char)ifr_mac.ifr_hwaddr.sa_data[4],
            (unsigned char)ifr_mac.ifr_hwaddr.sa_data[5]);

    printf("local mac:%s \n",mac_addr);    
    
    close( sock_mac );
    return string( mac_addr );
}

void port_status(unsigned int flags);

/* set == 0: do clean , set == 1: do set! */
int set_if_flags(char *pif_name, int sock, int status, int set)
{
    struct ifreq ifr;
    int ret = 0;
    
    strncpy(ifr.ifr_name, pif_name, strlen(pif_name) + 1);
    ret = ioctl(sock, SIOCGIFFLAGS, &ifr);
    if(ret)
        return -1;
    /* set or clean */  
    if(set)
        ifr.ifr_flags |= status;
    else 
        ifr.ifr_flags &= ~status;
    /* set flags */
    ret = ioctl(sock, SIOCSIFFLAGS, &ifr);
    if(ret)
        return -1;
    
    return 0;
}

//通过socket获取 各网卡信息
int get_if_info(int fd)
{
    struct ifreq buf[MAX_INTERFACE];    
    struct ifconf ifc;
    int ret = 0;
    int if_num = 0;

    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = (caddr_t) buf;
    
    ret = ioctl(fd, SIOCGIFCONF, (char*)&ifc);
    if(ret)
    {
        printf("get if config info failed");
        return -1;
    }
    /* 网口总数 ifc.ifc_len 应该是一个出入参数 */    
    if_num = ifc.ifc_len/sizeof(struct ifreq);
    printf("interface num is interface = %d\n", if_num);

    while(if_num-- > 0)
    {
        printf("net device: %s\n", buf[if_num].ifr_name);

        /* 获取第n个网口信息 */
        ret = ioctl(fd, SIOCGIFFLAGS, (char*)&buf[if_num]);
        if(ret)
            continue;

        /* 获取网口状态 */
        port_status(buf[if_num].ifr_flags);
        
        /* 获取当前网卡的ip地址 */
        ret = ioctl(fd, SIOCGIFADDR, (char*)&buf[if_num]);
        if(ret)
            continue;
        printf("IP addr is: \n%s\n", inet_ntoa(((struct sockaddr_in *)(&buf[if_num].ifr_addr))->sin_addr));

        /* 获取当前网卡的mac */
        ret = ioctl(fd, SIOCGIFHWADDR, (char*)&buf[if_num]);
        if(ret)
            continue;

        printf("%02x:%02x:%02x:%02x:%02x:%02x\n\n",
            (unsigned char)buf[if_num].ifr_hwaddr.sa_data[0],
            (unsigned char)buf[if_num].ifr_hwaddr.sa_data[1],
            (unsigned char)buf[if_num].ifr_hwaddr.sa_data[2],
            (unsigned char)buf[if_num].ifr_hwaddr.sa_data[3],
            (unsigned char)buf[if_num].ifr_hwaddr.sa_data[4],
            (unsigned char)buf[if_num].ifr_hwaddr.sa_data[5]
        );
    }
    return 0;
}

void port_status(unsigned int flags)
{
    if(flags & IFF_UP)  
    {
        printf("is up\n");      
    }
    if(flags & IFF_BROADCAST)   
    {
        printf("is broadcast\n");   
    }
    if(flags & IFF_LOOPBACK)    
    {
        printf("is loop back\n");   
    }
    if(flags & IFF_POINTOPOINT) 
    {
        printf("is point to point\n");  
    }
    if(flags & IFF_RUNNING) 
    {
        printf("is running\n"); 
    }
    if(flags & IFF_PROMISC) 
    {
        printf("is promisc\n"); 
    }
}

//通过socket获取 各网卡信息  注意只有网卡不被禁用时才生效
string get_if_mac_2_str()
{
    string str_mac;
    struct ifreq buf[MAX_INTERFACE];    
    struct ifconf ifc;
    int ret = 0;
    int if_num = 0;

    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = (caddr_t) buf;

    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd <= 0)
    {
        printf("con`t create socket");
        return "";
    }
    
    ret = ioctl(fd, SIOCGIFCONF, (char*)&ifc);
    if(ret)
    {
        printf("get if config info failed");
        return "";
    }
    /* 网口总数 ifc.ifc_len 应该是一个出入参数 */    
    if_num = ifc.ifc_len/sizeof(struct ifreq);

    while(if_num-- > 0)
    {

        /* 获取第n个网口信息 */
        ret = ioctl(fd, SIOCGIFFLAGS, (char*)&buf[if_num]);
        if(ret)
            continue;
        
        /* 获取当前网卡的ip地址 
        ret = ioctl(fd, SIOCGIFADDR, (char*)&buf[if_num]);
        if(ret)
            continue;
        */

        /* 获取当前网卡的mac */
        ret = ioctl(fd, SIOCGIFHWADDR, (char*)&buf[if_num]);
        if(ret)
            continue;

        char mac[32] = {0};
        sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x",
            (unsigned char)buf[if_num].ifr_hwaddr.sa_data[0],
            (unsigned char)buf[if_num].ifr_hwaddr.sa_data[1],
            (unsigned char)buf[if_num].ifr_hwaddr.sa_data[2],
            (unsigned char)buf[if_num].ifr_hwaddr.sa_data[3],
            (unsigned char)buf[if_num].ifr_hwaddr.sa_data[4],
            (unsigned char)buf[if_num].ifr_hwaddr.sa_data[5]
        );

        string  str(mac,17);
        if("00:00:00:00:00:00" != str)
            str_mac += str;
    }
    close(fd);
    return str_mac;
}


int test()
{
    cout << get_if_mac_2_str().c_str() <<endl;
    return 0;
}

#endif


#if defined(_WIN32) || defined(_WIN64)

#define _WINSOCK_DEPRECATED_NO_WARNINGS
//错误    C4996   'inet_ntoa': Use inet_ntop() or InetNtop() instead 
//  or define _WINSOCK_DEPRECATED_NO_WARNINGS to disable deprecated API warnings
//错误    C4996   'gethostbyname': Use getaddrinfo() or GetAddrInfoW() instead 
//   or define _WINSOCK_DEPRECATED_NO_WARNINGS to disable deprecated API warnings   

#include <winsock2.h>
#include <string.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "netapi32.lib")

#include <WinSock2.h>
#include <Iphlpapi.h>
#include <iostream>
using namespace std;
#pragma comment(lib,"Iphlpapi.lib") //需要添加Iphlpapi.lib库
//#pragma comment(lib, "ws2_32.lib")

#define _CRT_SECURE_NO_WARNINGS

bool isRealLocalAapter(const char *pAdapterName)
{
#define NET_CARD_KEY "SYSTEM\\CurrentControlSet\\Control\\Network\\{4D36E972-E325-11CE-BFC1-08002BE10318}"    
    BOOL ret_value = FALSE;
    char szDataBuf[MAX_PATH + 1];
    DWORD dwDataLen = MAX_PATH;
    DWORD dwType = REG_SZ;
    HKEY hNetKey = NULL;
    HKEY hLocalNet = NULL;
    if (ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, NET_CARD_KEY, 0, KEY_READ, &hNetKey))
        return FALSE;
    sprintf_s(szDataBuf, "%s\\Connection", pAdapterName);
    if (ERROR_SUCCESS != RegOpenKeyEx(hNetKey, szDataBuf, 0, KEY_READ, &hLocalNet))
    {
        RegCloseKey(hNetKey);
        return FALSE;
    }
    if (ERROR_SUCCESS != RegQueryValueEx(hLocalNet, "MediaSubType", 0, &dwType, (BYTE *)szDataBuf, &dwDataLen)) {
        goto ret;
    }
    if (*((DWORD *)szDataBuf) != 0x02)
        goto ret;
    dwDataLen = MAX_PATH;
    if (ERROR_SUCCESS != RegQueryValueEx(hLocalNet, "PnpInstanceID", 0, &dwType, (BYTE *)szDataBuf, &dwDataLen)) {
        goto ret;
    }
    if (strncmp(szDataBuf, "PCI", strlen("PCI")))
        goto ret;
    ret_value = TRUE;
ret:
    RegCloseKey(hLocalNet);
    RegCloseKey(hNetKey);
    return ret_value;

    //若PnpInstanceID的值以PCI开头，说明是物理网卡，
    //  MediaSubType    01:常见网卡 02:无线网卡 
}


string get_mac()
{
    PIP_ADAPTER_INFO    pIpAdapterInfo = new IP_ADAPTER_INFO();                 //PIP_ADAPTER_INFO结构体指针存储本机网卡信息
    unsigned long       stSize = sizeof(IP_ADAPTER_INFO);                   //得到结构体大小,用于GetAdaptersInfo参数
    int                 nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);    //填充pIpAdapterInfo指针变量;其中stSize参数既是一个输入量也是一个输出量
    DWORD               netCardNum = 0;                                     //记录网卡数量 
    GetNumberOfInterfaces(&netCardNum);
    cout << "网卡数量：" << netCardNum << endl;
    if (ERROR_BUFFER_OVERFLOW == nRel)
    {
        //如果函数返回的是ERROR_BUFFER_OVERFLOW
        //  则说明GetAdaptersInfo参数传递的内存空间不够,同时其传出stSize,表示需要的空间大小
        //  这也是说明为什么stSize既是一个输入量也是一个输出量
        delete pIpAdapterInfo;                                          //释放原来的内存空间
        pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];            //重新申请内存空间用来存储所有网卡信息
        nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);        //再次调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量
    }
    cout << "--------------------------------------------------------------------" << endl;
    if (ERROR_SUCCESS == nRel)
    {
        //输出网卡信息    可能有多网卡,因此通过循环去判断
        while (pIpAdapterInfo)
        {
            cout << "  序号：" << ++netCardNum << "\t  index：" << pIpAdapterInfo->Index << endl;
            cout << "  名称：" << pIpAdapterInfo->AdapterName << endl;
            cout << "  描述：" << pIpAdapterInfo->Description << endl; //通过有无‘PCI’判断是不是物理网卡 理由不充分
            
            switch (pIpAdapterInfo->Type)
            {
            case MIB_IF_TYPE_OTHER:         cout << "  类型：OTHER " << endl; break;
            case MIB_IF_TYPE_ETHERNET:      cout << "  类型：ETHERNET  " << endl; break;
            case MIB_IF_TYPE_TOKENRING:     cout << "  类型：TOKENRING" << endl; break;
            case MIB_IF_TYPE_FDDI:          cout << "  类型：FDDI      " << endl; break;
            case MIB_IF_TYPE_PPP:           cout << "  类型：PPP       " << endl; break;
            case MIB_IF_TYPE_LOOPBACK:      cout << "  类型：LOOPBACK  " << endl; break;
            case MIB_IF_TYPE_SLIP:          cout << "  类型：SLIP      " << endl; break;
            case 71:                        cout << "  类型：无线网卡  " << endl; break;
            default:                        cout << "  类型：          " << endl; break;
            }

            PIP_ADAPTER_INFO info = pIpAdapterInfo;
            if (                    // 00-05-69 或 00-0C-29 或 00-50-56
                (0x00 == info->Address[0] && 0x05 == info->Address[1] && 0x69 == info->Address[2]) ||
                (0x00 == info->Address[0] && 0x0c == info->Address[1] && 0x29 == info->Address[2]) ||
                (0x00 == info->Address[0] && 0x50 == info->Address[1] && 0x56 == info->Address[2])
                )
            {
                printf("  [VMware 网卡]\n");                              // VMware 网卡 
            }
            
            bool isReal = isRealLocalAapter(pIpAdapterInfo->AdapterName);
            cout << "  是否是本地网卡：" << isReal << endl;
            char mac[24] = { 0 };
            sprintf_s(mac, "%02X-%02X-%02X-%02X-%02X-%02X",
                pIpAdapterInfo->Address[0], pIpAdapterInfo->Address[1], pIpAdapterInfo->Address[2],
                pIpAdapterInfo->Address[3], pIpAdapterInfo->Address[4], pIpAdapterInfo->Address[5]);
            cout << "  网卡MAC地址："<< mac << endl;
            //cout << "-----网卡IP地址如下：" << endl;
            //int                   IPnumPerNetCard = 0;                            //记录每张网卡上的IP地址数量
            //IP_ADDR_STRING *pIpAddrString = &(pIpAdapterInfo->IpAddressList);     //可能网卡有多IP,因此通过循环去判断
            //do {
            //  cout << "该网卡上的IP数量： " << ++IPnumPerNetCard << endl;
            //  cout << "IP 地址：         " << pIpAddrString->IpAddress.String << endl;
            //  cout << "子网掩码：          " << pIpAddrString->IpMask.String << endl;
            //  cout << "网关地址：          " << pIpAdapterInfo->GatewayList.IpAddress.String << endl;
            //  pIpAddrString = pIpAddrString->Next;
            //} while (pIpAddrString);
            pIpAdapterInfo = pIpAdapterInfo->Next;
            cout << "--------------------------------------------------------------------" << endl;
            if (isReal)
                return mac;
        }
    }

    if (pIpAdapterInfo)                                                             //释放内存空间
        delete pIpAdapterInfo;

    return "";
}

string get_if_mac_2_str()
{
    return get_mac();
    return "xxx";
}

#endif
