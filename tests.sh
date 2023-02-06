#!/bin/bash
for i in {0..11}; do
	echo "file: test$i.csv"
	./read tests/test$i.csv 
	echo " "
done
