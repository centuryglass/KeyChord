#!/usr/bin/perl
# Reads in a pixel font from a black and white image file, saving its data to
# Text_BinaryFont.cpp.

use strict;
use warnings;
use Image::Magick;
use File::Slurp;
use File::Basename;

use constant IMG_WIDTH => 160;
use constant IMG_HEIGHT => 160;
use constant RELATIVE_OUTPATH => '../Source/GUI/Text/Text_BinaryFont.cpp';

my $mapVarName = 'const juce::uint32 fontMap';

my $path = $ARGV[0];

my $dirname = dirname(__FILE__);
my $outPath = $dirname."/".RELATIVE_OUTPATH;

if (! -f $path)
{
    die("No valid input path!\n");
}
if (! -f $outPath)
{
    die("No valid output path!\n Path tried: \"$outPath\"");
}

my $image = new Image::Magick;
$image->Read($path);

my $output = "$mapVarName [".( (IMG_WIDTH / 32) * IMG_HEIGHT)."] = \n{";

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

my $updatedFile = read_file($outPath);
$updatedFile =~ s/\v$mapVarName.*?;/\n$output/s;
write_file($outPath, $updatedFile);
