##### IDCache.pm ###############################################################
# Stores a set of cached ColourId Element Namespace objects.                   #
################################################################################

##### Functions: #####

#==============================================================================#
#--- new: ---
# Constructs a new IDCache object.
#--- Returns: ---
# An empty cache object.
#==============================================================================#

#==============================================================================#
#--- addElement: ---
# Adds an Element object to the cache.
#--- Parameters: ---
# $element: A new valid Element object to store. If this element's namespace
#           name is not in the cache already, a new Namespace object will also
#           be created to represent it and stored in the cache.
#==============================================================================#

#==============================================================================#
#--- removeElement: ---
# Removes an element from the cache.
#--- Parameters: ---
# $element: The Element to remove, or a search value the findElement function
#           can use to locate the cached Element.
#==============================================================================#

#==============================================================================#
#--- assignKey: ---
# Assigns a JSON colour value key string to an Element.
#--- Parameters: ---
# $searchValue: The full name, current key, or hex ColourId value of the element
#               being updated.
#
# $key:         The new key to assign to the matching element.
#==============================================================================#

#==============================================================================#
#--- findElement: ---
# Finds a cached Element objects.
#--- Parameters: ---
# @searchValues: The full name, current key, or hex ColourId value of the
#                Element that should be found.
#--- Returns: ---
# The requested Element, or undef if the search value that did not match a
# cached Element.
#==============================================================================#

#==============================================================================#
#--- findNamespace: ---
# Finds a cached Namespace object.
#--- Parameters: ---
# $name: The name of the requested Namespace.
#--- Returns: ---
# The matching namespace object, or undef if no match was found
#==============================================================================#

#==============================================================================#
#--- hasID: ---
# Checks if the cache contains an element with a specific hex ID string.
#--- Parameters: ---
# $id: The hexadecimal ID string value to search for.
#--- Returns: ---
# Whether an Element with the given ID is already cached.
#==============================================================================#

#==============================================================================#
#--- SortValue: ---
# Gets the value used to sort an Element by ID.
#--- Parameters: ---
# $searchValue: A string used to find a cached Element.
#--- Returns: ---
# The matching Element's ID sort value, or -1 if no match was found.
#==============================================================================#

#==============================================================================#
#--- getNamespaceNames: ---
# Gets the list of Namespace object names, sorted by ID of first held Element.
#--- Returns: ---
# The names of all of the Namespace objects present in the cache.
#==============================================================================#

#==============================================================================#
#--- getElements: ---
# Gets all cached Element objects, sorted by Element ID value.
#--- Returns: ---
# The full list of all cached Element objects.
#==============================================================================#

#==============================================================================#
#--- getElementNames: ---
# Gets the list of all Element full names, sorted by Element ID value.
#--- Returns: ---
# The full names of all cached Element objects.
#==============================================================================#

#==============================================================================#
#--- getElementKeys: ---
# Gets the full list of all Element JSON colour key strings, sorted by Element
# ID value.
#--- Returns: ---
# The array of all non-empty Element key values.
#==============================================================================#

use strict;
use warnings;

package IDCache;
use lib './project-scripts/ColourID';
use Element;
use Namespace;
use Scalar::Util qw(blessed);

# Constructs a new IDCache object.
sub new
{
    my $class = shift;
    my $self =
    {
        _namespaces => {}, # Store namespace objects by name
        _elements   => {}, # Store element objects by name
        _keys       => {}, # Store element objects by key
        _ids        => {}  # Store element objects by ID
    };
    bless($self, $class);
    return $self;
}

# Adds an Element object to the cache.
sub addElement
{
    my $self = shift;
    my $element = shift;
    if (!blessed($element) || blessed($element) ne 'Element')
    {
        die "Invalid Element param $element!\n";
    }
    $self->{_elements}->{$element->getFullName()} = $element;
    # If an existing element is being replaced, make sure to remove it from
    # everywhere in the cache.
    my $id = $element->getID();
    my $replacedElement = $self->{_ids}->{$id};
    if (defined($replacedElement))
    {
        $self->removeElement($replacedElement);
    }
    $self->{_ids}->{$id} = $element;

    my $nsName = $element->getNamespace();
    my $namespace = $self->{_namespaces}->{$nsName};
    if (!defined($namespace))
    {
        $namespace = new Namespace($nsName);
        $self->{_namespaces}->{$nsName} = $namespace;
    }
    $namespace->addElement($element);
    my $key = $element->getKey();
    if ($key)
    {
        $self->{_keys}->{$key} = $element;
    }
}

# Removes an element from the cache.
sub removeElement
{
    my $self = shift;
    my $element = shift;
    if (defined($element) && !blessed($element))
    {
        $element = $self->findElement($element);
    }
    if (blessed($element) && (blessed($element) eq 'Element'))
    {
        my $namespaceName = $element->getNamespace();
        my $namespace = $self->findNamespace($namespaceName);
        $namespace->removeElement($element->getID());
        if ( (scalar $namespace->getElements()) == 0)
        {
            delete($self->{_namespaces}->{$namespaceName});
        }
        delete($self->{_elements}->{$element->getFullName()});
        my $key = $element->getKey();
        if ($key)
        {
            delete($self->{_keys}->{$key});
        }
        delete($self->{_ids}->{$element->getID()});
    }
}

# Assigns a JSON colour value key string to an Element.
sub assignKey
{
    my $self = shift;
    my $searchVal = shift;
    my $key = shift;

    if (exists($self->{_keys}->{$key}))
    {
        print("IDCache::assignKey: key \"$key\" is already in use!\n");
        return;
    }
    my $element = $self->findElement($searchVal);
    if (defined($element))
    {
        my $replacement = $element->withKey($key);
        $self->removeElement($element);
        $self->addElement($replacement);
    }
    else
    {
        print("IDCache::assignKey: failed to find element $searchVal\n");
    }
}

# Finds a cached Element object.
sub findElement
{
    my $self = shift;
    my $searchValue = shift;
    my $element;
    if (exists($self->{_elements}->{$searchValue}))
    {
        $element = $self->{_elements}->{$searchValue};
    }
    elsif (exists($self->{_keys}->{$searchValue}))
    {
        $element = $self->{_keys}->{$searchValue};
    }
    elsif (exists($self->{_ids}->{$searchValue}))
    {
        $element = $self->{_ids}->{$searchValue};
    }
    return $element;
}

# Finds a cached Namespace object.
sub findNamespace
{
    my $self = shift;
    my $name = shift;
    if (exists($self->{_namespaces}->{$name}))
    {
        return $self->{_namespaces}->{$name};
    }
    return undef;
}

# Checks if the cache contains an element with a specific hex ID string.
sub hasID
{
    my $self = shift;
    my $id = shift;
    return defined($id) && exists($self->{_ids}->{$id})
            && defined($self->{_ids}->{$id});
}

# Gets the value used to sort an Element by ID.
sub sortValue
{
    my $self = shift;
    my $searchValue = shift;
    my $element = $self->findElement($searchValue);
    if (!defined($element))
    {
        return -1;
    }
    return $element->sortValue();
}

# Gets the list of Namespace object names, sorted by ID of first held Element.
sub getNamespaceNames
{
    my $self = shift;
    my @names = keys(%{$self->{_namespaces}});
    sub sortVal
    {
        my $name = shift;
        my $self = shift;
        return $self->{_namespaces}->{$name}->getFirstID();
    }
    return sort {sortVal($a, $self) <=> sortVal($b, $self)} @names;
}

# Gets a stored Namepace object.
# name: The name of the object to find.
sub getNamespace
{
    my $self = shift;
    my $name = shift;
    if (exists($self->{_namespaces}->{$name}))
    {
        return $self->{_namespaces}->{$name};
    }
    return undef;
}

# Gets all cached Element objects, sorted by Element ID value.
sub getElements
{
    my $self = shift;
    my @names = $self->getElementNames();
    my @elements;
    foreach my $name (@names)
    {
        push(@elements, $self->findElement($name));
    }
    return @elements;
}

# Gets the list of all Element full names, sorted by Element ID value.
sub getElementNames
{
    my $self = shift;
    my @names = keys(%{$self->{_elements}});
    return sort {$self->sortValue($a) <=> $self->sortValue($b)} @names;
}

# Gets the full list of all Element JSON colour key strings, sorted by Element
# ID value.
sub getElementKeys
{
    my $self = shift;
    my @names = keys(%{$self->{_keys}});
    return sort {$self->sortValue($a) <=> $self->sortValue($b)} @names;
}
1;
