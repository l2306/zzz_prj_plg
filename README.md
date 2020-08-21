

# 插件框架(不喜勿喷)

## 分别有两种实现方式：
   * 1，采用使用消息总线的方式，plugin*目录  优点:减少彼此依赖，缺点:不能相互调用，需要构造消息
   * 2，采用插件库交叉接口方式，plg*目录     优点:方便交叉调用，缺点:可能耦合较强，

## 基本说明：
    _bin_linux      程序window生成目录
    _exe_win        程序window生成目录
    0utils          工具库
    3rd             三方库
    plg             插件基础库
    plg_*           具体插件实现
    plugin          插件基础库
    plugin_*        具体插件实现

## 注意：  window，linux中不同处
    *win 特有的   .vcxproj .vcxproj.filters .sln .v12.suo            
    *linux 则是   makefile mk.sh                                              
