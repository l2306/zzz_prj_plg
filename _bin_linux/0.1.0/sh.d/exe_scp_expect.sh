#!/usr/bin/expect

#	(需安装expect)	 用户  地址  密码
#./_expect_scp.sh simba@localhost:/home/simba/x.sh /home/simba/x "1234567"
#./_expect_scp.sh /home/simba/x.sh simba@localhost:/home/simba/x "1234567"

set timeout 10
set src_file [lindex $argv 0]
set dst_file [lindex $argv 1]
set password [lindex $argv 2]
spawn scp -rf $src_file $dst_file
expect {
	"(yes/no)?"
	{
		send "yes\n"
		expect "*password:" { send "$password\n"}
	}
	"*password:"
	{
		send "$password\n"
	}
}
expect "100%"
expect eof

