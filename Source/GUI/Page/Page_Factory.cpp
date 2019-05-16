#include "Page_Factory.h"
#include "Page_Type.h"
#include "HomePage.h"

// Creates an instance of the application's home page.
Page::Component* Page::Factory::createHomePage()
{
    Component* homePage = new HomePage;
    homePage->setFactoryInterface(this);
    return homePage;
}


// Creates a new page to push on top of the page stack.
Page::Component* Page::Factory::createPage(const Type pageType)
{
    Component* newPage = nullptr;
    switch(pageType)
    {
        case Type::exampleType:
            newPage = nullptr; // = new YourType;
    }
    return newPage;
}
