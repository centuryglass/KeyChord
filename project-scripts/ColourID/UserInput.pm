##### UserInput.pm #############################################################
# Reads, processes, and validates command line input entered through STDIN.
################################################################################

##### Functions: #####

#==============================================================================#
#--- checkInput: ---
# Repeatedly reads input lines until the user inputs a valid option.
#--- Parameters: ---
# @options: A list of strings that will be accepted as valid options.
#--- Returns: ---
# The selected option.
#==============================================================================#

#==============================================================================#
#--- confirm: ---
# Reads user input until the user confirms or rejects an option by entering 'y'
# or 'n'.
#--- Returns: ---
# Whether the user selected 'y' to confirm.
#==============================================================================#

#==============================================================================#
#--- inputText: ---
# Prompts the user to enter a new text value until a valid choice is made.
#--- Parameters: ---
# [$validatorRegex]:  An expression that text input must match before it will be
#                     accepted. If undefined, all input is considered valid.
#
# [$renameMessage]:   A message prompt to print before first accepting input. If
#                     undefined, a generic default input prompt is used.
#
# [$errorMessage]:    A message to print whenever the user inputs an invalid
#                     choice. If undefined, a generic default error message is
#                     used.
#
# [$confirmMessage]:  A message to print asking the user to confirm their
#                     selected input. If undefined, the first valid input
#                     received will be accepted without prompting for
#                     confirmation.
#--- Returns: ---
# A valid text input value, or the empty string if the user chose not to confirm
# their input.
#==============================================================================#

use strict;
use warnings;

package UserInput;

# Repeatedly reads input lines until the user inputs a valid option.
sub checkInput
{
    my @options = @_;
    my $input = "";
    while ( (scalar @options) > 0)
    {
        $input = <STDIN>;
        foreach my $option (@options)
        {
            if ($input =~ /^$option\n?$/i)
            {
                return $option;
            }
        }
    }
}

# Reads user input until the user confirms or rejects an option by entering 'y'
# or 'n'
sub confirm
{
    print("(y/n):");
    return (checkInput('y', 'n') eq 'y');
}

# Prompts the user to enter a new text value until a valid choice is made.
sub inputText
{
    my $validatorRegex = (shift || '.*');
    my $renameMessage = (shift || "Please enter the new value:");
    my $errorMessage = (shift ||  "Invalid input, try again:");
    my $confirmMessage = shift;
    my $newText;
    print($renameMessage);
    while (!defined($newText))
    {
        $newText = <STDIN>;
        #strip trailing newline
        $newText =~ s/\n$//g;
        if ($newText =~ /$validatorRegex/)
        {
            if (defined($confirmMessage))
            {
                print($confirmMessage);
                if (!confirm())
                {
                    return "";
                }
            }
        }
        else
        {
            print("$errorMessage");
            $newText = undef;
        }
    }
    return $newText;
}
1;
