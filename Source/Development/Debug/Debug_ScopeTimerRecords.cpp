#include "Debug_ScopeTimerRecords.h"
#include <map>
#include <stack>

enum class RecordType
{
    start,
    stop
};

struct RecordData
{
    juce::String scopeName;
    juce::uint32 time;
    RecordType recordType;
};

static juce::CriticalSection recordGuard;
static std::map<juce::Thread::ThreadID, juce::Array<RecordData>> timeRecords;


// Records the start of a ScopeTimer's measurement period.
void Debug::ScopeTimerRecords::addStartRecord(
        const juce::String scopeName,
        const juce::Thread::ThreadID threadID)
{
    const juce::ScopedLock recordLock(recordGuard);
    RecordData newRecord =
    {
        scopeName,
        juce::Time::getMillisecondCounter(),
        RecordType::start
    };
    timeRecords[threadID].add(newRecord);
}


// Records the end of a ScopeTimer's measurement period.
void Debug::ScopeTimerRecords::addStopRecord(const juce::String scopeName,
        const juce::Thread::ThreadID threadID)
{
    const juce::ScopedLock recordLock(recordGuard);
    RecordData newRecord =
    {
        scopeName,
        juce::Time::getMillisecondCounter(),
        RecordType::stop
    };
    timeRecords[threadID].add(newRecord);
}


/**
 * @brief  Processes record data to print nested scope record times in order.
 */
class RecordPrinter
{
public:
    /**
     * @brief  Create a RecordPrinter to print a single thread's timer records.
     *
     * @param recordsToPrint  All ScopeTimer data from a single thread.
     */
    RecordPrinter(juce::Array<RecordData>& recordsToPrint);

    /**
     * @brief  Compares RecordData items so that they may be sorted.
     *
     * @param first   The first of two records to compare.
     *
     * @param second  The second of two records to compare.
     *
     * @return        A value less than zero if the first record comes before
     *                the second, zero if both records start at the same time,
     *                and a value greater than zero if the second record comes
     *                before the first.
     */
    static int compareElements(RecordData first, RecordData second);

    /**
     * @brief  Prints all scope timer records in order, showing time between
     *         scopes and the total duration recorded by each timer.
     */
    void printRecords();

private:
    /**
     * @brief  Prints a single line of text within the record.
     *
     * @param line  The line that should be printed to the console.
     */
    void printRecordLine(const juce::String line);

    juce::Array<RecordData> records;
    std::stack<RecordData> openRecords;
    juce::uint32 lastPrintedTime = 0;
};


// Create a RecordPrinter to print a single thread's timer records.
RecordPrinter::RecordPrinter(juce::Array<RecordData>& recordsToPrint) :
    records(recordsToPrint) { }


/**
 * @brief  Gets a string representation of a duration in time.
 *
 * @param startTime  The start of the represented time period.
 *
 * @param endTime    The end of the represented time period.
 *
 * @return           A formatted string containing the difference between
 *                   startTime and endTime, labeled as a duration in
 *                   milliseconds.
 */
static juce::String durationString(const juce::int64 startTime,
        const juce::int64 endTime)
{
    juce::String duration = " (";
    duration << juce::String(endTime - startTime) << " ms)";
    return duration;
}


// Prints all scope timer records in order, showing time between scopes and the
// total duration recorded by each timer.
void RecordPrinter::printRecords()
{
    using std::cout;
    lastPrintedTime = 0;

    for (int i = 0; i < records.size(); i++)
    {
        RecordData record = records[i];
        if (record.recordType == RecordType::start)
        {
            juce::uint32 endTime = 0;
            int recursionCount = 0;
            for (int endIdx = i + 1; endIdx < records.size(); endIdx++)
            {
                RecordData nextRecord = records[endIdx];
                if (nextRecord.scopeName == record.scopeName)
                {
                    if (nextRecord.recordType == RecordType::start)
                    {
                        recursionCount++;
                    }
                    else
                    {
                        recursionCount--;
                        if (recursionCount < 0)
                        {
                            endTime = nextRecord.time;
                            break;
                        }
                    }
                }
            }
            jassert(endTime > 0);

            if (lastPrintedTime > 0 && record.time > lastPrintedTime)
            {
                printRecordLine("");
                printRecordLine(durationString(lastPrintedTime, record.time));
            }
            printRecordLine("");
            openRecords.push(record);
            juce::String newRecordTitle;
            newRecordTitle << record.scopeName << " "
                << durationString(record.time, endTime);
            printRecordLine(newRecordTitle);
            lastPrintedTime = record.time;
        }
        else
        {
            jassert(record.scopeName == openRecords.top().scopeName);
            if (record.time > lastPrintedTime
                    && records[i - 1].scopeName != record.scopeName)

            {
                printRecordLine("");
                printRecordLine(durationString(lastPrintedTime, record.time));
            }
            lastPrintedTime = record.time;
            openRecords.pop();
        }
    }
    jassert(openRecords.empty());
}


// Prints a single line of text within the record.
void RecordPrinter::printRecordLine(const juce::String line)
{
    std::cout << "  ";
    for (int i = 0; i < openRecords.size(); i++)
    {
        std::cout << "| ";
    }
    std::cout << line << "\n";
}


// Organizes and prints all records chronologically.
void Debug::ScopeTimerRecords::printRecords()
{
    using std::cout;
    const juce::ScopedLock recordLock(recordGuard);
    if (!timeRecords.empty())
    {
        cout << "\nPrinting timer records for " << timeRecords.size()
                << " threads:\n";
    }
    for (auto threadRecords : timeRecords)
    {
        cout << "\nThread " << threadRecords.first << ":\n";
        RecordPrinter threadRecordPrinter(threadRecords.second);
        threadRecordPrinter.printRecords();
    }
}
