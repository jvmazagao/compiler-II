#!/bin/sh

echo lamp compiler test suite
echo ========================

for i in *.lamp ; do
  /usr/bin/printf "%s... \n" $i
  ../main $i 
 
  # if [ $? -eq 0 ] ; then
  #   echo ok
  # else
  #   echo fail
  # fi
done