#include "Process_State.h"
#include "JuceHeader.h"

// Returns the process state represented by the state field character in the
// /proc/<pid>/stat file.
Process::State Process::readStateChar(const char stateCode)
{
    // TODO:  Why is state code 'C' included as a running process state? 'C'
    // does not appear to be a valid code according to the proc manpage.
    // I suspect it's probably a rare edge case or deprecated option I
    // found earlier, but I didn't document unusual cases well enough
    // when implementing process data reading.
    switch(stateCode)
    {
        case 'R':
        case 'C':
            return State::running;
        case 'S':
            return State::sleeping;
        case 'K': // Deprecated WakeKill state, mostly equivalent to diskSleep
        case 'P': // Deprecated Parked state, mostly equivalent to diskSleep
        case 'D':
            return State::diskSleep;
        case 'Z':
            return State::zombie;
            break;
        case 'T':
            return State::stopped;
            break;
        case 't':
            return State::tracingStop;
        case 'W':
            return State::paging;
        case 'X':
        case 'x':
            return State::dead;
    }
    return State::unknown;
}


// Gets a string representation of a process state for debugging.
juce::String Process::stateString(const Process::State state)
{
    switch(state)
    {
        case State::running:
            return "running";
        case State::sleeping:
            return "sleeping";
        case State::diskSleep:
            return "diskSleep";
        case State::zombie:
            return "zombie";
        case State::stopped:
            return "stopped";
        case State::tracingStop:
            return "tracingStop";
        case State::paging:
            return "paging";
        case State::dead:
            return "dead";
        case State::unknown:
            return "unknown";
        case State::invalid:
            return "invalid";
    }
    return "[unhandled State!]";
}
