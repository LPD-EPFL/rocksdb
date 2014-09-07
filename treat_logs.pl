#!/usr/bin/perl

opendir(DIR, ".") or die "cannot open directory";
@docs = grep(/[\_OPTERON|\_XEON]$/,readdir(DIR));
foreach $file (@docs) {
    open (FILE, $file) or die "could not open $file\n";
    $suf = "_new";
    my $newfile = "$file$suf";
    print "$newfile\n";
    open(my $out, '>', $newfile) or die "Could not open file '$newfile' $!";

    while(<FILE>){
        #print "$_";
        if (/^([0-9]*)\t\t([0-9]*)/){
        	$total_throughput = $1*$2;
        	print "$1 $2 $total_throughput\n";
        	chomp $_;
        	print "$_\t$total_throughput\n";
        	print $out "$_\t$total_throughput\n";
        } else{
        	print $out "$_";
    	}
    }
    close $out;

}