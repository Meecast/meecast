#!/usr/bin/perl

use strict;
use Data::Dumper;

my $region_name = undef;
my %stations = undef;
my %regions = undef;

while(my $str = <>){
my ($name, $code, $lon, $lat) = ();
    if($str =~ /^Region=(.+)/){
	if(defined $region_name && $region_name ne $1){
	    if(defined %stations){
		my %tmp = %stations;
		$regions{$region_name} = \%tmp;
		%stations = undef;
	    }
	    $region_name = $1;
	}
	else{
	    if(!defined $region_name){
		$region_name = $1;
	    }
	}
    }
    else{
	if($str =~ /(^[A-Z]{4}[0-9]{4}|^\d+)=([^\d=]+)=?([0-9\-\.]+)?=?([0-9\-\.]+)?/){
	    ($name, $code, $lon, $lat) = ($2, $1, $3, $4);
	    chomp $name;
	    chomp $code;
	    if($lon eq ""){ $lon = undef; }
	    if($lat eq ""){ $lat = undef; }
	    if(defined $name){
		if($code =~ /^[A-Z]{4}.+/){
		    push(@{$stations{$name}}, { "weather.com" => $code } );
		}
		else{
		    push(@{$stations{$name}}, { "rp5.ru" => $code } );
		}
		if(defined $lon && defined $lat){
		    push(@{$stations{$name}}, { "coordinates" => [ $lon, $lat ] } );
		}
	    }
	}
    }
}
    $regions{$region_name} = \%stations;

foreach my $region (sort keys(%regions)){
    if($region ne ""){
	print "Region=$region\n";
	foreach my $station (sort keys(%{$regions{$region}})){
	    chomp $station;
	    print "$station";
	    if($station ne ""){
		my @data = @{$regions{$region}->{$station}};
		my ($weather_com_code, $rp5_ru_code, $lon, $lat) = ();
		foreach my $item (@data){
		    if(exists($item->{"weather.com"})){
			$weather_com_code = $item->{"weather.com"};
		    }
		    if(exists($item->{"rp5.ru"})){
			$rp5_ru_code = $item->{"rp5.ru"};
		    }
		    if(exists($item->{"coordinates"})){
			$lon = $item->{"coordinates"}[0];
			$lat = $item->{"coordinates"}[1];
		    }
		}
		print "=$weather_com_code=$rp5_ru_code=$lon=$lat\n";
	    }
	}
    }
}

