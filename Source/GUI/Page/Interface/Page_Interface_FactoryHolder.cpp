#include "Page_Interface_FactoryHolder.h"
#include "Page_Interface_Factory.h"

// Gets the factory holder's interface to the page factory.
Page::Interface::Factory* Page::Interface::FactoryHolder::getFactoryInterface()
{
    return factoryInterface;
}


// Sets the factory holder's factory interface pointer.
void Page::Interface::FactoryHolder::setFactoryInterface(Factory* pageFactory)
{
    factoryInterface = pageFactory;
}
