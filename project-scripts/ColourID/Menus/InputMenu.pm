##### InputMenu.pm #############################################################
# Creates and manages a text menu interface.                                   #
##### Functions: #####

#==============================================================================#
#--- new: ---
# Initializes a new InputMenu.
#--- Parameters: ---
# $title:       The title to print at the top of the menu.
#
# [$exitText]:  The text to print describing the exit command.
#
# [$action]:    The action to run when a valid option is selected, passing in
#               the option value as the first parameter. If undefined, the
#               default action attempts to run the selected option value as a
#               function reference, passing on all additional arguments.
#
# [@args]:      Additional arguments to pass to the action function.
#--- Returns: ---
# The new menu object.
#==============================================================================#

#==============================================================================#
#--- addOption: ---
# Adds a new option to the menu.
#--- Parameters: ---
# $description: Text to print describing the option.
#
# $optionData:  The first parameter to pass to the action command if the user
#               selects this option.
#==============================================================================#

#==============================================================================#
#--- clearOptions: ---
# Removes all options from the menu.
#==============================================================================#

#==============================================================================#
#--- setRefreshAction: ---
# Saves a function that the menu should perform to update itself before each
# time it prints out the available menu options.
#-- Parameters: --
# $refreshAction: A function reference that will run each time the menu is
#                 reprinted. The menu object will be passed to this function as
#                 its only parameter.
#==============================================================================#

#==============================================================================#
#--- setTitle: ---
# Changes the menu title printed before listing menu options.
#-- Parameters: --
# $title: A new title string to print.
#==============================================================================#

#==============================================================================#
#--- openMenu: ---
# Displays the menu, repeatedly accepting input and running the menu action
# with the selected option parameter until the user enters 'q'.
#==============================================================================#

#==============================================================================#
#--- closeMenu: ---
# If the menu is currently open, ensures that the menu will close before
# taking any further actions.
#==============================================================================#

use strict;
use warnings;

# Creates a custom command line input menu.
package InputMenu;

# Initializes a new InputMenu.
sub new
{
    my ($class, $title, $exitText, $action, @args) = @_;
    if (!$title)
    {
        $title = "Select a menu option:";
    }
    if (!$exitText)
    {
        $exitText = "Exit";
    }
    if (!$action)
    {
        $action = sub
        {
            my $option = shift;
            my @otherArgs = @_;
            $option->(@otherArgs);
        };
    }
    my $self =
    {
        _title    => $title,
        _exitText => $exitText,
        _action   => $action,
        _args     => \@args,
        _options  => [],
        _refresh  => sub { }
    };
    bless($self, $class);
    return $self;
}

# Adds a new option to the menu.
sub addOption
{
    my ($self, $description, $optionData) = @_;
    push(@{$self->{_options}}, [$description, $optionData]);
}

# Removes all options from the menu.
sub clearOptions
{
    my $self = shift;
    $self->{_options} = [];
}

# Saves a function that the menu should perform to update itself before each
# time it prints out the available menu options.
sub setRefreshAction
{
    my $self = shift;
    my $refreshAction = shift;
    $self->{_refresh} = $refreshAction;
}


# Changes the menu title printed before listing menu options.
sub setTitle
{
    my ($self, $newTitle) = @_;
    $self->{_title} = $newTitle;
}


# Displays the menu, accepting input until the user enters 'q'
sub openMenu
{
    my $self = shift;
    delete($self->{_shouldClose});
    my $input = "";
    while (! ($input =~ /q/))
    {
        if (exists($self->{_shouldClose}))
        {
            return;
        }
        $self->{_refresh}->($self);
        if (exists($self->{_shouldClose}))
        {
            return;
        }
        my $infoText = "\n".$self->{_title}."\n";
        my $index = 1;
        my $numOptions = @{$self->{_options}};
        foreach my $option (@{$self->{_options}})
        {
            $infoText = $infoText."$index: ".$option->[0]."\n";
            $index++;
        }
        $infoText = $infoText."q: ".$self->{_exitText}."\n";
        print($infoText);
        $input = <STDIN>;
        if ($input =~ /(\d+)/)
        {
            my $index = $1 - 1;
            if ( ($index >= 0) && ($index < $numOptions))
            {
                my $optionData = $self->{_options}->[$index]->[1];
                {
                    $self->{_action}->($optionData, @{$self->{_args}});
                }
            }
        }
    }
}

# If the menu is currently open, ensures that the menu will close before
# taking any further actions.
sub closeMenu
{
    my $self = shift;
    $self->{_shouldClose} = 1;
}
1;
