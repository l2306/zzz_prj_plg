#!/bin/bash

ZZZ_APP=zzz_app
VERSION=0.1.0

#定义颜色变量
GREEN='\E[1;32m'
BLUE='\E[1;34m'
SHAN='\E[33;5m'      # 绿色闪烁警示
OVER='\E[0m'          # 清除颜色
#用echo -e来调用

#变更版本,若无则拷贝最近版本,并在此基础上修改
if [ -d _bin_linux/$VERSION ]; then
    echo  -e "${BLUE} $ZZZ_APP $VERSION 已存在 ${OVER}" 
else
    ver_all=`ls _bin_linux -F | grep '/$' | sed 's/\///g'`
    ver_last="$(echo "$ver_all" | sort -rV | head -n 1)"; 
    echo  -e "${GREEN}  last_ver = $ver_last  \n{\n$ver_all\n}     ${OVER}" 
    cp -rf  _bin_linux/$ver_last  _bin_linux/$VERSION
fi

dir_need_build=(. plg_0 plg_1st plg_2nd plugin_one plugin_two)
make_some()
{
    mkdir -p ${TMP_OBJ_DIR}
    if [ $SUB != 'all' ] ; 
    then
        echo " "  &&  make $clean  -C $SUB
    else 
        for i in ${dir_need_build[@]}; do
            echo " "  &&   make $clean  -C $i
        done
    fi
}

choose()
{
case $# in
    0)   echo -e "${SHAN}     input:   all/centos/ubuntu            ${OVER}" && exit 1 ;;
    1)   echo -e "${SHAN}     input:   all/ ${dir_need_build[@]}    ${OVER}" && exit 1 ;;
    2)   echo -e "${SHAN}     input:   d/r/dc/rc	            ${OVER}" && exit 1 ;;
    *)   echo "" ;;
esac
    export PLT=$1   &&  echo " 操作系统    $PLT "
    export SUB=$2   &&  echo " 构建子项    $SUB " 
    export D_R=$3   &&  echo "内测or发版   $D_R " 
}

choose $*

# 构造make环境，并进行make
out=`pwd`/_bin_linux
case ${D_R} in
    d)   export TMP_OBJ_DIR=$out/obj_d  &&  export dbg_or_not=-g  export BUILD_TODIR=$out/dbg       && make_some $SUB ;;
    r)   export TMP_OBJ_DIR=$out/obj_r  &&  export dbg_or_not=    export BUILD_TODIR=$out/$VERSION  && make_some $SUB ;;  
    dc)  export TMP_OBJ_DIR=$out/obj_d  &&  export clean=clean    export BUILD_TODIR=$out/dbg       && make_some $SUB ;;
    rc)  export TMP_OBJ_DIR=$out/obj_r  &&  export clean=clean    export BUILD_TODIR=$out/$VERSION  && make_some $SUB ;;
    *)   echo "  ${D_R}  --内测or发版-- d/r/cd/cr  invalid make_cmd"    ;;
esac
