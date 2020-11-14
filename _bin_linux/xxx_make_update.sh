#!/bin/bash

APP_NAME="zzz_app"

:<<note
#共享库本身的文件名，共享库的命名必须是libname.so.x.y.z，这里xyz代表的是此库文件的版本号
#前缀： lib  #后缀： so  #name：自定义的库文件名
#x: Major   Version Number,主版本号，不兼容。重大升级，不同主版本之间的库是不兼容的。
#y: Minor   Version Number,次版本号，向下兼容。增量升级，增加一些新的接口但保留原有接口。高次向后兼容低次。
#z: Release Version Number,发布版本，互相兼容。诸如库的一些错误修改、性能改进等，不会添加新接口，也不会更改接口。主次版本完全相同，不同发行版本完全兼容。

#`echo "vv" | awk '/只在/ { sub(/\< \>/,"-"); print $0 }' `
note

fn_show_cmd()
{
cat <<EOF
----
    ./${0##*/}	 patch  [旧] [新]   #补丁
    ./${0##*/}	 whole  [新]        #全包
    ./${0##*/}	 publish            #发布：ftp
                                           注意:  文件名,目录名不能有空格
----
EOF
}

tmp_log=`mktemp`

#将$1文件内容 插入到$2文件的第3行后
function add_content_src_to_dst_file ()
{
    src_file=$1
    dst_file=$2
	# sed -i "2i\\insert line" file        # -i
	# 2i\\ 拆解3部分：2为行号，i\为sed行下追加命令，\为转义字符(必须转义读取变量)
	#       "" 双引号，保持引号内的字面值，可读\$转义后的变量内容，单引号不行
	tac $src_file | while read -r line
	do
		#echo $line |
		sed -i "6i\\     $line" $dst_file
	done
}

GREEN='\E[1;32m'      # 绿色闪烁警示
BLUE='\E[1;34m'       # 蓝/
RES='\E[0m'           # 清除颜色

function _fun_make_patch_del_cn()
{
	echo -e "${GREEN} del ${RES}"

	#根目录存在删除
	del_files=`diff -rq $1 $2 | grep "只在 $1 "  | sed 's/[[:space:]]//g' | sed 's/存在：/\//g' | sed 's/只在//g'`
	for del_path in $del_files
	do
		echo "  "$del_path && echo "    - $del_path" >> $tmp_log
		mid_file=${del_path#*/}    #
		#cp -f $del_path  $3/$mid_file.del
		touch $3/$mid_file.del
	done

	#子目录存在删除
	del_files=`diff -rq $1 $2 | grep "只在 $1/"  | sed 's/[[:space:]]//g' | sed 's/存在：/\//g' | sed 's/只在//g'`
	for del_path in $del_files
	do
		echo "  "$del_path && echo "    - $del_path" >> $tmp_log
		mid_path=${del_path#$1/}		#去掉$1 获得子路径
		mid_dir=${mid_path%/*}		#获取中间路径  必须创建目录
		if [ $mid_dir ]; then   	mkdir -p $3/$mid_dir;   fi
		touch $3/$mid_path.del
	done
}


function _fun_make_patch_add_cn()
{
	echo -e  "${GREEN} add ${RES}"

	#根目录存在添加
	add_files=`diff -rq $1 $2 | grep "只在 $2 "  | sed 's/[[:space:]]//g' | sed 's/存在：/\//g' | sed 's/只在//g'`
	for add_path in $add_files
	do
		echo "  $add_path" && echo "    + $add_path" >> $tmp_log
		mid_file=${add_path#*/}    #
		cp -rf $add_path  $3/$mid_file
	done

	#子目录存在添加
	add_files=`diff -rq $1 $2 | grep "只在 $2/"  | sed 's/[[:space:]]//g' | sed 's/存在：/\//g' | sed 's/只在//g'`
	for add_path in $add_files
	do
		echo "  $add_path" && echo "    + $add_path" >> $tmp_log
		mid_path=${add_path#$2/}		#去掉$2 获得子路径
		mid_dir=${mid_path%/*}		#获取中间路径  必须创建目录
		if [ $mid_dir ]; then   	mkdir -p $3/$mid_dir;   fi
		cp -rf $add_path  $3/$mid_path
	done
}

function _fun_make_patch_mod_cn()
{
	echo -e  "${GREEN} mod ${RES}"

	# [旧]=>[新]
	mod_files=`diff -rq $1 $2 | grep "文件 " | sed 's/文件 //g' | sed 's/ 和 /\^\^/g' | sed 's/ 不同//g'`
	for mod_path in $mod_files
	do
		new_path=${mod_path#*^^}     #去掉^^ 获得新的子路径
		new_dir=${new_path%/*}       #获取所在目录
		new_mid=${new_dir#$2/}       #新的中间路径
		mod_one=${new_path#*/}
		echo "  $1^$2...$mod_one" && echo "    * $1^$2...$mod_one" >> $tmp_log
		if [ $2 = $new_dir ]; then
			cp $new_path  $3/  
		else
			mkdir -p $3/$new_mid;  cp $new_path $3/$new_mid 
		fi
	done
}

function _fun_make_patch_del_en()
{
	echo -e "${GREEN} del ${RES}"

	#根目录存在删除
	del_files=`diff -rq $1 $2 | grep "Only in $1: " | sed 's/Only in //g' | sed 's/[[:space:]]//g' | sed 's/:/\//g'`
	for del_path in $del_files
	do
		echo "  "$del_path && echo "    - $del_path" >> $tmp_log
		mid_file=${del_path#*/}    #
		cp -f $del_path  $3/$mid_file.del
		touch $3/$mid_file.del
	done

	#子目录存在删除
	del_files=`diff -rq $1 $2 | grep "Only in $1/" | sed 's/Only in //g' | sed 's/[[:space:]]//g' | sed 's/:/\//g'`
	for del_path in $del_files
	do
		echo "  "$del_path && echo "    - $del_path" >> $tmp_log
		mid_path=${del_path#$1/}		#去掉$1 获得子路径
		mid_dir=${mid_path%/*}		    #获取中间路径  必须创建目录
		if [ $mid_dir ]; then   	mkdir -p $3/$mid_dir;   fi
		touch $3/$mid_path.del
	done
}


function _fun_make_patch_add_en()
{
	echo -e  "${GREEN} add ${RES}"

	#根目录存在添加
	add_files=`diff -rq $1 $2 | grep "Only in $2: " | sed 's/Only in //g' | sed 's/[[:space:]]//g' | sed 's/:/\//g' | sed 's/$2\///g' `
	for add_path in $add_files
	do
		echo "  $add_path" && echo "    + $add_path" >> $tmp_log
		mid_file=${add_path#*/}    #
		cp -rf $add_path  $3/$mid_file
	done

	#子目录存在添加
	add_files=`diff -rq $1 $2 | grep "Only in $2/" | sed 's/Only in //g' | sed 's/[[:space:]]//g' | sed 's/:/\//g' | sed 's/$2\///g' `
	for add_path in $add_files
	do
		echo "  $add_path" && echo "    + $add_path" >> $tmp_log
		mid_path=${add_path#$2/}		#去掉$2 获得子路径
		mid_dir=${mid_path%/*}		    #获取中间路径  必须创建目录
		if [ $mid_dir ]; then    mkdir -p $3/$mid_dir;   fi
		cp -rf $add_path  $3/$mid_path
	done
}

function _fun_make_patch_mod_en()
{
	echo -e  "${GREEN} mod ${RES}"

	# [旧]=>[新]
	mod_files=`diff -rq $1 $2 | grep "Files" | sed 's/Files//g' | sed 's/ and /\^\^/g' | sed 's/ differ//g'`
	for mod_path in $mod_files
	do
		new_path=${mod_path#*^^}     #去掉^^ 获得新的子路径
		new_dir=${new_path%/*}       #获取所在目录
		new_mid=${new_dir#$2/}       #新的中间路径
		mod_one=${new_path#*/}
		echo "  $1^$2...$mod_one" && echo "    * $1^$2...$mod_one" >> $tmp_log
		if [ $2 = $new_dir ]; then
			cp $new_path  $3/  
		else
			mkdir -p $3/$new_mid;  cp $new_path $3/$new_mid 
		fi
	done
}

function _patch_info_log()
{
	echo " ignore _patch_/ver/x_apply_patch.log"
	rm -rf _patch_/ver/x_apply_patch.log    #删除日志，日志为更新依据
	have_diff=`diff -rq _patch_ ._patch_`	#是否与上次增量一样
	
	if [ -z "$have_diff" ];then
		echo -e "${BLUE}\n无效打包:  增量与上版一致 \n${RES}"
		rm -rf _patch_
	else
		patch_name_tgz=${APP_NAME}__$1_v_$2__`date +%y%m%d_%H%M%S`.tgz
		tar -zcf _patch_.tgz _patch_
		mv _patch_.tgz $patch_name_tgz
		md5sum_pack=`md5sum $patch_name_tgz | awk '{print $1}' `
		sed -i "1i \ "  $tmp_log && vim  $tmp_log
		sed -i "2c 新增量:   $patch_name_tgz"   x_patch_update.log
		sed -i "3c md5sum:   $md5sum_pack"  x_patch_update.log
		sed -i "3a\----------------------"  x_patch_update.log
		sed -i "4a\md5sum: $md5sum_pack _patch_: $patch_name_tgz"   x_patch_update.log
		add_content_src_to_dst_file $tmp_log x_patch_update.log
		rm -rf ._patch_ && cp -rf _patch_ ._patch_ && rm -rf _patch_			#保存上次增量  防止打包重复
		echo -e "${GREEN}\n增量打包完成        \n${RES}"
	fi
	rm -rf $tmp_log
	 #计算tar包的hash 不能真正防止重复
}

#制作增量升级                                 $1 旧版   $2 新版   $3 增包名称
function fun_make_patch()
{
	echo -e "${GREEN}\n检索增量文件..  $1 => $2   \n${RES}"
	mkdir -p ._patch_ _patch_				#之前备份, 临时存储

	diff_use_lang=`diff . .. | head -n 1 | awk '{print $1}'`
	if [ 'Only' = $diff_use_lang ]
	then
	    _fun_make_patch_del_en $1 $2  _patch_
	    _fun_make_patch_add_en $1 $2  _patch_
	    _fun_make_patch_mod_en $1 $2  _patch_
	else
        _fun_make_patch_del_cn $1 $2  _patch_
	    _fun_make_patch_add_cn $1 $2  _patch_
	    _fun_make_patch_mod_cn $1 $2  _patch_
	fi

	_patch_info_log $1 $2
}

function fun_make_whole()
{
	# WSL 中 mv竟然没有权限
	#mv $1 ${APP_NAME}
	#tar -zcf $APP_NAME.tgz $APP_NAME
	#mv $APP_NAME $1
	#mv $APP_NAME.tgz ${APP_NAME}__$1__`date +%y%m%d_%H%M%S`.tgz
	#echo -e "${GREEN}\n 全量打包   $1  \n${RES}"

	cp -rf $1 ${APP_NAME}
	tar -zcf $APP_NAME.tgz $APP_NAME
	rm -rf $APP_NAME
	cp  $APP_NAME.tgz ${APP_NAME}__$1__`date +%y%m%d_%H%M%S`.tgz
	rm $APP_NAME.tgz
	echo -e "${GREEN}\n 全量打包   $1  \n${RES}"
}

function fun_publish()
{
	#cp -rf x_patch_update.log  /var/ftp_vir_usr/HrW/dn/
	source ./xxx_auto_ftp2.sh up_file   .  x_patch_update.log  HrW/dn
	source ./xxx_auto_ftp2.sh dn_file   HrW/dn  xzt.txt  .
}


case "$1" in
	patch)   fun_make_patch $2 $3	  ;;
	whole)   fun_make_whole $2		 ;;
	publish) fun_publish		    ;;
	"")      fn_show_cmd		    ;;
	*)       fn_show_cmd	       ;;
esac


