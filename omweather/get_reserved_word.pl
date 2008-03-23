#!/usr/bin/perl

use strict;

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
open(FOUT, ">>", "src/hash.data") or die "Can't open file: $!";
foreach $key (sort(keys %text)){
    print FOUT "\tg_hash_table_insert(hash, \"$key\", _(\"$text{$key}\"));\n";
}

close FOUT;
