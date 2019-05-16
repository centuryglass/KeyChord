#include "Page_Interface_Component.h"

// Gets the page component's interface to the page stack.
Page::Interface::Stack* Page::Interface::Component::getStackInterface()
{
    return stackInterface;
}


// Sets the page component's interface to the page stack.
void Page::Interface::Component::setStackInterface(Stack* stack)
{
    stackInterface = stack;
}
