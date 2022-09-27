#!/bin/bash

trap 'echo 10 intercepted FIN; break' 10

CNT=1

while true
do 
  echo "Count $CNT"
  sleep 1
  CNT=$((CNT+1))
done
