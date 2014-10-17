#自动编译扩展脚本  lijianwei
#!/bin/sh
#平滑关闭
kill -QUIT `cat /usr/local/debug_php/var/run/php-fpm.pid`
#启动
/usr/local/debug_php/sbin/php-fpm

