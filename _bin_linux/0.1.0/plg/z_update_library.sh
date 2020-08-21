#!/bin/bash

function version_gt() { test "$(echo "$@" | tr " " "\n" | sort -V | head -n 1)" != "$1"; }

:<<note
function dn_version_gt()     #比较 $1 $2 版本号大小
{
if version_gt $1 $V2; then
	echo "$1 > $2"
else
	echo "$1 <= $2"
fi
}
note

#定义颜色变量
GREEN='\E[1;32m'    	# 绿
BLUE='\E[1;34m'       # 蓝/
SHAN='\E[33;5m'      # 绿色闪烁警示
RES='\E[0m'           # 清除颜色
#用echo -e来调用

fn_update_library()
{
	ALL_LNK=`ls *.so`
	for var_lnk in $ALL_LNK
	do
		if [ -f ${var_lnk}.del ]; then
			echo -e  "${GREEN}   $var_lnk  delete ...        ${RES}"
           find ${var_lnk}* | xargs rm
			continue 1
		fi

		linkpath=`readlink -f $var_lnk`
		realname=`echo ${linkpath##*/}`

		libs=`ls $var_lnk.*`
       ver_new="$(echo "$libs" | tr " " "\n" | sort -rV | head -n 1)"; 
               
		if version_gt $ver_new $realname; then
			find $var_lnk* | grep -v $ver_new | xargs rm
			echo -e  "${GREEN}   $ver_new => $var_lnk        ${RES}"
			ln   -sf $ver_new $var_lnk 
		else
			echo -e  "${BLUE}   $realname is lastest_version ${RES}"
		fi

		#echo "  ****"  $libs 
		#echo "  ===="  $realname "::" ${realname%%.so.*} "--" ${realname##*.so.} 
		#echo "  ----"  $ver_new 

	done
}

fn_update_library

#共享库本身的文件名，共享库的命名必须是libname.so.x.y.z，这里xyz代表的是此库文件的版本号
#前缀： lib
#后缀： so
#name：自定义的库文件名
#x: Major Version Number，主版本号，不兼容。重大升级，不同主版本之间的库是不兼容的。
#y: Minor Version Number, 次版本号，向下兼容。增量升级，增加一些新的接口但保留原有接口。高次版本的库向后兼容低次版本的库。
#z: Release Version Number，发布版本号，互相兼容。诸如库的一些错误修改、性能改进等，不会添加新接口，也不会更改接口。主版本和次版本完全相同的情况下，不同发行版本之间完全兼容。

#echo  | cut -f 1  -d '.'
