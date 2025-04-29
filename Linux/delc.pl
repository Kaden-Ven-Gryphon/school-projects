#!/usr/bin/perl -w
use Cwd;
my $dir = getcwd;
my $yesTxt1 = 'y';
my $yesTxt2 = 'Y';
my $quitTxt1 = 'q';
my $quitTxt2 = 'Q';


#check if their are any files spcified
if (@ARGV)
{
	@file_list = @ARGV;
}
#else print error
else
{
	print ("ERROR: no args\n");
	exit 1;
}
#for each of the files listed
foreach (@file_list)
{
	#check if its a file
	if (-f $_)
	{
		#ask to delete file and get user input
		print ("Delete? [y, q]> file: $_> ");
		$input = <STDIN>;
		chop($input);
		print ("\n");
		#if y add to list
		if(($input eq $yesTxt1) || ($input eq $yesTxt2))
		{
			push(@toDelete_list, $_);
		}
		#if q exit the loop
		elsif(($input eq $quitTxt1) || ($input eq $quitTxt2))
		{
			last;
		}
	}
	#or dir
	else
	{
		#ask the user for input
		print ("Delete? [y, q]> dir: $_> ");
		$input = <STDIN>;
		chop($input);
		print ("\n");
		#if q add file to list
		if(($input eq $yesTxt1) || ($input eq $yesTxt2))
		{
			push(@toDelete_list, $_);
		}
		#else if q exit loop
		elsif(($input eq $quitTxt1) || ($input eq $quitTxt2))
		{
			last;
		}
	}
}
#print to the list of files to be deleted
foreach (@toDelete_list)
{
	print ("$_\n");
}
#get user input
print "complete all deletions? [y]: ";
$input = <STDIN>;
chop($input);
#if y delete list of files
if(($input eq $yesTxt1) || ($input eq $yesTxt2))
{
	print("Deleteing Files\n");
	foreach (@toDelete_list)
	{
		system("rm -r \"$_\"");
	}
}
else
{
	print("No files Deleted\n");
}
exit 0;
