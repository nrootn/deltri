#!/usr/local/bin/perl
use strict;
use warnings;
use Getopt::Long;

my $input_file;
# -i input file
GetOptions("input|i=s" => \$input_file
	);

# Open input file
open(ROI, '<', $input_file) or die("Invalid input file specified");

# Delete output file if it exists and create new output file
(my $filename = $input_file) =~ s/\.[^.]+$//;
system("rm ${filename}\.node") if -e $filename . ".node";
open(NODE, '>>', $filename . ".node") or die("Cannot create output file");

# Write line 1
print NODE "# Node count, 3 dim, no attribute, no boundary marker\n";

# Skip comment lines
while (my $line = <ROI>) {
	chomp($line);
	if ($line =~ /^\s*#/) { next; }
	last;
}

# Get line of nodes
my $line = <ROI>;
chomp($line);
my @nodes = split(/\ /, $line);

# Add up all the nodes
my $total_nodes = 0;
foreach my $nodes (@nodes) { $total_nodes += $nodes; }

# Write the rest of the output
print NODE "$total_nodes 3 0 0\n";
print NODE "# Node index, node coordinates\n";

my $i = 1;
while (my $line = <ROI>) {
	print NODE "$i $line";
	$i++;
}

close(ROI);
close(NODE);