#!/bin/sh 

# add here list of directories under ./src that should be linked to tests 
LINK_SOURCE_CODE_DIRECTORIES=""


cd $(dirname $(readlink -f $0))

SOURCE_FILES=""
for directory in $LINK_SOURCE_CODE_DIRECTORIES; do 
    SOURCE_FILES="$SOURCE_FILES ../src/$directory/*.c"
done

gcc $SOURCE_FILES *.c -lcunit -o runner -I../src/
./runner
rm runner