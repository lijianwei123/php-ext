#自动编译扩展脚本  lijianwei
#!/bin/sh
/home/php/bin/phpize
./configure --with-php-config=/home/php/bin/php-config
make clean
make
make install


