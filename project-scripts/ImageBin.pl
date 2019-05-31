#!/usr/bin/perl
use strict;
use warnings;
use Image::Magick;
use File::Slurp;

use constant IMG_WIDTH => 160;
use constant IMG_HEIGHT => 160;

my $path = $ARGV[0];
my $outPath = $ARGV[1];

if (! -f $path)
{
    die("No valid input path!\n");
}
if (! defined($outPath))
{
    die("No valid output path!\n");
}

my $image = new Image::Magick;
$image->Read($path);

my $output = "juce::uint32 fontMap [".( (IMG_WIDTH / 32) * IMG_HEIGHT)
        ."] = \n{";

my ($width, $height) = $image->Get('width', 'height');
if ($width != IMG_WIDTH || $height != IMG_HEIGHT)
{
    die("Invalid image dimensions $width x $height, expected "
            .IMG_WIDTH." x ".IMG_HEIGHT."\n");
}

foreach my $row (0 ... (IMG_HEIGHT - 1))
{
    my $rowText = "    ";
    foreach my $column (0 ... (IMG_WIDTH - 1))
    {
        if (($column % 32) == 0)
        {
            if ($column > 0)
            {
                $rowText = "$rowText, ";
            }
            $rowText = $rowText."0b";
        }
        my @pVal = $image->Get("Pixel[$column, $row]");
        if (substr($pVal[0], 0, 1) eq "0")
        {
            $rowText = $rowText."1";
        }
        else
        {
            $rowText = $rowText."0";
        }
    }
    if ($row == 0)
    {
        $output = "$output\n$rowText";
    }
    else
    {
        $output = "$output,\n$rowText";
    }
}
$output = "$output\n};\n";
write_file($outPath, $output);
