# Page Module Documentation
The Page module divides up the application's interface into separate pages, sets the layout of UI components within those pages, and provides controls to switch between pages.

#### [Page\::Component](../../Source/GUI/Page/Page_Component.h)
Component object represent a single visible UI page. They create the layout used to organize all items in the page, and provide an interface for opening new pages.

#### [Page\::StackComponent](../../Source/GUI/Page/Page_StackComponent.h)
The StackComponent object holds all open Page\::Component objects. It adds new pages to the application window, and removes and destroys old pages as they are closed. It also notifies page component objects whenever they're being added or revealed on the stack.

#### [Page\::Factory](../../Source/GUI/Page/Page_Factory.h)
The Factory object provides an interface that allows page Component objects to create and add other pages without being dependent on those other page classes.

## Page Types

#### [Page\::Type](../../Source/GUI/Page/Page_Type.h)
Type enumerates all kinds of Page\::Component object the page factory may create.


## Internal Interface Classes

#### [Page\::Interface\::Component](../../Source/GUI/Page/Interface/Page_Interface_Component.h)
This Component interface is implemented by Page\::Component, and is used by the Page\::StackComponent to send messages to page component objects.

#### [Page\::Interface\::Factory](../../Source/GUI/Page/Interface/Page_Interface_Factory.h)
The Factory interface is implemented by Page\::Factory, and is used by Page Component objects to get new Page Component objects from the Factory.

#### [Page\::Interface\::FactoryHolder](../../Source/GUI/Page/Interface/Page_Interface_FactoryHolder.h)
The FactoryHolder interface is implemented by Page\::Component, and used by the Factory object to create the connection between itself and new Page Component objects.

#### [Page\::Interface\::Stack](../../Source/GUI/Page/Interface/Page_Interface_Stack.h)
The Stack interface is implemented by Page\::StackComponent, and is used by Page Component objects to add and remove visible pages.
