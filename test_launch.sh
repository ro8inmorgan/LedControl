#!/bin/sh
echo $0 $*
cd $(dirname "$0")

./test > test.log 2>&1
