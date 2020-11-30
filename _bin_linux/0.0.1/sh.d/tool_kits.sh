#!/bin/sh

# sed -i "s/原字符串/新字符串/g" `grep 原字符串 -rl 所在目录`
# [ 0 = 0 ] && echo "yes"|| echo "not"

# 日志文件 名称格式：  名字_数字.log
fn_log_expire()
{
    log_max_num=$1
    log_dir=$2
    for file in `ls *.log |sort -g -r`
    do
        file_name=$(basename "$file")
        extension=${file_name##*.}
        file_name=${file_name%.*}
        numb_part=${file_name##*_}
        base_name="${file_name%%_*}_" 
        # echo "$file_name.$extension $base_name $numb_part"
        [ $numb_part -le $log_max_num ] && mv "$file" "$base_name$(($numb_part +1))".$extension
    done
}