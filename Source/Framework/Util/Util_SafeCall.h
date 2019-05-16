/**
 * @file  Util_SafeCall.h
 *
 * @brief  Provides a template function for safely running asynchronous calls
 *         on the JUCE message thread to update a component.
 */

#include "JuceHeader.h"

namespace Util { namespace SafeCall {

/**
 * @brief  Asynchronously runs a function call on a component pointer on the
 *         JUCE message thread unless the component is destroyed before the call
 *         can run.
 *
 * @tparam ComponentType  The juce::Component subclass of the component pointer.
 *
 * @param component       The component pointer that the call will update.
 *
 * @param call            A function that asynchronously updates the component.
 */
template <class ComponentType>
void callAsync
(ComponentType* component, const std::function<void(ComponentType*)> call)
{
    juce::Component::SafePointer<ComponentType> safePtr(component);
    juce::MessageManager::callAsync([safePtr, call]()
    {
        if (ComponentType* componentPtr = safePtr.getComponent())
        {
            call(componentPtr);
        }
    });
}

} }
