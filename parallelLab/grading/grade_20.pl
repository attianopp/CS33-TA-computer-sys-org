#!/usr/bin/perl 
#!/usr/local/bin/perl 
use Getopt::Std;
use Cwd;
use lib ".";
use config;
use File::Find;

#########################################################################
# grade_20.pl - Parallel Lab autograder
#
# Copyright (c) 2020 UCLA - Spring 2020 TA: Attiano Purpura-Pontoniere
#
# This program automatically grades the all the Parallel Lab handin parallel.c files in the handin directory.
# dir structure: (src handin grade_20.pl config.pm) at the root level
# handin has a subdirectory for each student submission, in the form of: ./handin/uid/parallel.c
#
# Example usage:
#     unix> ./grade_20.pl
# 
#########################################################################

$| = 1; # autoflush output on every print statement

# Any files created by this script are readable only by the user
umask(0077); 

#
# usage - print help message and terminate
#
sub usage {
    printf STDERR "$_[0]\n";
    printf STDERR "Usage: $0 [-he] -f <filename> [-s <srcdir>]\n";
    printf STDERR "Options:\n";
    printf STDERR "  -h           Print this message.\n";
    printf STDERR "  -f <file>    C file to be graded.\n";
    printf STDERR "  -s <srcdir>  Location of directory with support code\n";
    printf STDERR "  -e           Don't include the original handin file on the grade sheet\n";
    die "\n";
}



##############
# Main routine
##############

# 
# Parse the command line arguments
#
getopts('hef:s:');
if ($opt_h) {
    usage();
}


# 
# These optional flags override defaults in config.pm
#
if ($opt_s) {         # driver src directory
    $SRCDIR = $opt_s;
}

# 
# Initialize some file and path names
#
$handin = $HANDINDIR . "/*"; 
$srcdir_abs = `cd $SRCDIR; pwd`; 
chomp($srcdir_abs);
$0 =~ s#.*/##s;       # this prog's basename
$out_file = "scores.txt";

#
# Open output file or create it if it doesnt exist
#
open(OFH, '>', $out_file) or die $!;


# 
# Loop through every submission in the handin directory assumes handin dir has the form /uid/parallel.c for each student with uid
#

my @dirs = grep { -d } glob $handin;

foreach ( @dirs ) {
	$score = -1;
	$extra_credit = 0;
	#printf "\$_ is $_\n";
	($uid = $_) =~ s#.*/##s; #get the students uid
	$infile = $_."/parallel.c";
	
	#overrides file to be graded
	if ($opt_f) {
		$infile = $opt_f;
	}
	
	($infile_basename = $infile) =~ s#.*/##s; # basename of input file
	$tmpdir = "/var/tmp/$uid.$$"; # scratch directory
	$diemsg = "The files are in $tmpdir. \n"; # This is a message we use in several places when the program dies
	
	# Make sure the input file exists and is readable
	if(!open(INFILE, $infile)){ 
		$score = -1;
		$out_string = "$uid $score $extra_credit \n";
		print OFH $out_string;
		print OFH "$0: ERROR UID-$uid: could not open file $infile for student: $uid \n";
		printf STDERR "$0: ERROR UID-$uid: could not open file $infile for student: $uid \n";
		next;
	}
	else{
		close(INFILE);
	}
	
	# Set up the contents of the scratch directory
	if(!(system("mkdir $tmpdir") == 0)){
		$score = -1; #ERROR
		$out_string = "$uid $score $extra_credit \n";
		print OFH $out_string;
		print OFH "ERROR UID-$uid: Unable to make $tmpdir.";
		printf STDERR "ERROR UID-$uid: Unable to make $tmpdir.";
		system("rm -fr $tmpdir"); # delete tmp dir for this student
		chdir($cwd);
		next;
	}

	# Source file to be graded
	if(!(system("cp $infile $tmpdir/parallel.c") == 0)){
		$score = -1; #ERROR
		$out_string = "$uid $score $extra_credit \n";
		print OFH $out_string;
		print OFH "ERROR UID-$uid: Unable to copy $infile to $tmpdir.";
		printf STDERR "ERROR UID-$uid: Unable to copy $infile to $tmpdir.";
		system("rm -fr $tmpdir"); # delete tmp dir for this student
		chdir($cwd);
		next;
	}
	# Autograding tools - Copy over main.c Makefile sequential.c utils.h from src
	
	if(!(system("cp $srcdir_abs/main.c $tmpdir") == 0)){
		$score = -1; #ERROR
		$out_string = "$uid $score $extra_credit \n";
		print OFH $out_string;
		print OFH "ERROR UID-$uid: Unable to copy $srcdir_abs/main.c to $tmpdir.";
		printf STDERR "ERROR UID-$uid: Unable to copy $srcdir_abs/main.c to $tmpdir.";
		system("rm -fr $tmpdir"); # delete tmp dir for this student
		chdir($cwd);
		next;
	}
	
	if(!(system("cp $srcdir_abs/Makefile $tmpdir") == 0)){
		$score = -1; #ERROR
		$out_string = "$uid $score $extra_credit \n";
		print OFH $out_string;
		print OFH "ERROR UID-$uid: Unable to copy $srcdir_abs/Makefile to $tmpdir.";
		printf STDERR "ERROR UID-$uid: Unable to copy $srcdir_abs/Makefile to $tmpdir.";
		system("rm -fr $tmpdir"); # delete tmp dir for this student
		chdir($cwd);
		next;
	}
	
	if(!(system("cp $srcdir_abs/sequential.c $tmpdir") == 0)){
		$score = -1; #ERROR
		$out_string = "$uid $score $extra_credit \n";
		print OFH $out_string;
		print OFH "ERROR UID-$uid: Unable to copy $srcdir_abs/sequential.c to $tmpdir.";
		printf STDERR "ERROR UID-$uid: Unable to copy $srcdir_abs/sequential.c to $tmpdir.";
		system("rm -fr $tmpdir"); # delete tmp dir for this student
		chdir($cwd);
		next;
	}
	
	if(!(system("cp $srcdir_abs/utils.h $tmpdir") == 0)){
		$score = -1; #ERROR
		$out_string = "$uid $score $extra_credit \n";
		print OFH $out_string;
		print OFH "ERROR UID-$uid: Unable to copy $srcdir_abs/utils.h to $tmpdir.";
		printf STDERR "ERROR UID-$uid: Unable to copy $srcdir_abs/utils.h to $tmpdir.";
		system("rm -fr $tmpdir"); # delete tmp dir for this student
		chdir($cwd);
		next;
	}
	
	# Run the driver in the scratch directory
	$cwd = getcwd(); 
	chdir($tmpdir) 
		or die "ERROR: Unable to switch to scratch directory $tmpdir\n";


	
	# $ENV{'PATH'}='/usr/bin';
	# system("export PATH=/usr/bin:$PATH") == 0
	#     or die "ERROR: Unable to switch gcc 4.8.5";
	system("gcc -v") == 0
		or die "ERROR: Unable to show gcc version \n";

	if(!(system("make") == 0)){
		$score = -1;
		$out_string = "$uid $score $extra_credit \n";
		print OFH $out_string;
		print OFH "ERROR UID-$uid: Unable to run make - compilation error \n";
		printf STDERR "ERROR UID-$uid: Unable to make \n";
		system("rm -fr $tmpdir"); # delete tmp dir for this student
		chdir($cwd);
		next;
	}
	if(!(system("./Test") == 0)){
		$score = -1;
		$out_string = "$uid $score $extra_credit \n";
		print OFH $out_string;
		print OFH "ERROR UID-$uid: Unable to run Test - runtime error \n";
		printf STDERR "ERROR UID-$uid: Unable to run Test \n";
		system("rm -fr $tmpdir"); # delete tmp dir for this student
		chdir($cwd);
		next;
	}
	
	#read score.txt and get score/extra_credit
	$score_file = "score.txt";
	if(!open(SFH, '<', $score_file)){ 
		$score = -1; # ERROR!
		$out_string = "$uid $score $extra_credit \n";
		print OFH $out_string;
		print OFH "$0: ERROR UID-$uid: could not open file $score_file for student: $uid \n";
		printf STDERR "$0: ERROR UID-$uid: could not open file $score_file for student: $uid \n";
		next;
	}
	#read in first line of score.txt that should have: "score extra_credit\n"
	$score_line = <SFH>;
	
	#split string based on space
	my @score_arr = split(' ', $score_line); 
	  
	# put the score and extra credit in the respective variables
	for my $i (0 .. $#score_arr)
	{
		if($i == 0){
			$score = $score_arr[$i];
		}
		if($i == 1){
			$extra_credit = $score_arr[$i];
		}
	}
	$out_string = "$uid $score $extra_credit \n";
	print OFH $out_string;
	close(SFH);

	system("rm -fr $tmpdir"); # delete tmp dir for this student
	chdir($cwd);
}

#
# close output file
#
close(OFH);



#old code

# #
# # Make sure the driver program exists and is executable
# # 
# # (-e "$srcdir_abs/driver.pl" and -x "$srcdir_abs/driver.pl")
# #     or  die "$0: ERROR: No executable driver.pl program in $srcdir_abs.\n";


# # 
# # Set up the contents of the scratch directory
# #
# system("mkdir $tmpdir");

# # Source file to be graded
# system("cp $infile $tmpdir/parallel.c") == 0
    # or die "ERROR: Unable to copy $infile to $tmpdir.";

# # Autograding tools
# system("cp $srcdir_abs/main.c $tmpdir") == 0
    # or die "ERROR: Unable to copy $srcdir_abs/main.c to $tmpdir.";
# system("cp $srcdir_abs/Makefile $tmpdir") == 0
    # or die "ERROR: Unable to copy $srcdir_abs/Makefile to $tmpdir.";
# system("cp $srcdir_abs/sequential.c $tmpdir") == 0
    # or die "ERROR: Unable to copy $srcdir_abs/sequential.c to $tmpdir.";
# system("cp $srcdir_abs/utils.h $tmpdir") == 0       
    # or die "ERROR: Unable to copy $srcdir_abs/utils.h to $tmpdir.";

# # Print header
# print "\nCS:APP Parallel Lab: Grading Sheet for $infile_basename\n\n";

# #
# # Run the driver in the scratch directory
# #
# $cwd = getcwd(); 
# chdir($tmpdir) 
    # or die "ERROR: Unable to switch to scratch directory $tmpdir\n";



# # $ENV{'PATH'}='/usr/bin';
# # system("export PATH=/usr/bin:$PATH") == 0
# #     or die "ERROR: Unable to switch gcc 4.8.5";
# system("gcc -v") == 0
    # or die "ERROR: Unable to show gcc version";
# system("make") == 0
    # or die "ERROR: Unable to make";
# system("./Test") == 0
    # or die "ERROR: Unable to run Test";

# # $ENV{'PATH'}='/usr/local/cs/bin';
# # # system("export PATH=/usr/local/cs/bin:$PATH") == 0
# # #     or die "ERROR: Unable to switch gcc 9.1.0";
# # system("gcc -v") == 0
# #     or die "ERROR: Unable to show gcc version";
# # system("make") == 0
# #     or die "ERROR: Unable to make";
# # system("./Test") == 0
# #     or die "ERROR: Unable to run Test";
# # chdir($cwd) 
# #     or die "ERROR: Unable to switch back to current working directory\n";


# #
# # Print the grade summary template that the instructor fills in
# #
# # print "\nPart 4: Grade\n\n";
# # print "Correctness: \t\t     / $MAXCORR\n\n";
# # print "Performance: \t\t     / $MAXPERF\n\n";
# # print "Coding Style:\t\t     / $MAXSTYLE\n\n";
# # print "             \t\t__________\n\n";
# # print "Total:       \t\t     / ", $MAXCORR+$MAXPERF+$MAXSTYLE, "\n";   

# # 
# # Optionally print the original handin file 
# #
# # if (!$opt_e) {
# #   print "\f\nPart 5: Original $infile file\n\n";
# #   system("cat $infile") == 0
# #     or die "ERROR: Unable to print $infile.";
# # }

# #
# # Everything went OK, so remove the scratch directory
# #

# system("rm -fr $tmpdir");
exit;


