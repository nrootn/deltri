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
print NODE "# node count, 3 dim, region attribute, no boundary marker\n";

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
print NODE "$total_nodes 3 0 1\n";
print NODE "# node index, node coordinates\n";

my $i = 1;
my $regionID = 1;
foreach my $nodes (@nodes) {
	for (my $j = 0; $j < $nodes; $j++) {
		my $line = <ROI>;
		chomp($line);
		print NODE "$i $line $regionID\n";
		$i++;
	}
	$regionID++;
}

close(ROI);
close(NODE);