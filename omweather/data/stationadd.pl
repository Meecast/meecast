#!/usr/bin/perl -w

open (SOURCE_FILE,"stationsList.txt");
#open (SOURCE_FILE,"s2.txt");
my $country = "";
my $country_dest = "";
my $station_dest = "";
my $town = "";
my $flag_find = 0;
my $flag_space = 0;
my @abouttown;
my $find_town = "";
my $temp_string ="";
my $temp_string2 ="";
my $zip_code = "";
my $i;


while(<SOURCE_FILE>){
    if  ($flag_space == 1)
    {
     $country = $_;
     chomp($country);
     $flag_space = 0;
#    print $country;
    }
    if ($_ eq "\n")
    {
     $flag_space = 1;
     $country = "";
     $country_dest = "";
     $town = "";
     $flag_find = 0;
    }
#    print $_;
    if ( $_ =~ m/.*;.*/  )
    {
     @abouttown = split(/;/,$_);
     $town = $abouttown[0];
     chomp($town);
     $zip_code = $abouttown[1];
     chomp($zip_code);
     
    }
    if (!($country eq "") and !($town eq ""))
     {
#     print "1 $country\n";
      open (DESTINATION_FILE,"stations.txt");
      open (DESTINATION_FILE2,">stationsoutput.txt");      
#      open (DESTINATION_FILE,"s1.txt");
      while(<DESTINATION_FILE>){

	 if ((length ($_)>3) and (length ($_)<83) and (!(substr($_,3,1) eq " ")) and (!(substr($_,0,1) eq "!")))
	 {
	  $country_dest=substr($_,0,19);
	  $country_dest =~ s/\s+$//;
	 }
#      print "$country_dest,$country 1\n";
   if ((length ($_)>82)  and (($country_dest eq $country) or (($country eq "CANADA") and (substr($_,81,2) eq "CA"))))
	 {
#	  print "2 $country_dest \n";
          $station_dest = substr($_,3,16);
	  $station_dest =~ s/\s+$//;
#	  print "$station_dest\n";
#          print ("$town,$station_dest\n");
	  if ($town eq  $station_dest)
	   {
	    $flag_find = 1;$find_town = $town; 
	   }
#	  print substr($_,20,4),",",$abouttown[2];    
	   my $temp1 = substr($_,20,4);
	   my $temp2 = $abouttown[2];
#	   print "$station_dest length ",length ($temp1), " ", length ($temp2)," $temp1 $temp2 \n";
	  if ( $temp1 eq $temp2 )
	  {
	   if ($flag_find == 0)
	   {
#	    print ("STRANGE $town and $station_dest\n");    
	    $flag_find = 1;$find_town = $town; 
	   }
	   else 
	   {
	    $flag_find = 1;
#            print "$town\n"; 
	   }
	  }
	   my $temp1 = substr($_,32,5);
	   my $temp2 = $abouttown[3];
#	   print "$station_dest length ",length ($temp1), " ", length ($temp2)," $temp1 $temp2 \n";
	  if ( $temp1 eq $temp2 )
	  {
	   if ($flag_find == 0)
	   {
#	    print ("STRANGE $town and $station_dest\n");    
	    $flag_find = 1;$find_town = $town; 
	   }
	   else 
	   {
	    $flag_find = 1;
#            print "$town\n"; 
	   }
	  }
#	  print "$country_dest\n";
	 }
	 if (($_ eq "\n") and ($country_dest eq $country) and ($flag_find == 0) and !($town eq ""))
	  {
	   $temp_string2 = "   $town";
	   print DESTINATION_FILE2 $temp_string2;  
	   for ($i=length($temp_string2);$i<84;$i++)
	   {
	     print DESTINATION_FILE2 " ";
	   }
	   print DESTINATION_FILE2   "$abouttown[1]\n";
	  }
        if ($flag_find == 0)
         {
	   print DESTINATION_FILE2 $_;
	 }
	else
	 {
	   $temp_string = $_;
	   chomp $temp_string;
	   print DESTINATION_FILE2 $temp_string," $abouttown[1]\n";
	   $flag_find = 0;
	   $town = "";
	 }   
      } 
      close (DESTINATION_FILE);
      close (DESTINATION_FILE2);
      rename "stationsoutput.txt","stations.txt";
     }
}