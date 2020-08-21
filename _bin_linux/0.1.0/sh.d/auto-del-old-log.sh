#!/bin/bash

find /opt/zzz_app_dir/log -mtime +30 -name "*.log" -exec rm -rf {} \;

#将.sh执行脚本加入到系统计划任务，到点自动执行
#crontab -e
#输入：
# 10 0 * * * /opt/soft/log/auto-del-7-days-ago-log.sh >/dev/null 2>&1
