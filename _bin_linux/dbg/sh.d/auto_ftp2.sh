#!/bin/bash


fn_cmd_show()
{
cat <<EOF
----
    ./${0##*/}	 up_dir    [本地]          [远端]  
    ./${0##*/}	 dn_dir    [远端]          [本地] 
    ./${0##*/}	 up_file   [本地]  [文件]  [远端] 
    ./${0##*/}	 dn_file   [远端]  [文件]  [远端] 
----
EOF
}


addr="47.95.114.239"
user="HrW"			#必须定义ftp用户  用于控制权限
pswd="1234567"

#	1. -n 不受.netrc文件的影响。（ftp默认为读取.netrc文件中的设定）
#	2. << 是使用即时文件重定向输入。
#	3. !是即时文件的标志它必须成对出现，以标识即时文件的开始和结尾。

#一，FTP要安装，SFTP不要安装
#二；SFTP更安全，效率比FTP低

#passive 被动模式     active 主动模式
#批量下载 
#远端目录srvr_src -->> 本地目录clnt_dst
fn_ftp_dn_dir(){
	srvr_src=$1
	clnt_dst=$2
ftp -n<<!
	open $addr
	user $user $pswd
	binary
	cd $srvr_src
	lcd $clnt_dst
	prompt
	mget *
	close
	bye
!
}

#批量上传
#本地目录clnt_src -->> 远端目录srvr_dst
fn_ftp_up_dir(){
	clnt_src=$1
	srvr_dst=$2
ftp -n<<!
	open $addr
	user $user $pswd
	binary
	hash
	cd $srvr_dst
	lcd $clnt_src
	prompt
	mput *
	close
	bye
!
}

#下载单个
#远端目录srvr_src -->> 本地目录clnt_dst
fn_ftp_dn_file(){
	srvr_src=$1
	file_tgt=$2
	clnt_dst=$3
ftp -n<<!
	open $addr
	user $user $pswd
	prompt
	binary
	hash
	cd $srvr_src
	lcd $clnt_dst
	mget $file_path $file_tgt
	close
	bye
!
}

#单个上传	
#本地目录clnt_src -->> 远端目录srvr_dst
fn_ftp_up_file(){
	clnt_src=$1
	file_tgt=$2
	srvr_dst=$3
ftp -n<<!
	open $addr
	user $user $pswd
	prompt
	binary
	cd $srvr_dst
	lcd $clnt_src
	put $file_tgt $file_tgt
	close
	bye
!
}

case "$1" in
	up_file)   fn_ftp_up_file $2 $3 $4	  ;;
	dn_file)   fn_ftp_dn_file $2 $3 $4    ;;
	up_dir)    fn_ftp_up_dir  $2    $3	  ;;
	dn_dir)    fn_ftp_dn_dir  $2    $3    ;;
	"")        fn_cmd_show		          ;;
	*)         fn_cmd_show	              ;;
esac


#			 拷贝目标			拷贝的目的地
#scp		本地文件			username@servername:远程目录
#scp -r	本地目录			username@servername:远程目录
#scp		username@servername:远程文件		本地目录
#scp -r 	username@servername:远程目录		本地目录

#sftp登陆远程服务器
#sftp username@ip
#	例如：sftp mqadmin@10.10.1.150		然后输入password即可
#
#put：上传文件
#	例如：put iostat-dd.log  	将iostat-dd.log上传到对端服务器
#get：下载文件
#	例如：get iostat-dd.log	将远端文件下载到本地
#
#help：帮助命令
#
#pwd/lpwd：
#	pwd查看远端服务器目录；lpwd查看本端目录。
#
#ls/lls：
#	ls列出远端目录文件；lls列出本端文件
#
#!command：!+命令表示在本段运行命令
#	例如：!ls列出本端文件；!pwd显示本端路径
#
#exit/quit：退出

