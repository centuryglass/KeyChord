#include "Process_Data.h"
#include "Process_State.h"

// Indices of process data members within the process stat file:
static const constexpr int idIndex        = 0;
static const constexpr int nameIndex      = 1;
static const constexpr int stateIndex     = 2;
static const constexpr int parentIdIndex  = 3;
static const constexpr int startTimeIndex = 21;

// Process stat file name before the pid directory:
static const juce::String pathPre = "/proc/";
// Process stat file name after the pid directory:
static const juce::String pathPost = "/stat";

// Reads process data from the system.
Process::Data::Data(const int processId) :
Data(juce::File(pathPre + juce::String(processId) + pathPost))
{
    // The parsed ID should always match the constructor ID if valid:
    jassert(!isValid() || this->processId == processId);
}


// Sorts processes by launch time, newest first.
class
{
public:
    static int compareElements(Process::Data first, Process::Data second)
    {
        return second.getStartTime() - first.getStartTime();
    }
} processComparator;

// Gets data for all direct child processes of the process this Data object
// represents.
juce::Array<Process::Data> Process::Data::getChildProcesses()
{
    using juce::File;
    using juce::Array;
    juce::File processDir("/proc");
    Array<File> childDirs
            = processDir.findChildFiles(File::findDirectories, false);
    Array<Data> childProcs;
    for (const File& dir : childDirs)
    {
        int childID = dir.getFileName().getIntValue();
        if (childID > 0)
        {
            Data processData(childID);
            if (processData.parentId == processId)
            {
                childProcs.add(processData);
            }
        }
    }
    childProcs.sort(processComparator);
    return childProcs;
}


// Checks whether this object found process data on construction.
bool Process::Data::isValid() const
{
    return lastState != State::invalid;
}


// Gets the ID of the process this Data object represents.
int Process::Data::getProcessId() const
{
    return processId;
}


// Gets the ID of the parent process of this Data process.
int Process::Data::getParentId() const
{
    return parentId;
}


// Gets the name of the executable this Data process was created to run.
juce::String Process::Data::getExecutableName() const
{
    return executableName;
}


// Gets the state of the Data process recorded when it was constructed.
Process::State Process::Data::getLastState() const
{
    return lastState;
}


// Gets the time this process was created.
juce::uint64 Process::Data::getStartTime() const
{
    return startTime;
}


// Create process data directly from the process stat file object.
Process::Data::Data(const juce::File statFile)
{
    if (statFile.existsAsFile())
    {
        // Parse process info from stat file strings:
        juce::StringArray statItems = juce::StringArray::fromTokens(
                statFile.loadFileAsString(), true);
        processId = statItems[idIndex].getIntValue();
        executableName = statItems[nameIndex].removeCharacters("()");
        parentId = statItems[parentIdIndex].getIntValue();
        // juce::String has no getUInt64 function, so time has to be parsed one
        // character at a time:
        startTime = 0;
        for (int i = 0; i < statItems[startTimeIndex].length(); i++)
        {
            startTime *= 10;
            startTime += (statItems[startTimeIndex][i] - '0');
        }
        lastState = readStateChar(statItems[stateIndex][0]);
    }
    else
    {
        lastState = State::invalid;
    }
}
