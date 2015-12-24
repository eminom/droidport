
package appconf;

use 5.010;
use strict;
use warnings;

use base qw/Exporter/;
our @EXPORT_OK = qw/goArchi/;

sub goArchi{
	my $arch = shift // die "No arch specified";
	die "Not recognized arch" if not grep{$_ eq $arch} qw/x86 armeabi/;
	my $target = 'Application.mk';
	my $src = "$target.template";
	open my $fin ,"<", "$src" or die "cannot open $src";
	open my $fout,">", "$target" or die "cannot open $target";
	while(<$fin>) {
		s/\$\{AppABI\}/$arch/;
		print {$fout} $_;
	}
	close $fin;
	close $fout;
	#print "done with $arch\n";
}

1;