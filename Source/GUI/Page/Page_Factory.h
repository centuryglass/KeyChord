#pragma once
/**
 * @file  PageFactory.h
 *
 * @brief  Creates all page components.
 */

#include "Page_Interface_Factory.h"
#include <functional>

namespace Page
{
    class Factory;
    class Component;
    enum class Type;
}

/**
 * @brief  Creates all new Page::Component objects, and allows pages to create
 *         other pages.
 */
class Page::Factory : public Page::Interface::Factory
{
public:
    Factory() { }

    virtual ~Factory() { }

    /**
     * @brief  Creates an instance of the application's home page.
     *
     * @return  A pointer to the new home page.
     */
    Page::Component* createHomePage();

private:
    /**
     * @brief  Creates a new page to push on top of the page stack.
     *
     * @param PageType  The type of page to create.
     */
    virtual Page::Component* createPage(const Type pageType) override;
};
