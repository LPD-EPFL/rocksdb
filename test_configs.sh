#!/bin/sh

now=`date`
filename=TestRun_$now.txt
echo "Test results \n" > $filename

for update in 0 10 50 100
do
	for numThreads in 1 2 4 8 16 19
	do
		#./db_test -u$update -d10 -n$numThreads -i1048576 | grep -w 'stopping\|Options' >> $filename
		./db_test -u$update -d10 -n$numThreads -i1024 | grep -w 'stopping\|Options\|Construct' >> $filename
	done
done

perl parse_results.pl "$filename"
echo "Done!\n"
