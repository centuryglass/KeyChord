#include "Debug_AddressLog.h"
#include <map>

/**
 * @brief  Gets the map used to link each logged address value to its unique ID
 *         number.
 *
 * @return  The address ID map.
 */
static std::map<const void*, int>& getAddressMap()
{
    static std::map<const void*, int> ids;
    return ids;
}


// Gets a unique, fixed ID value to represent a particular memory address.
int Debug::AddressLog::getID(const void* address)
{
    static int nextID = 0;
    std::map<const void*, int>& ids = getAddressMap();
    if (nextID == 0)
    {
        ids[0] = 0;
        nextID++;
    }
    if (ids.count(address) == 0)
    {
        ids[address] = nextID;
        nextID++;
    }
    return ids[address];
}


// Appends a line of text to the log of events occurring to a specific address.
const juce::String& Debug::AddressLog::addEvent
(const void* address, const juce::String event, const void* address2)
{
    static juce::CriticalSection logSection;
    static std::map<int, juce::String> eventLog;
    const juce::ScopedLock eventLock(logSection);
    if (address == nullptr)
    {
        return eventLog[0];
    }
    int id = getID(address);
    juce::String& log = eventLog[id];
    log += "\n";
    log += event;
    if (address2 != nullptr)
    {
        log += getID(address2);
    }
    return log;
}


// Prints all logged events for a specific memory address
void Debug::AddressLog::printLog(int addressID)
{
    const void* address = nullptr;
    std::map<const void*, int>& ids = getAddressMap();
    for (auto iter = ids.begin(); iter != ids.end(); iter++)
    {
        if (iter->second == addressID)
        {
            address = iter->first;
            break;
        }
    }
    const juce::String& log = addEvent(address, "printed log");
    std::cout << "\n\n\tPrinting log for " << addressID << ":\n"
            << log << "\n\n";
}
