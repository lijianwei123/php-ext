#自动编译扩展脚本  lijianwei
#!/bin/sh
/usr/local/debug_php/bin/phpize
./configure --with-php-config=/usr/local/debug_php/bin/php-config
#make clean
make
make install


