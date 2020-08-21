#!/bin/sh
source ../sh.d/auto_ftp2.sh


fn_app_bak()
{
	mkdir -p ../bak
	APP="z_plg"
	VER=`cat ../VERSION`
	DATE=`date +%m%d`
	echo "backup app:$APP ver:$VER $DATE"
	tar -zcvf ../bak/${APP}_${VER}_${DATE}_bak.tgz \
		{../cfg,../log,../plg,../util.sh,../z_app,../zz_plg.sh}
}


# 主要是遍历目录，删除“del”标记文件
function _apply_patch_detail()
{ 
	for file in `ls $1` 
	do
		file_path=$1"/"$file
		del=${file_path##*.}
		#echo $1"/"$file            $del
		if [ "del" = "$del" ]
		then
			del_obj=${file_path%.*}		#echo " del_obj="$del_obj
			rm -rf $del_obj $del_obj.del
		else
			if [ -d $file_path ] 
  			then
				echo "  --"${file_path##*/}
				if [ "_patch_" = ${file_path##*/} ]
				then
					#echo "--------_patch_--------"
					return
				fi
   				_apply_patch_detail $file_path 
  			fi
		fi
	done 
} 

#具体升级细节 解压patch包  执行安装
apply_patch_detail()
{
	path_name=$1
	rm -rf _patch_
	tar -zxf $path_name
	cp -pdrf _patch_/* ../
	_apply_patch_detail ..
	rm -rf _patch_
	ver_raw=${path_name#*_v_}
	ver_now=${ver_raw%__*}
	sed -i "2c \当前版本:   $ver_now"        x_apply_patch.log
    sed -i "4i \\`date +%y%m%d_%H%M%S`  $1"  x_apply_patch.log	
}

apply_patch()
{
    fn_ftp_dn_file  HrW/dn  x_patch_update.log  .
	ver_cur=`awk -F ' ' 'NR==2{print $2}' x_apply_patch.log`
	ver_new0=`awk -F ' ' 'NR==2{print $2}' x_patch_update.log`
	ver_new1=${ver_new0#*_v_}
	ver_new=${ver_new1%__*}

	ver_big="$(echo $ver_cur $ver_new | tr " " "\n" | sort -rV | head -n 1)"; 
	
    if [ $ver_cur = $ver_big ]; then
        echo 无需升级
    else		
        patch__need=`tac x_patch_update.log | grep "_patch_" | grep "${ver_cur}_v_" | awk 'NR==1{print $4}'`
		md5sum_need=`tac x_patch_update.log | grep "_patch_" | grep "${ver_cur}_v_" | awk 'NR==1{print $2}'`
        [ ! -f $patch__need ] && fn_ftp_dn_file HrW/dn  $patch__need  .
        md5sum_down=`md5sum $patch__need | awk '{print $1}'`
        if [ $md5sum_need != $md5sum_down ]; then
            echo 校验md5失败 $md5sum_need $md5sum_down
        else
            apply_patch_detail $patch__need
        fi
    fi
}

apply_patch
