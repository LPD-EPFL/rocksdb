#!/bin/sh

now=`date`
filename=TestRun_$now.txt
echo "Test results \n" > $filename

for listSize in 1000 10000 50000 100000
do

	for update in 0 1 10 50 100
	do
		for numThreads in 1 2 4 8 16 19 24 32 40
		do
			#./db_test -u$update -d10 -n$numThreads -i1048576 | grep -w 'stopping\|Options' >> $filename
			./db_test -u$update -d10 -n$numThreads -i$listSize | grep -w 'stopping\|Options\|Construct' >> $filename
		done
	done
done

perl parse_results.pl "$filename"
echo "Done!\n"
