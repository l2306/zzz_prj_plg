#!/bin/sh

# https://www.jb51.net/article/34005.htm
#./exe_scp.sh ./exe_scp_host_addr.list /home/simba/x.sh /home/simba/x

list_file=$1
src_file=$2
dst_file=$3
cat $list_file | while read line
do
	hostaddr=`echo $line | awk '{print $1}'`
	username=`echo $line | awk '{print $2}'`
	password=`echo $line | awk '{print $3}'`
	echo "$host_ip"
	./exe_scp_expect.sh $src_file $username@$hostaddr:$dst_file $password
done 

#
#1.安装依赖
#    yum install -y tcl tclx tcl-devel

#2.下载expect-5.43.tgz包（也可用别的）
#	根据参数，运行./configure
#	./configure --with-tcl=/usr/lib --with-tclinclude=/usr/include/tcl-private/generic
	
#3.make && make install  安装完毕
