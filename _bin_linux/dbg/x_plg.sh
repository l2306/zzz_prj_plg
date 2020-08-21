#!/bin/bash

APP="zzz_app"
AppName="zzz_app_plg"

fn_show_cmd()
{
cat <<EOF
----
	./${0##*/}	info	运行信息
	./${0##*/}	stop	停用程序
	./${0##*/}	start	启动程序
	./${0##*/}	restart	重启程序
	./${0##*/}	install	安装软件
	./${0##*/}	uninstall	卸载软件
	./${0##*/}	applypatch	增量升级
	./${0##*/}	pack 	打包程序(版本号)
----
EOF
#	echo "	./$APP.sh deamon &		运行信息"
}
fn_app_start()
{
	CUR_USR=`whoami`
	PID=`ps -u $CUR_USR | grep $APP | grep -v "grep" | awk {'print $1'}`
	if [ $PID ]
	then
		echo "proc already started!"
		echo "	usr:$CUR_USR  app:$APP  pid:$PID		already started!"
	else
		echo "	app not exist!"
		nohup ./$APP >./log/std_out2 2>&1 &
		echo "	app has started!"
	fi
}
fn_app_stop()
{
	CUR_USR=`whoami`
	PID=`ps -u $CUR_USR | grep $APP | grep -v "grep" | awk {'print $1'}`

	if [ $PID ]
	then
		echo "	usr:$CUR_USR  app:$APP  pid:$PID		app is killed" 
		kill -9 $PID
	else
		echo "	$APP:$PID is not running"
   		exit 1
	fi
}

fn_app_info()
{
	VER=`cat .VERSION`
	PID=`ps -u | grep $APP | grep -v "grep" | awk {'print $2'}`

	if [ $PID ]
	then
		echo "	app:$APP  ver:$VER  pid:$PID " 
		ls -l /proc/$PID | grep cwd
   		exit 1
	fi
	#过滤得到 程序所在路径	
	#	目的: 可能程序未定 安装目录 多个程序在不同位置导致不能确定程序位置
}

fn_app_install()
{
	if [ `whoami` != "root" ]
	then
    	echo "必须使用root用户执行本安装程序！"
    	exit -1
	fi
	mkdir -p /opt/$AppName
	cp -rf * /opt/$AppName
	
	#配置动态库目录 使其生效	添加删除都比较方便
	cp cfg/zzz_app_lib_dir.conf /etc/ld.so.conf.d
	ldconfig	
	#low的方法	ldconfig `pwd`//
#		/etc/rc.d/init.d
}
fn_app_uninstall()
{
	if [ `whoami` != "root" ]
	then
    	echo "必须使用root用户执行本安装程序！"
    	exit -1
	fi
	rm -rf /opt/$AppName

	rm -f /etc/ld.so.conf.d/zzz_app_lib_dir.conf
	ldconfig	
}

fn_app_delbak()
{
	find bak/ -name "*bak.tgz" -ok rm -rf {} \;
}

#	循环监控zip文件变换
fn_loop()
{
	while true;	
	do
		find /opt/soft/log/ -mtime +30 -name "*.log" -exec rm -rf {} \;
		ps -ef | grep "a.out" | grep -v "grep"
		if [ "$?" -eq 1 ]
		then
			echo "app not exist!"
			echo "./a.out &"
			nohup ./a.out >./log/std_out2 2>&1 &
			echo "app has been restarted!"
		else
			echo "app already started!"
		fi
		sleep 10
	done
}

fn_app_pack()
{
	VER=`cat .VERSION`
	if [ -z $1 ]
	then
		echo "	沿用之前版本 $VER"
		tar -zcvf app_${APP}_pkg_${VER}.tgz  ../_app  
	else
		echo $1>.VERSION
		tar -zcvf app_${APP}_pkg_$1.tgz  ../_app  
	fi
}

whiptail --title " Msg Box " --msgbox " 欢迎使用本软件  " 10 30

case "$1" in
	info)
			fn_app_info		;;
	stop)
			fn_app_stop		;;
	start)
			fn_app_start		;;
	delbak)
			fn_app_delbak		;;
	install)
			fn_app_install		;;
	uninstall)
			fn_app_uninstall	;;
	pack)
			fn_app_pack $2		;;
	"")
			fn_show_cmd		;;
	*)
			echo "	invaild cmd "	;;
esac



