
[simba@kingdom plg (复件)]$ ll
总用量 668
lrwxrwxrwx. 1 simba simba    21 7月  25 02:36 plugin_one.so -> plugin_one.so.0.234.1
-rw-rw-r--. 1 simba simba 76592 5月  20 22:43 plugin_one.so.0.1.1
-rw-rw-r--. 1 simba simba 76592 5月  20 22:43 plugin_one.so.0.123.1
-rw-rw-r--. 1 simba simba 76592 5月  20 22:43 plugin_one.so.0.234.1
lrwxrwxrwx. 1 simba simba    18 7月  25 02:36 plugin_two2.so -> plugin_two2.so.0.2
-rw-rw-r--. 1 simba simba 73234 7月  25 00:21 plugin_two2.so.0.2
lrwxrwxrwx. 1 simba simba    16 7月  25 02:36 plugin_two3.so -> plugin_two3.so.1
-rw-rw-r--. 1 simba simba 73234 7月  25 02:34 plugin_two3.so.1
-rw-rw-r--. 1 simba simba 73234 7月  25 02:38 plugin_two3.so.2
lrwxrwxrwx. 1 simba simba    16 7月  25 02:36 plugin_two4.so -> plugin_two4.so.1
-rw-rw-r--. 1 simba simba 73234 7月  25 02:34 plugin_two4.so.1
-rw-rw-r--. 1 simba simba 73234 7月  25 02:37 plugin_two4.so.del
lrwxrwxrwx. 1 simba simba    15 7月  25 02:36 plugin_two.so -> plugin_two.so.1
-rw-rw-r--. 1 simba simba 73234 7月  25 02:29 plugin_two.so.1
-rwxrw-r--. 1 simba simba  2109 7月   3 12:22 z_incremental_update.sh
-rwxrw-r--. 1 simba simba  2001 7月  25 02:25 z_update_library.sh
[simba@kingdom plg (复件)]$ 
[simba@kingdom plg (复件)]$ ./z_update_library.sh 
   plugin_one.so.0.234.1 is lastest_version 
   plugin_two2.so.0.2 is lastest_version 
   plugin_two3.so.2 => plugin_two3.so        
   plugin_two4.so  delete ...        
   plugin_two.so.1 is lastest_version 
[simba@kingdom plg (复件)]$ ll
总用量 452
lrwxrwxrwx. 1 simba simba    21 7月  25 02:36 plugin_one.so -> plugin_one.so.0.234.1
-rw-rw-r--. 1 simba simba 76592 5月  20 22:43 plugin_one.so.0.1.1
-rw-rw-r--. 1 simba simba 76592 5月  20 22:43 plugin_one.so.0.123.1
-rw-rw-r--. 1 simba simba 76592 5月  20 22:43 plugin_one.so.0.234.1
lrwxrwxrwx. 1 simba simba    18 7月  25 02:36 plugin_two2.so -> plugin_two2.so.0.2
-rw-rw-r--. 1 simba simba 73234 7月  25 00:21 plugin_two2.so.0.2
lrwxrwxrwx. 1 simba simba    16 7月  25 02:40 plugin_two3.so -> plugin_two3.so.2
-rw-rw-r--. 1 simba simba 73234 7月  25 02:38 plugin_two3.so.2
lrwxrwxrwx. 1 simba simba    15 7月  25 02:36 plugin_two.so -> plugin_two.so.1
-rw-rw-r--. 1 simba simba 73234 7月  25 02:29 plugin_two.so.1
-rwxrw-r--. 1 simba simba  1223 7月  25 02:39 z_incremental_update.sh
-rwxrw-r--. 1 simba simba  2001 7月  25 02:25 z_update_library.sh
[simba@kingdom plg (复件)]$ 

