#!/usr/local/bin/perl

use POSIX qw(strftime);
$datestring = strftime "%b_%e_%H:%M", localtime;

my $input_file = $ARGV[0];
my $output_file = "results_$datestring.dat";

my $duration = -1;
my $num_threads = -1;
my $update_percent = -1;
my $list_size = -1;

my $total_ops = 0;
my $total_writes = 0;
my $total_deletes = 0;
my $total_gets = 0;

my $avg_ops = 0;
my $avg_writes = 0;
my $avg_deletes = 0;
my $avg_gets = 0;

my $total_flush_to_disk_time = 0;
my $total_string_creation_time = 0;
my $backup_count = 0;

my $write_latency_micros = 0;
my $read_latency_micros = 0;


my $first_pass = 1;

open (INPUT_FILE, "<$input_file")  || die "Can't open $input_file: $!\n";
open (my $out, '>', $output_file) or die "Could not open file '$output_file' $!";

while (<INPUT_FILE>) {

	if (/^Options Updates ([0-9]*) Duration\(s\) ([0-9]*) NumThreads ([0-9]*) NumKeys ([0-9]*)/){
		#print old stats
		if ($first_pass == 0){
			$avg_ops = int($total_ops/($num_threads * $duration));
						
			$avg_writes = int($total_writes/($num_threads * $duration));
			$avg_deletes = int($total_deletes/($num_threads * $duration));
			$avg_gets = int($total_gets/($num_threads * $duration));

			if ($backup_count != 0) {	
				$avg_write_to_disk = int ($total_flush_to_disk_time/$backup_count);
				$avg_string_creation = int ($total_string_creation_time/$backup_count);
			} else{
				$avg_write_to_disk = 0;
				$avg_string_creation = 0;
			}

			print $out "$num_threads\t\t$avg_ops\t\t$avg_writes\t\t$avg_deletes\t\t$avg_gets\t\t$avg_string_creation\t\t$avg_write_to_disk\t\t$backup_count\t\t$read_latency_micros\t\t$write_latency_micros\n";

			print "Average vals ops: $avg_ops, writes: $avg_writes, deletes: $avg_deletes, gets: $avg_gets, bg_str_creation: $avg_string_creation, bg_flush_to_disk: $avg_write_to_disk, backup count: $backup_count \n";
		} else {
			$first_pass = 0;
		}	


		if ($update_percent != $1 || $list_size != $4){
			$update_percent = $1;
			$list_size = $4;

			print $out "\n\n";
			print $out "## -i$list_size -u$update_percent\n";
			print $out "#  cores\tops/s\twrites/s\tdeletes/s\tgets/s\tbg_str_micros\tbg_flush_micros\tget_op_micros\twrite_op_micros\n";
		}

		$duration = $2;
		$num_threads = $3;
		print "$_";

		$total_ops = 0;
		$total_writes = 0;
		$total_deletes = 0;
		$total_gets = 0;
		
		$total_flush_to_disk_time = 0;
		$total_string_creation_time = 0;
		$backup_count = 0;

		$write_latency_micros = 0;
		$read_latency_micros = 0;
		

	} 

	if (/^\.\.\. stopping.*after ([0-9]*).*ops: ([0-9]*) writes, ([0-9]*) deletes, ([0-9]*) /){
	$total_ops += $1;
	$total_writes += $2;
	$total_deletes += $3;
	$total_gets += $4;

	}

	if (/^Construct String microsec ([0-9]*) Flush to disk microsec ([0-9]*)/){
		$total_string_creation_time += $1;
		$total_flush_to_disk_time = $2;
		$backup_count += 1;
	}


	if (/^rocksdb\.db\.get\.micros statistics Percentiles ... 50 \: ([0-9]*\.[0-9]*)/){
		$read_latency_micros = $1;
	}

	if (/^rocksdb\.db\.write\.micros statistics Percentiles ... 50 \: ([0-9]*\.[0-9]*)/){
		$write_latency_micros = $1;
	}


}

$avg_ops = int($total_ops/($num_threads * $duration));
						
$avg_writes = int($total_writes/($num_threads * $duration));
$avg_deletes = int($total_deletes/($num_threads * $duration));
$avg_gets = int($total_gets/($num_threads * $duration));
if ($backup_count != 0) {
    $avg_write_to_disk = int ($total_flush_to_disk_time/$backup_count);
    $avg_string_creation = int ($total_string_creation_time/$backup_count);
} else{
    $avg_write_to_disk = 0;
    $avg_string_creation = 0;
}


print $out "$num_threads\t\t$avg_ops\t\t$avg_writes\t\t$avg_deletes\t\t$avg_gets\t\t$avg_string_creation\t\t$avg_write_to_disk\t\t$backup_count\t\t$read_latency_micros\t\t$write_latency_micros \n";
print "Average vals ops: $avg_ops, writes: $avg_writes, deletes: $avg_deletes, gets: $avg_gets\t\t$avg_string_creation\t\t$avg_write_to_disk\t\t$backup_count\t\t$read_latency_micros\t\t$write_latency_micros\n";


close(INPUT_FILE);
close $out;
