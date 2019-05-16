#include "Layout_Group_RelativeLayout.h"

namespace GroupLayout = Layout::Group;

//#################### GroupLayout::RelativeLayout ############################

// Sets the size of the top and bottom margins of the layout using a weight
// value.
void GroupLayout::RelativeLayout::setYMarginWeights(const unsigned int weight)
{
    yMarginWeight = weight;
    yMarginFraction = 0;
}


// Sets the size of the left and right margins of the layout as a fraction of
// the total layout width.
void GroupLayout::RelativeLayout::setXMarginFraction(const float fraction)
{
    xMarginFraction = fraction;
}


// Sets the size of the top and bottom margins of the layout as a fraction of
// the total layout height.
void GroupLayout::RelativeLayout::setYMarginFraction(const float fraction)
{
    yMarginFraction = fraction;
    yMarginWeight = 0;
}


// Sets the fraction of the total layout width to leave empty between all row
// items with non-zero weights.
void GroupLayout::RelativeLayout::setXPaddingFraction(const float fraction)
{
    xPaddingFraction = fraction;
    xPaddingWeight = 0;
}


// Sets the fraction of the total layout height to leave empty between all
// layout rows with non-zero weights.
void GroupLayout::RelativeLayout::setYPaddingFraction(const float fraction)
{
    yPaddingFraction = fraction;
    yPaddingWeight = 0;
}


// Assigns a weight value to the amount of empty space left between each row
// item, ignoring row items with a zero weight value.
void GroupLayout::RelativeLayout::setXPaddingWeight(const unsigned int weight)
{
    xPaddingWeight = weight;
    xPaddingFraction = 0;
}


// Assigns a weight value to the amount of empty space left between each row,
// ignoring rows with a zero weight value.
void GroupLayout::RelativeLayout::setYPaddingWeight(const unsigned int weight)
{
    yPaddingWeight = weight;
    yPaddingFraction = 0;
}


// Gets the fraction of the width to allocate to each horizontal margin.
float GroupLayout::RelativeLayout::getXMarginFraction() const
{
    return xMarginFraction;
}


// Gets the fraction of the height to allocate to each vertical margin.
float GroupLayout::RelativeLayout::getYMarginFraction() const
{
    return yMarginFraction;
}


// Gets the weight value assigned to each vertical margin.
unsigned int GroupLayout::RelativeLayout::getYMarginWeight() const
{
    return yMarginWeight;
}


// Gets the fraction of the width to allocate to each space between RowItems.
float GroupLayout::RelativeLayout::getXPaddingFraction() const
{
    return xPaddingFraction;
}


// Gets the weight value assigned to each horizontal space between layout rows.
unsigned int GroupLayout::RelativeLayout::getXPaddingWeight() const
{
    return xPaddingWeight;
}


// Gets the fraction of the height to allocate to each space between layout
// rows.
float GroupLayout::RelativeLayout::getYPaddingFraction() const
{
    return yPaddingFraction;
}


// Gets the weight value assigned to each vertical space between layout rows.
unsigned int GroupLayout::RelativeLayout::getYPaddingWeight() const
{
    return yPaddingWeight;
}


// Gets the number of rows in the layout.
unsigned int GroupLayout::RelativeLayout::rowCount() const
{
    return rows.size();
}


// Checks if the layout contains any rows.
bool GroupLayout::RelativeLayout::isEmpty() const
{
    return rows.size() == 0;
}


// Gets a row in the layout.
const GroupLayout::Row& GroupLayout::RelativeLayout::getRow
(const unsigned int index) const
{
    return rows.at(index);
}


// Inserts a new row into the layout at a specific index.
void GroupLayout::RelativeLayout::insertRow
(const unsigned int index, const GroupLayout::Row row)
{
    int iterIdx = 0;
    auto rowIter = rows.begin();
    while (rowIter != rows.end() && iterIdx < index)
    {
        iterIdx++;
        rowIter++;
    }
    rows.insert(rowIter, row);
}


// Replaces a row in the layout.
void GroupLayout::RelativeLayout::setRow
(const unsigned int index, const GroupLayout::Row row)
{
    if (index == rows.size())
    {
        addRow(row);
    }
    else
    {
        rows.at(index) = row;
    }
}


// Adds a new row to the end of the layout.
void GroupLayout::RelativeLayout::addRow(const GroupLayout::Row row)
{
    rows.push_back(row);
}


// Removes a row from the layout.
void GroupLayout::RelativeLayout::removeRow(const unsigned int rowIndex,
        const bool removeComponentsFromParent)
{
    if (rowIndex < rows.size())
    {
        if (removeComponentsFromParent)
        {
            Row& toRemove = rows[rowIndex];
            for (int i = 0; i < toRemove.itemCount(); i++)
            {
                const RowItem& rowItem = toRemove.getRowItem(i);
                juce::Component* rowComponent = rowItem.getComponent();
                if (rowComponent != nullptr)
                {
                    juce::Component* parent
                            = rowComponent->getParentComponent();
                    if (parent != nullptr)
                    {
                        parent->removeChildComponent(rowComponent);
                    }
                }
            }
        }
        rows.erase(rows.begin() + rowIndex);
    }
}


// Checks if this layout and another are equivalent.
bool GroupLayout::RelativeLayout::operator== (const RelativeLayout& rhs) const
{
    if (yPaddingFraction != rhs.yPaddingFraction
            || yPaddingWeight   != rhs.yPaddingWeight
            || xPaddingFraction != rhs.xPaddingFraction
            || xPaddingWeight   != rhs.xPaddingWeight
            || yMarginFraction  != rhs.yMarginFraction
            || yMarginWeight    != rhs.yMarginWeight
            || xMarginFraction  != rhs.xMarginFraction
            || rows.size()      != rhs.rows.size())
    {
        return false;
    }
    for (int i = 0; i < rows.size(); i++)
    {
        if (rows[i] != rhs.rows[i])
        {
            return false;
        }
    }
    return true;
}


// Checks if this layout and another are not equivalent.
bool GroupLayout::RelativeLayout::operator!= (const RelativeLayout& rhs) const
{
    return ! (*this == rhs);
}


// Gets an iterator pointing to the first row in the layout.
GroupLayout::RelativeLayout::iterator GroupLayout::RelativeLayout::begin()
{
    return rows.begin();
}


// Gets an iterator pointing just after the last row in the layout.
GroupLayout::RelativeLayout::iterator GroupLayout::RelativeLayout::end()
{
    return rows.end();
}


// Gets a const iterator pointing to the first row in the layout.
GroupLayout::RelativeLayout::const_iterator
GroupLayout::RelativeLayout::begin() const
{
    return rows.begin();
}


// Gets a const iterator pointing just after the last row in the layout.
GroupLayout::RelativeLayout::const_iterator GroupLayout::RelativeLayout::end()
    const
{
    return rows.end();
}


//######################### GroupLayout::Row ##################################

// Adds a new RowItem to the end of this row.
void GroupLayout::Row::addRowItem(const GroupLayout::RowItem rowItem)
{
    rowItems.push_back(rowItem);
}


// Gets the number of rowItems in the row.
unsigned int GroupLayout::Row::itemCount() const
{
    return rowItems.size();
}


// Checks if this row is empty.
bool GroupLayout::Row::isEmpty() const
{
    return rowItems.empty();
}


// Gets a RowItem from the row.
const GroupLayout::RowItem& GroupLayout::Row::getRowItem
(const unsigned int index) const
{
    return rowItems.at(index);
}


// Gets the weight assigned to this row.
unsigned int GroupLayout::Row::getWeight() const
{
    return weight;
}


// Checks if this Row and another Row are equivalent.
bool GroupLayout::Row::operator== (const GroupLayout::Row& rhs) const
{
    if (weight != rhs.weight || rowItems.size() != rhs.rowItems.size())
    {
        return false;
    }
    for (int i = 0; i < rowItems.size(); i++)
    {
        if (rowItems[i] != rhs.rowItems[i])
        {
            return false;
        }
    }
    return true;
}


// Checks if this Row and another Row are not equivalent.
bool GroupLayout::Row::operator != (const GroupLayout::Row& rhs) const
{
    return ! (*this == rhs);
}


// Gets an iterator pointing to the first item in the row.
GroupLayout::Row::iterator GroupLayout::Row::begin()
{
    return rowItems.begin();
}


// Gets an iterator pointing just after the last item in the row.
GroupLayout::Row::iterator GroupLayout::Row::end()
{
    return rowItems.end();
}


// Gets a const iterator pointing to the first item in the row.
GroupLayout::Row::const_iterator GroupLayout::Row::begin() const
{
    return rowItems.begin();
}


// Gets a const iterator pointing just after the last item in the row.
GroupLayout::Row::const_iterator GroupLayout::Row::end() const
{
    return rowItems.end();
}


//######################## GroupLayout::RowItem ################################

// Gets the component assigned to this RowItem.
juce::Component* GroupLayout::RowItem::getComponent() const
{
    return component;
}


// Gets the horizontal weight value assigned to this RowItem.
unsigned int GroupLayout::RowItem::getWeight()const
{
    return weight;
}


// Checks if this row item represents an empty space.
bool GroupLayout::RowItem::isEmpty() const
{
    return component == nullptr;
}


// Checks if this RowItem and another are equivalent. RowItems are equivalent if
// they have the same weight and Component.
bool GroupLayout::RowItem::operator== (const GroupLayout::RowItem& rhs) const
{
    return component == rhs.component && weight == rhs.weight;
}


// Checks if this RowItem and another are not equivalent.
bool GroupLayout::RowItem::operator!= (const GroupLayout::RowItem& rhs) const
{
    return ! (*this == rhs);
}
