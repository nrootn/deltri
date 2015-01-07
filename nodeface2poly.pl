#!/usr/local/bin/perl
use strict;
use warnings;
use Getopt::Long;

my $node_file;
my $face_file;
# -n node file
# -f face file
GetOptions("node|n=s" => \$node_file,
		   "face|f=s" => \$face_file
	);

# Open input files
open(NODE, '<', $node_file) or die("Invalid node file specified");
open(FACE, '<', $face_file) or die("Invalid face file specified");

# Delete output file if it exists and create new output file
(my $filename = $node_file) =~ s/\.[^.]+$//;
system("rm ${filename}\.poly") if -e $filename . ".poly";
open(POLY, '>>', $filename . ".poly") or die("Cannot create output file");

# Skip comment lines
my $line;
while ($line = <NODE>) {
	chomp($line);
	if ($line =~ /^\s*#/) { next; }
	last;
}

# Write nodes to output
print POLY "# Part 1 - node list\n";
print POLY "# node count, 3 dim, region attribute, boundary marker\n";
# Make sure string delimiter is one space only
$line =~ s/\s+/\ /g;
print POLY "$line\n";
print POLY "# node index, node coordinates\n";

# Store lines for future use
my @node_lines = <NODE>;

my @tmp_line;
foreach $line (@node_lines) {
	if ($line =~ /^\s*#/) { next; }
	# Get rid of whitespace at start/end of string
	$line =~ s/^\s+|\s+$//g;
	# Make sure string delimiter is one space only
	$line =~ s/\s+/\ /g;
	@tmp_line = split(/\ /, $line);
	print POLY "$tmp_line[0] $tmp_line[1] $tmp_line[2] $tmp_line[3]\n";
}

# Read through input face file and write faces to output
$line = <FACE>;
print POLY "\n# Part 2 - facet list\n";
print POLY "# facet count, 3 dim, region attribute, boundary marker\n";
print POLY "$line";
print POLY "# facets\n";

my @nodes;
my $regionID;
while ($line = <FACE>) {
	# Skip comment lines
	if ($line =~ /^\s*#/) { next; }
	@nodes = split(/\s* \s*/, $line);
	chomp(@nodes);

	# Figure out region ID
	$regionID = (split /\s* \s*/, $node_lines[$nodes[2]-1])[-1];
	chomp($regionID);

	# Print face to output file
	print POLY "1 0 $regionID\n";
	print POLY "3 $nodes[2] $nodes[3] $nodes[4]\n";
}

print POLY "\n# Part 3 - hole list\n";
print POLY "0    # no hole\n";

print POLY "\n# Part 4 - region list\n";
print POLY "0    # no region\n";

# Close input files
close(NODE);
close(FACE);
# Close output files
close(POLY);