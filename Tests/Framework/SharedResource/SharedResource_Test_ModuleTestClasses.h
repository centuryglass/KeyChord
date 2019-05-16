#pragma once
/**
 * @file  Test_SharedResource_Modular_TestClasses.h
 *
 * @brief  Defines a SharedResource::Modular::Resource, three resource Module
 *         classes, and corresponding module Handler classes with a few basic
 *         function useful for testing.
 */

#include "SharedResource_Modular_Resource.h"
#include "SharedResource_Modular_Module.h"
#include "SharedResource_Modular_Handler.h"
#include "JuceHeader.h"

// Modular resource class:
class TestResource;

// Resource module classes:
class TestModuleA;
class TestModuleB;
class TestModuleC;

// Module handler classes:
class TestHandlerA;
class TestHandlerB;
class TestHandlerC;

// Default stored test values:
static const constexpr char* defaultValueA = "Module A";
static const constexpr double defaultValueB = 0.5;
static const constexpr int defaultValueC = 2;

/**
 * @brief  The modular resource instance class used for testing.
 */
class TestResource : public SharedResource::Modular::Resource<>
{
public:
    static const juce::Identifier resourceKey;

    TestResource();

    virtual ~TestResource();

private:
    // Allow getModule() specializations to access resource modules:
    template<class ModuleType>
    friend ModuleType* SharedResource::Modular::Resource<>::getModule();

    TestModuleA* moduleA;
    TestModuleB* moduleB;
    TestModuleC* moduleC;
};

/**
 * @brief  A generic Module class template used to implement all test modules.
 *
 * @tparam testValueType  The type of value the module stores for testing.
 */
template<typename testValueType>
class TestModule : public SharedResource::Modular::Module<TestResource>
{
public:
    TestModule(TestResource& resource, testValueType defaultValue):
    SharedResource::Modular::Module<TestResource>::Module(resource),
    value(defaultValue) { }

    class Interface
    {
    public:
        virtual juce::String getName() = 0;
    };

    testValueType getValue() const
    {
        return value;
    }

    void setValue(testValueType newValue)
    {
        value = newValue;
    }

    int actOnHandlers()
    {
        int count = 0;
        juce::String handlerName;
        foreachHandler<Interface>([&count, &handlerName](Interface* handler)
        {
            count++;
            if (!handlerName.isEmpty())
            {
                jassert(handlerName == handler->getName());
            }
            else
            {
                handlerName = handler->getName();
            }
        });
        return count;
    }

private:
    testValueType value;
};

/**
 * @brief  A generic module handler class used to implement all test handlers.
 *
 * @tparam ModuleType      The module class this Handler accesses.
 *
 * @tparam testValueType   The type of test value the Handler's Module uses
 */
template<class ModuleType, typename testValueType>
class TestHandler :
    public SharedResource::Modular::Handler<TestResource, ModuleType>,
    public TestModule<testValueType>::Interface
{
public:
    using SharedResource::Modular::Handler<TestResource, ModuleType>::Handler;

    testValueType getValue() const
    {
        using namespace SharedResource::Modular;
        LockedPtr<TestResource, const ModuleType> module
                = Handler<TestResource, ModuleType>::getReadLockedResource();
        return module->getValue();
    }

    void setValue(testValueType newValue)
    {
        using namespace SharedResource::Modular;
        LockedPtr<TestResource, ModuleType> module
                = Handler<TestResource, ModuleType>::getWriteLockedResource();
        module->setValue(newValue);
    }

    int actOnHandlers()
    {
        using namespace SharedResource::Modular;
        LockedPtr<TestResource, ModuleType> module
                = Handler<TestResource, ModuleType>::getWriteLockedResource();
        return module->actOnHandlers();
    }
};

// Test module implementations:
class TestModuleA : public TestModule<juce::String>
{
public:
    TestModuleA(TestResource& resource);
};

class TestModuleB : public TestModule<double>
{
public:
    TestModuleB(TestResource& resource);
};

class TestModuleC : public TestModule<int>
{
public:
    TestModuleC(TestResource& resource);

    juce::String getAValue() const;

    double getBValue() const;

    int actOnAllHandlers();
};

// Test handler implementations:
class TestHandlerA : public TestHandler<TestModuleA, juce::String>
{
public:
    virtual juce::String getName() override { return "HandlerA"; }
};

class TestHandlerB : public TestHandler<TestModuleB, double>
{
public:
    virtual juce::String getName() override { return "HandlerB"; }
};

class TestHandlerC : public TestHandler<TestModuleC, int>
{
public:
    virtual juce::String getName() override { return "HandlerC"; }

    juce::String getAValue() const;

    double getBValue() const;

    int actOnAllHandlers();
};

// getModule template specialization declarations:
namespace SharedResource { namespace Modular {

template<> template<> TestModuleA*
SharedResource::Modular::Resource<>::getModule<TestModuleA>();

template<> template<> TestModuleB*
SharedResource::Modular::Resource<>::getModule<TestModuleB>();

template<> template<> TestModuleC*
SharedResource::Modular::Resource<>::getModule<TestModuleC>();

template<> template<> const TestModuleA*
SharedResource::Modular::Resource<>::getModule<const TestModuleA>();

template<> template<> const TestModuleB*
SharedResource::Modular::Resource<>::getModule<const TestModuleB>();

template<> template<> const TestModuleC*
SharedResource::Modular::Resource<>::getModule<const TestModuleC>();

} }
