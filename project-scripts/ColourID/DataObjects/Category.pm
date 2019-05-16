##### Category.pm: #############################################################
# Represents a Theme::Colour::UICategory value.
################################################################################

##### Functions: #####

#==============================================================================#
#--- new: ---
# Creates a new Category object.
#--- Parameters: ---
# $value: The category type the object should represent. This may be given as
#         one of the category type constants defined below, as a valid
#         category name, or as a valid category JSON key string.
#- Returns: ---
# The new Category object, or undef if no valid category value was given.
#==============================================================================#

#==============================================================================#
#--- getValue: ---
# Gets a category's numeric value.
#--- Returns: ---
# The category value as one of the category constants defined below. This will
# match the integer value of the UICategory enum value the object represents.
#==============================================================================#

#==============================================================================#
#--- getTypeName: ---
# Gets the category's enum value name.
#--- Returns: ---
# The name of the category's UICategory enum value.
#==============================================================================#

#==============================================================================#
#--- getKey: ---
# Gets the category's colour configuration key value.
#--- Returns: ---
# The key used to store the category's colour in the colours.json file.
#==============================================================================#

use strict;
use warnings;

package Category;
use Scalar::Util qw(blessed);
use Scalar::Util::Numeric qw(isint);

# Category types:
use constant WINDOW_BACKGROUND      => 0;
use constant WIDGET_BACKGROUND      => 1;
use constant WIDGET_ON              => 2;
use constant WIDGET_OFF             => 3;
use constant MENU_BACKGROUND        => 4;
use constant OUTLINE                => 5;
use constant FOCUSED_OUTLINE        => 6;
use constant TEXT                   => 7;
use constant HIGHLIGHTED_TEXT       => 8;
use constant TEXT_FIELD             => 9;
use constant HIGHLIGHTED_TEXT_FIELD => 10;
use constant NONE                   => 11;

use constant NUM_TYPES => 12;

my @categoryNames = (
        'windowBackground',
        'widgetBackground',
        'widgetOn',
        'widgetOff',
        'menuBackground',
        'outline',
        'focusedOutline',
        'text',
        'highlightedText',
        'textField',
        'highlightedTextField',
        'none',
);

my @categoryKeys = (
        'Window background',
        'Widget background',
        'Widget',
        'Widget (off)',
        'Menu background',
        'Outline',
        'Focused outline',
        'Text',
        'Highlighted text',
        'Text field',
        'Highlighted text field',
        'none',
);

# Create a Category object from a category value, another category, a category
# name, or a category key.
sub new
{
    my $class = shift;
    my $type = shift;
    my $self = undef;
    if (isint($type) && ($type >= 0) && ($type < NUM_TYPES))
    {
        $self =
        {
            _value => $type
        };
    }
    elsif (blessed($type) && blessed($type) eq 'Category')
    {
        $self =
        {
            _value => $type->getValue()
        };
    }
    sub checkNames
    {
        my $self = shift;
        my $type = shift;
        my @nameArray = @_;
        if (!defined($self))
        {
            foreach my $i (0 ...(NUM_TYPES - 1))
            {
                if ($type eq $nameArray[$i])
                {
                    $self =
                    {
                        _value => $i
                    };
                    return $self;
                }
            }
        }
        return $self;
    }
    $self = checkNames($self, $type, @categoryNames);
    $self = checkNames($self, $type, @categoryKeys);
    if (defined($self))
    {
        bless($self, $class);
    }
    return $self;
}

# Gets the Category's numeric category value
sub getValue
{
    my $self = shift;
    return $self->{_value};
}

# Gets the Category's value type name
sub getTypeName
{
    my $self = shift;
    return $categoryNames[$self->{_value}];
}

# Gets the Category's value key
sub getKey
{
    my $self = shift;
    return $categoryKeys[$self->{_value}];
}
1;
