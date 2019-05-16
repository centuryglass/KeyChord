#pragma once
/**
 * @file  Page_Interface_FactoryHolder.h
 *
 * @brief  Allows the page factory to connect itself to page component objects.
 */

namespace Page
{
    namespace Interface
    {
        class Factory;
        class FactoryHolder;
    }
    class Factory;
}
/**
 * @brief  Creates the connection between the page and its factory.
 */
class Page::Interface::FactoryHolder
{
public:
    FactoryHolder() { }

    virtual ~FactoryHolder() { }

protected:
    /**
     * @brief  Gets the factory holder's interface to the page factory.
     *
     * @return   The factory interface pointer, or nullptr if the factory
     *           holder was not created by a Page::Factory.
     */
    Factory* getFactoryInterface();

private:
    friend Page::Factory;

    /**
     * @brief  Sets the factory holder's factory interface pointer.
     *
     *  The Page::Factory must use this function to pass itself to each
     * FactoryHolder it creates that needs to access the factory.
     *
     * @param pageFactory  The factory interface pointer to set.
     */
    void setFactoryInterface(Factory* pageFactory);

    Factory* factoryInterface = nullptr;
};
