#!/usr/bin/perl

use strict;

my @result;

while(<>){
#    if(/<a\s+href=\"http:\/\/rp5\.ru\/get_newinformer\.php\?id=(\d+)\">/){
    while(/<a href="http:\/\/rp5\.ru\/town\.php\?id=(\d+)"\s+class="mpl">(\D+)<\/a>/g){
	push @result, "$1=$2\n";
    }
}
print @result

__END__
<a href="http://rp5.ru/town.php?id=7546" class="mpl">Aflenz</a>
<a href="http://rp5.ru/get_newinformer.php?id=13139">