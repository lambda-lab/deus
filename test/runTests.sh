#!/bin/sh 

cd $(dirname $(readlink -f $0))

gcc *.c -lcunit -o runner -I../src/
./runner
rm runner