#!/bin/bash

CNT=1

while [ "$CNT" -lt "${1}" ]; do
  echo "Count $CNT"
  sleep 1
  CNT=$((CNT+1))
done
