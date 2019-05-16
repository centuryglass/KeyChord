##### Element.pm  ##############################################################
# Represents a C++ Theme::Colour::Element object.
################################################################################

##### Functions: #####

#==============================================================================#
#--- new ---
# Creates a new Element object from element data params.
#--- Parameters ---
# $namespace:   The element's namespace name.
#
# $name:        The element's object name.
#
# $id:          The element's integer ID value.
#
# [$category]:  The element's UICategory object, a value that can be used to
#               construct a valid Category object, or no value to default to
#               Category::NONE.
#
# [$key]:       The element's optional JSON colour key.
#
# [$colour]:    The element's optional default JSON colour value.
#--- Returns ---
# The new element object, or undef if invalid parameters were given.
#==============================================================================#

#==============================================================================#
#--- new: ---
# Creates a new Element object by parsing a C++ Element definition.
#--- Parameters: ---
# $namespace: The element's namespace name.
#
# $text:      The full text of a C++ Element definition.
#--- Returns: ---
# The new element object parsed from the C++ declaration, or undef if the text
# was not a valid declaration.
#==============================================================================#

#==============================================================================#
#--- getNamespace: ---
# Gets the name of the namespace that contains the Element.
#--- Returns: ---
# The namespace name provided on construction.
#==============================================================================#

#==============================================================================#
#--- getName: ---
# Gets the Element's colour value name.
#--- Returns: ---
# The name of the UI colour element the object represents.
#==============================================================================#

#==============================================================================#
#--- getID: ---
# Gets the Element's ColourId value.
#--- Returns: ---
# The element's hexadecimal number string value.
#==============================================================================#

#==============================================================================#
#--- getCategory: ---
# Gets the element's Category enum object.
#--- Returns: ---
# The UI element category that best matches the Element
#==============================================================================#

#==============================================================================#
#--- getKey: ---
# Gets the element's optional JSON colour key.
#--- Returns: ---
# The key string used to store the element's colour in the colours.json file,
# or the empty string if no key is set.
#==============================================================================#

#==============================================================================#
#--- getDefaultColour: ---
# Gets the colour value assigned to the element in the default colours.json
# file.
#--- Returns: ---
# The default colour assigned to this element in the configuration file, or the
# empty string if no key is set.
#==============================================================================#

#==============================================================================#
#--- getFullName: ---
# Gets the full name used to identify the Element in C++.
#--- Returns: ---
# The full name including namespace, formatted as Namespace::Name.
#==============================================================================#

#==============================================================================#
#--- getDeclaration: ---
# Gets a C++ Theme::Colour::Element constant declaration using this object's
# values.
#--- Parameters: ---
# [$indent]: Optional number of indentations to include at the beginning of
#            each line in the returned declaration.
#--- Returns: ---
# An Element declaration that may be used in a C++ header file.
#==============================================================================#

#==============================================================================#
#--- sortValue: ---
# Gets a value that may be used when sorting Elements by ID.
#--- Returns: ---
# The numeric representation of the Element ID value.
#==============================================================================#

#==============================================================================#
#--- withNamespace: ---
# Gets an element identical to this one, but with a different namespace.
#--- Parameters: ---
# $namespace: A new namespace name to apply to the copied element.
#--- Returns: ---
# An element identical to this one, except for the new namespace value. 
#==============================================================================#

#==============================================================================#
#--- withName: ---
# Gets an element identical to this one, but with a different name.
#--- Parameters: ---
# $name: A new name to apply to the copied element.
#--- Returns: ---
# An element identical to this one, except for the new name value. 
#==============================================================================#

#==============================================================================#
#--- withID: ---
# Gets an element identical to this one, but with a different ID.
#--- Parameters: ---
# $id: A new ID to apply to the copied element.
#--- Returns: ---
# An element identical to this one, except for the new ID value. 
#==============================================================================#

#==============================================================================#
#--- withCategory: ---
# Gets an element identical to this one, but with a different category.
#--- Parameters: ---
# $category: A new category to apply to the copied element.
#--- Returns: ---
# An element identical to this one, except for the new category value. 
#==============================================================================#

#==============================================================================#
#--- withKey: ---
# Gets an element identical to this one, but with a different JSON key.
#--- Parameters: ---
# $key: A new JSON key to apply to the copied element.
#--- Returns: ---
# An element identical to this one, except for the new key value. 
#==============================================================================#

#==============================================================================#
#--- withDefaultColour: ---
# Gets an element identical to this one, but with a different default colour
# value.
#--- Parameters: ---
# $colour: A new colour string to apply to the copied element.
#--- Returns: ---
# An element identical to this one, except for the new colour value. 
#==============================================================================#

use strict;
use warnings;

package Element;
use Scalar::Util qw(blessed);
use lib './project-scripts/ColourID/DataObjects';
use Category;

# Constructs a new element:
# namespace: The element's namespace name.
# name:      The element's object name.
# id:        The element's hex ID string.
# Category:  The element's UICategory object.
#
# return:    A new element object.
sub new
{
    my $class = shift;
    my $namespace = shift;
    my $name, my $id, my $category, my $key, my $colour;
    my $numParams = @_;
    if ($numParams == 1)
    {
        my $text = shift;
        if ($text =~ /
                    (?:Element\s+)?
                    (\w+)\s*\(\s*            # match object name
                    (0x[0-9a-fA-F]+),\s*     # match hex ID
                    UICategory:: (\w+)\s*\); # match category name
                /sx)
        {
            $name = $1;
            $id = $2;
            $category = $3;
        }
    }
    else
    {
        $name = shift;
        $id = shift;
        $category = shift;
        $key = shift;
        $colour = shift;
        if ($colour)
        {
            if (! ($colour =~ /^0x/i))
            {
                $colour = "0x$colour";
            }
            if ($colour =~ /^0x[0-9a-f]{8}$/i)
            {
                $colour = lc($colour);
            }
            else
            {
                print("Element::setDefaultColour: Default colour must be an"
                        ." eight digit hexadecimal number.\n");
                $colour = "";
            }
        }
    }
    if (!defined($category))
    {
        $category = Category::NONE;
    }
    $category = new Category($category);
    foreach my $optionalParam (\$colour, \$key)
    {
        if (!defined($$optionalParam))
        {
            $$optionalParam = "";
        }
    }
    if (defined($namespace) && defined($name) && defined($id)
            && defined($category))
    {
        my $self =
        {
            _namespace => $namespace,
            _name      => $name,
            _id        => $id,
            _category  => $category,
            _key       => $key,
            _colour    => $colour
        };
        bless($self, $class);
        return $self;
    }
    return undef;
}

# Gets the name of the namespace that contains the Element.
sub getNamespace
{
    my $self = shift;
    return $self->{_namespace};
}

# Gets the Element's colour value name.
sub getName
{
    my $self = shift;
    return $self->{_name};
}

# Gets the Element's ColourId value.
sub getID
{
    my $self = shift;
    return $self->{_id};
}

# Gets the element's Category enum object.
sub getCategory
{
    my $self = shift;
    return $self->{_category};
}

# Gets the element's optional JSON colour key.
sub getKey
{
    my $self = shift;
    return $self->{_key};
}

# Gets the colour value assigned to the element in the default colours.json
# file.
sub getDefaultColour
{
    my $self = shift;
    return $self->{_colour};
}

# Sets the colour value assigned to the element in the default colours.json
# file.
sub setDefaultColour
{
    my ($self, $colour) = @_;
    if (! ($colour =~ /^0x/i))
    {
        $colour = "0x$colour";
    }
    if ($colour =~ /^0x[0-9a-f]{8}$/i)
    {
        $self->{_colour} = lc($colour);
    }
    else
    {
        print("Element::setDefaultColour: Default colour must be an eight "
                ."digit hexadecimal number.\n");
    }
}

# Gets the full name used to identify the Element in C++.
sub getFullName
{
    my $self = shift;
    return $self->getNamespace()."::".$self->getName();
}

# Gets a C++ declaration for the Element as a Theme::Colour::Element constant.
sub getDeclaration
{
    my ($self, $indent) = @_;
    if (defined($indent))
    {
        $indent = " " x ($indent*4);
    }
    else
    {
        $indent = "";
    }
    return $indent."static const Element ".$self->getName()."(\n"
            .$indent.(" " x 8).$self->getID()
            .", UICategory::".$self->getCategory()->getTypeName().");";
}

# Gets a value that may be used when sorting Elements by ID.
sub sortValue
{
    my $self = shift;
    return hex($self->getID() || -1);
}

# Gets an element identical to this one, but with a different namespace.
sub withNamespace
{
    my ($self, $namespaceName) = @_;
    return new Element($namespaceName,
            $self->getName(),
            $self->getID(),
            $self->getCategory(),
            $self->getKey(),
            $self->getDefaultColour());
}

# Gets an element identical to this one, but with a different name.
sub withName
{
    my ($self, $name) = @_;
    return new Element($self->getNamespace(),
            $name,
            $self->getID(),
            $self->getCategory(),
            $self->getKey(),
            $self->getDefaultColour());
}

# Gets an element identical to this one, but with a different ID.
sub withID
{
    my ($self, $id) = @_;
    return new Element($self->getNamespace(),
            $self->getName(),
            $id,
            $self->getCategory(),
            $self->getKey(),
            $self->getDefaultColour());
}

# Gets an element identical to this one, but with a different Category.
sub withCategory
{
    my ($self, $category) = @_;
    return new Element($self->getNamespace(),
            $self->getName(),
            $self->getID(),
            $category,
            $self->getKey(),
            $self->getDefaultColour());
}

# Gets an element identical to this one, but with a different JSON key.
sub withKey
{
    my ($self, $key) = @_;
    return new Element($self->getNamespace(),
            $self->getName(),
            $self->getID(),
            $self->getCategory(),
            $key,
            $self->getDefaultColour());
}

# Gets an element identical to this one, but with a different default colour.
sub withDefaultColour
{
    my ($self, $colour) = @_;
    return new Element($self->getNamespace(),
            $self->getName(),
            $self->getID(),
            $self->getCategory(),
            $self->getKey(),
            $colour);
}
1;
