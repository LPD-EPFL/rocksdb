#!/bin/sh

now=`date`
filename=TestRun_$now.txt
echo "Test results \n" > $filename

for numThreads in 1 2 # 4 8 16 20
do
	for update in 0 #10 50 100
	do
		./db_test -u$update -d1 -n$numThreads -i1024 | grep -w 'stopping\|Options' >> $filename
	done
done