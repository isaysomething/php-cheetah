#!/bin/bash
sudo /usr/local/php/bin/phpize --clean
echo 'Clean Complete'
/usr/local/php/bin/phpize
./configure --with-php-config=/usr/local/php/bin/php-config
sudo make install
echo "please add 'extension=cheetah.so;' into your php.ini"
sudo /usr/local/php/bin/phpize --clean
processes=`ps -e | grep php-fpm | awk '{print $1}'`
if [ "$processes" = "" ]
then
    echo 'empty'
else
    echo '正在终止PHP7进程......'
sudo kill -9 $processes
fi
echo '正在启动PHP7......'
sudo php-fpm
echo '完成安装'
#sudo nm -A /opt/php/php7/lib/php/extensions/debug-zts-20141001/cheetah.so
