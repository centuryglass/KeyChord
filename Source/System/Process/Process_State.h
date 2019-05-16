#pragma once
/**
 * @file  Process_State.h
 *
 * @brief  Lists all possible Linux process states.
 */

namespace juce { class String; }

namespace Process
{
    enum class State;

    /**
     * @brief  Returns the process state represented by the state field
     *         character in the /proc/<pid>/stat file.
     *
     * @param stateCode  A character used to represent a process state.
     *
     * @return           The represented state, or State::unknown if given an
     *                   unexpected character type.
     */
    State readStateChar(const char stateCode);

    /**
     * @brief  Gets a string representation of a process state for debugging.
     *
     * @param state  One of the possible Linux process states.
     *
     * @return       The process state's string representation.
     */
    juce::String stateString(const State state);
}

/**
 * @brief  All possible Linux process states.
 */
enum class Process::State
{
    running,     // R: actively executing instructions.
    sleeping,    // S: waiting for an event or available time slot.
    diskSleep,   // D: waiting for a resource or timeout.
    stopped,     // T: finished, sending SIGCHLD to its parent process.
    zombie,      // Z: waiting to be removed by its parent process.
    tracingStop, // t: stopped by a debugger.
    paging,      // W: waiting for paged memory(only before 2.6.xx kernel).
    dead,        // X: finished and removed from the process table.
    idle,        // I: a kernel thread waiting in the TASK_IDLE state.
    // Error states:
    unknown, // Process exists, but its state can't be determined.
    invalid  // Not a process.
};
