#!/usr/bin/perl

use strict;


my $source = "./countries";
my ($countriesfile, $regionsfile, $locationsfile) = ("countries.list", "regions.list", "locations.list");

open(COUNTRIES, ">$countriesfile") || die "Can't create file $countriesfile: $!\n";
open(REGIONS, ">$regionsfile") || die "Can't create file $regionsfile: $!\n";
open(LOCATIONS, ">$locationsfile") || die "Can't create file $locationsfile: $!\n";

my $countries = countrylist($source);
foreach my $country (sort(@$countries)){
    my $region_start = undef;
    my ($minlat,$maxlat,$minlon,$maxlon)= (90,0,180,-180);
    if(defined $region_start){
	my $pos = tell(REGIONS);
	print COUNTRIES "$country;$region_start;$pos;\n";
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
		    print REGIONS "$region;$location_start;".scalar($pos - 1).";$minlat;$minlon;$maxlat;$maxlon;\n";
		    ($minlat,$maxlat,$minlon,$maxlon)= (90,0,180,-180);
		    $location_start = $pos;
		    $region = $1;
		}
		else{
	    	    $region = $1;
		    $location_start = tell(LOCATIONS);
		}
	    }
	    else{
		if($line =~ /(.+)=(.+)(=(.+)=(.+))?/){
		    ($code, $location, $lat, $lon) = ($1, $2, $3, $4);
		    die "Can't get current position in $locationsfile: $!\n" if $location_start == -1;
		    if ($lat < $minlat) {$minlat = $lat;}
		    if ($lat > $maxlat) {$maxlat = $lat;}
		    if ($lon < $minlon) {$minlon = $lon;}
		    if ($lon > $maxlon) {$maxlon = $lon;}
		    print LOCATIONS "$location;$code;$lat;$lon;\n";
		}
	    }
	}
	close FH;
	if(defined $location_start){
            print REGIONS "$region;$location_start;".scalar(tell(LOCATIONS) - 1).";$minlat;$minlon;$maxlat;$maxlon;\n";
        }
    }
    if(defined $region_start){
        print COUNTRIES "$country;$region_start;".scalar(tell(REGIONS) - 1).";\n";
    }
}

close COUNTRIES;
close REGIONS;
close LOCATIONS;

sub countrylist{
    my $path = shift;
    my @countries;
    
    opendir(DIR, $path) || die "Can't opendir $path: $!\n";
    @countries = grep {/^[^\.]/} readdir(DIR);
    closedir DIR;
    return \@countries;
}
