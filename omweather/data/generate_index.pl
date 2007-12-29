#!/usr/bin/perl

use strict;

my $source = "./countrys";
my ($countrysfile, $regionsfile, $locationsfile) = ("countrys.list", "regions.list", "locations.list");

open(COUNTRYS, ">$countrysfile") || die "Can't create file $countrysfile: $!\n";
open(REGIONS, ">$regionsfile") || die "Can't create file $regionsfile: $!\n";
open(LOCATIONS, ">$locationsfile") || die "Can't create file $locationsfile: $!\n";

my $countrys = countrylist($source);
foreach my $country (sort(@$countrys)){
    my $region_start = undef;
    
    if(defined $region_start){
	my $pos = tell(REGIONS);
	print COUNTRYS "$country;$region_start;$pos;\n";
	$region_start = $pos;
    }
    else{
	$region_start = tell(REGIONS);
	open(FH, "<$source/$country") || die "Can't create file $source/$country: $!\n";
	my ($region, $code, $location, $lat, $lon) = (undef);
	my ($location_start) = (undef);
	while(my $line = <FH>){
	    if($line =~ /^Region=(.+)/){
		if(defined $location_start && $region ne $1){
		    my $pos = tell(LOCATIONS);
		    print REGIONS "$region;$location_start;".scalar($pos - 1).";\n";
		    $location_start = $pos;
		    $region = $1;
		}
		else{
	    	    $region = $1;
		    $location_start = tell(LOCATIONS);
		}
	    }
	    else{
		if($line =~ /(.+)=(.+)=(.+)=(.+)/){
		    ($code, $location, $lat, $lon) = ($1, $2, $3, $4);
		    die "Can't get current position in $locationsfile: $!\n" if $location_start == -1;
		    print LOCATIONS "$location;$code;$lat;$lon;\n";
		}
	    }
	}
	close FH;
	if(defined $location_start){
            print REGIONS "$region;$location_start;".scalar(tell(LOCATIONS) - 1).";\n";
        }
    }
    if(defined $region_start){
        print COUNTRYS "$country;$region_start;".scalar(tell(REGIONS) - 1).";\n";
    }
}

close COUNTRYS;
close REGIONS;
close LOCATIONS;

sub countrylist{
    my $path = shift;
    my @countrys;
    
    opendir(DIR, $path) || die "Can't opendir $path: $!\n";
    @countrys = grep {/^[^\.]/} readdir(DIR);
    closedir DIR;
    return \@countrys;
}
