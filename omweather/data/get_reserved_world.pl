#!/usr/bin/perl

use strict;
use Data::Dumper;

my %text;
my $key;

while(my $buf = <>){
    if( $buf =~ /.+<t>.+<\/t>/){
	$buf =~ s/^(\s+)?//;
	$buf =~ s/^(<t>)//;
	$buf =~ s/(<\/t>)//;
	$buf =~ s/[\n\r]+//;
	if(!exists($text{$buf})){
	    $text{$buf} = $buf;
	}
    }
}
foreach $key (sort(keys %text)){
    print "\tg_hash_table_insert(hash, \"$key\", _(\"$text{$key}\"));\n";
}
