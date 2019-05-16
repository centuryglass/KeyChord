##### Namespace.pm #############################################################
# Represents an inner ColourId namespace containing Element object definitions.#
################################################################################

#==============================================================================#
#--- new: ---
# Constructs a new Namespace object.
#--- Parameters: ---
# $name: The name of the new Namespace.
#--- Returns: ---
# The new object.
#==============================================================================#

#==============================================================================#
#--- getName: ---
# Gets the Namespace object's name.
#--- Returns: ---
# The name value set on construction.
#==============================================================================#

#==============================================================================#
#--- sortElements: ---
# Sorts the Namespace's internal Element list by ID value.
#==============================================================================#

#==============================================================================#
#--- addElement: ---
# Inserts an element into its sorted position in the Namespace.
#--- Parameters: ---
# $element: An Element object that belongs in this Namespace that the Namespace
#           will hold.
#==============================================================================#

#==============================================================================#
#--- removeElement: ---
# Removes an element from the Namespace.
#--- Parameters: ---
# $id: An Element ID used to select the element that should be removed.
#==============================================================================#

#==============================================================================#
#--- getElements: ---
# Gets all Element objects in the Namespace.
#--- Returns: ---
# All stored Element objects, sorted by ID.
#==============================================================================#

#==============================================================================#
#--- getFirstID: ---
# Gets the ID value of the first Element in the Namespace.
#--- Returns: ---
# The ID as a number, or -1 if the Namespace is empty.
#==============================================================================#

#==============================================================================#
#--- getDeclarationText: ---
# Creates a C++ declaration of this object's namespace and all Element objects
# it contains.
#--- Returns: ---
# The full C++ declaration text.
#==============================================================================#

use strict;
use warnings;

package Namespace;
use lib './project-scripts/ColourID';
use Element;
use Scalar::Util qw(blessed);

# Constructs a new namespace:
sub new
{
    my $class = shift;
    my $self =
    {
        _name    => shift,
        _elements => []
    };
    bless($self, $class);
    return $self;
}

# Gets the Namespace object's name.
sub getName
{
    my $self = shift;
    return $self->{_name};
}

# Sorts the Namespace's internal Element list by ID value.
sub sortElements
{
    my $self = shift;
    @{$self->{_elements}} = sort {hex($a->getID()) <=> hex($b->getID())}
            @{$self->{_elements}};
}

# Inserts an element into its sorted position in the Namespace.
sub addElement
{
    my $self = shift;
    my $element = shift;
    if (blessed($element) && (blessed($element) eq 'Element')
        && ($element->getNamespace() eq $self->getName()))
    {
        push(@{$self->{_elements}}, $element);
        $self->sortElements();
    }
    else
    {
        die "Invalid element $element";
    }
}

# Removes an element from the Namespace.
sub removeElement
{
    my $self = shift;
    my $id = shift;
    if (!$id)
    {
        print("Namespace::removeElement: missing ID parameter\n");
        return;
    }
    my $numElements = @{$self->{_elements}};
    for (my $i = 0; $i < $numElements; $i++)
    {
        my $element = $self->{_elements}->[$i];
        if ($element && ($element->getID() eq $id))
        {
            splice(@{$self->{_elements}}, $i, 1);
            $self->sortElements();
            return;
        }
    }
}

# Gets all Element objects in the namespace
sub getElements
{
    my $self = shift;
    return @{$self->{_elements}};
}

# Gets the ID value of the first Element in the namespace
# Return: The ID as a number, or -1 if the namespace is empty
sub getFirstID
{
    my $self = shift;
    if ( (scalar @{$self->{_elements}}) == 0)
    {
        return -1;
    }
    return hex(${$self->{_elements}}[0]->getID());
}

# Creates a C++ declaration of this object's namespace and all Element objects
# it contains.
sub getDeclarationText
{
    my $self = shift;
    my $decl = "namespace ".$self->getName()."\n{\n";
    my @elements = $self->getElements();
    foreach my $element (@elements)
    {
        $decl = $decl.$element->getDeclaration(1)."\n";
    }
    return $decl."}\n";
}
1;
