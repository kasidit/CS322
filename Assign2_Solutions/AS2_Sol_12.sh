#!/bin/bash

CNT=1

read -p "Please enter the number of counts: " MAXCNT

while [ "$CNT" -lt "$MAXCNT" ]; do
  echo "Count $CNT"
  sleep 1
  CNT=$((CNT+1))
done
