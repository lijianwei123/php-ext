#自动编译扩展脚本  lijianwei
#!/bin/sh
/usr/local/php/bin/phpize
./configure --with-php-config=/usr/local/php/bin/php-config
make
make install


