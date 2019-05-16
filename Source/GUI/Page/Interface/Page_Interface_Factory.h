#pragma once
/**
 * @file  Page_Interface_Factory.h
 *
 * @brief  Allows individual page components to create new pages.
 */

namespace Page
{
    namespace Interface
    {
        class Factory;
    }
    class Component;
    enum class Type;
}

/**
 * @brief  The Page::Component's interface to the page factory.
 */
class Page::Interface::Factory
{
public:
    Factory() { }

    virtual ~Factory() { }

protected:
    friend Page::Component;

    /**
     * @brief  Creates a new page to push on top of the page stack.
     *
     * @param PageType  The type of page to create.
     */
    virtual Component* createPage(const Type pageType) = 0;
};
