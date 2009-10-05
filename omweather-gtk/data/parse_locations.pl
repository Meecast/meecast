#!/usr/bin/perl

use strict;

my $region;


while(<>){
    if(/<a href="http:\/\/www\.weather\.com\/common\/drilldown\/.+\?from=search">.+<\/a>(.+)<\/font>/){
	$region = $1;
	$region =~ s/^\s+//; $region =~ s/^[^A-Za-z]+//;
	open(OUT, ">$region") || die "Can't create file $region: $!\n";
	print OUT "Region=$region";
    }
    if(/<a href=\"http:\/\/www\.weather\.com\/weather\/local\/([A-Z]{4}\d{4})\?from=search\">(.+)<\/a><br>/){
	print OUT "\n$1=$2"
    }
}
close OUT;

