#include "SharedResource_Test_ModuleTestClasses.h"
#include <iostream>

const juce::Identifier TestResource::resourceKey = "Test_Modular_Resource";

TestModuleA::TestModuleA(TestResource& resource):
TestModule<juce::String>(resource, defaultValueA)
{
    std::cout << "TestModuleA created.\n";
}

TestModuleB::TestModuleB(TestResource& resource):
TestModule<double>(resource, defaultValueB)
{
    std::cout << "TestModuleB created.\n";
}

TestModuleC::TestModuleC(TestResource& resource):
TestModule<int>(resource, defaultValueC)
{
    std::cout << "TestModuleC created.\n";
}

juce::String TestModuleC::getAValue() const
{
    const TestModuleA* moduleA = getConstSiblingModule<TestModuleA>();
    if (moduleA == nullptr)
    {
        std::cout<< "C::getA() returned null!";
    }
    return moduleA->getValue();
}

double TestModuleC::getBValue() const
{
    const TestModuleB* moduleB = getConstSiblingModule<TestModuleB>();
    if (moduleB == nullptr)
    {
        std::cout<< "C::getB() returned null!";
    }
    return moduleB->getValue();
}

int TestModuleC::actOnAllHandlers()
{
    int count = actOnHandlers();
    count += getSiblingModule<TestModuleA>()->actOnHandlers();
    count += getSiblingModule<TestModuleB>()->actOnHandlers();
    return count;
}

juce::String TestHandlerC::getAValue() const
{
    SharedResource::Modular::LockedPtr<TestResource, const TestModuleC> testC
            = getReadLockedResource();
    return testC->getAValue();
}

double TestHandlerC::getBValue() const
{
    SharedResource::Modular::LockedPtr<TestResource, const TestModuleC> testC
            = getReadLockedResource();
    return testC->getBValue();
}

int TestHandlerC::actOnAllHandlers()
{
    SharedResource::Modular::LockedPtr<TestResource, TestModuleC> testC
            = getWriteLockedResource();
    return testC->actOnAllHandlers();
}

TestResource::TestResource() : SharedResource::Modular::Resource<>(resourceKey)
{
    std::cout << "TestResource created\n";
    moduleA = new TestModuleA(*this);
    moduleB = new TestModuleB(*this);
    moduleC = new TestModuleC(*this);
}

TestResource::~TestResource()
{
    delete moduleA;
    delete moduleB;
    delete moduleC;
}

namespace SharedResource { namespace Modular {

template<> template<> TestModuleA*
SharedResource::Modular::Resource<>::getModule<TestModuleA>()
{
    TestResource* thisResource = static_cast<TestResource*>(this);
    return thisResource->moduleA;
}

template<> template<> TestModuleB*
SharedResource::Modular::Resource<>::getModule<TestModuleB>()
{
    TestResource* thisResource = static_cast<TestResource*>(this);
    return thisResource->moduleB;
}


template<> template<> TestModuleC*
SharedResource::Modular::Resource<>::getModule<TestModuleC>()
{
    TestResource* thisResource = static_cast<TestResource*>(this);
    return thisResource->moduleC;
}

template<> template<> const TestModuleA*
SharedResource::Modular::Resource<>::getModule<const TestModuleA>()
{
    TestResource* thisResource = static_cast<TestResource*>(this);
    return thisResource->moduleA;
}

template<> template<> const TestModuleB*
SharedResource::Modular::Resource<>::getModule<const TestModuleB>()
{
    TestResource* thisResource = static_cast<TestResource*>(this);
    return thisResource->moduleB;
}

template<> template<> const TestModuleC*
SharedResource::Modular::Resource<>::getModule<const TestModuleC>()
{
    TestResource* thisResource = static_cast<TestResource*>(this);
    return thisResource->moduleC;
}

} }
