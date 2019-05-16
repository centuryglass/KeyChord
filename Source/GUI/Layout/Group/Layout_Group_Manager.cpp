#include "Layout_Group_Manager.h"
#include "Layout_Transition_Animator.h"
#include <map>


// Gets the current component layout held by this LayoutManager.
Layout::Group::RelativeLayout Layout::Group::Manager::getLayout() const
{
    return layout;
}


// Set a new Component layout, removing all old layout definitions.
void Layout::Group::Manager::setLayout
(const RelativeLayout& layout, juce::Component* parentToInit)
{
    clearLayout();
    this->layout = layout;
    int yMarginWeights = layout.getYMarginWeight() * 2;
    yWeightSum = yMarginWeights;
    int rowNum = 0;

    for (int rowNum = 0; rowNum < layout.rowCount(); rowNum++)
    {
        const Row& rowLayout = layout.getRow(rowNum);
        xWeightSums.set(rowNum, 0);
        if (rowLayout.getWeight() > 0)
        {
            // Add padding weights between rows with non-zero weight
            if (yWeightSum > yMarginWeights)
            {
                yWeightSum += layout.getYPaddingWeight();
            }
            yWeightSum += rowLayout.getWeight();
        }
        unsigned int& rowWeightSum = xWeightSums.getReference(rowNum);

        for (const RowItem& rowItem : rowLayout)
        {
            if (rowItem.getWeight() > 0)
            {
                // Add padding weights between row items with non-zero weight
                if (rowWeightSum > 0)
                {
                    rowWeightSum += layout.getXPaddingWeight();
                }
                rowWeightSum += rowItem.getWeight();
            }
            juce::Component* rowComponent = rowItem.getComponent();
            if (parentToInit != nullptr && rowComponent != nullptr
                    && (rowComponent->getParentComponent() != parentToInit))
            {
                parentToInit->addAndMakeVisible(rowComponent);
            }
        }
    }
}


// Changes the current layout and immediately applies the updated layout to all
// components in the layout, optionally animating the transition.
void Layout::Group::Manager::transitionLayout(
        const RelativeLayout& newLayout,
        juce::Component* parent,
        const Transition::Type transition,
        const unsigned int duration,
        const bool animateUnmoved)
{
    jassert(parent != nullptr); // Parent must be non-null!

    // Immediately swap layouts if no transition animation is needed:
    if (transition == Transition::Type::none || duration == 0)
    {
        clearLayout(true);
        setLayout(newLayout, parent);
        layoutComponents(parent->getLocalBounds());
        return;
    }

    // When animating, map all layout items, tracking which ones are in each
    // layout.
    struct ComponentInfo
    {
        bool inOldLayout = false;
        bool inNewLayout = false;
    };

    std::map<juce::Component*, ComponentInfo> layoutItems;

    for (const Row& layoutRow : newLayout)
    {
        for (const RowItem& layoutItem : layoutRow)
        {
            if (!layoutItem.isEmpty())
            {
                layoutItems[layoutItem.getComponent()].inNewLayout = true;
            }
        }
    }
    for (const Row& layoutRow : layout)
    {
        for (const RowItem& layoutItem : layoutRow)
        {
            if (!layoutItem.isEmpty())
            {
                juce::Component* component = layoutItem.getComponent();
                ComponentInfo& itemInfo = layoutItems[component];
                itemInfo.inOldLayout = true;

                // Transition out and remove items that aren't in the new list.
                if (!itemInfo.inNewLayout)
                {
                    Transition::Animator::transitionOut(component, transition,
                            duration, true);
                    parent->removeChildComponent(component);
                }
            }
        }
    }
    setLayout(newLayout, parent);
    BoundsGrid newBoundsGrid = getBoundsGrid(parent->getLocalBounds());
    for (int rowIndex = 0; rowIndex < layout.rowCount(); rowIndex++)
    {
        const Row& row = layout.getRow(rowIndex);
        for (int itemIndex = 0; itemIndex < row.itemCount(); itemIndex++)
        {
            const RowItem& rowItem = row.getRowItem(itemIndex);
            if (!rowItem.isEmpty())
            {
                juce::Component* component = rowItem.getComponent();
                const ComponentInfo& itemInfo = layoutItems[component];
                const juce::Rectangle<int>& newBounds
                        = newBoundsGrid.getReference(rowIndex)
                        .getReference(itemIndex);
                if (animateUnmoved || !itemInfo.inOldLayout
                        || (newBounds != component->getBounds()))
                {
                    if (itemInfo.inOldLayout)
                    {
                        Transition::Animator::transitionOut(component,
                                transition, duration, true);
                    }
                    Transition::Animator::transitionIn(component, transition,
                            newBounds, duration);
                }
            }
        }
    }
}


// Adds all components in the layout to a parent component, and makes them all
// visible.
void Layout::Group::Manager::addComponentsToParent(juce::Component* parent)
{
    for (int rNum = 0; rNum < layout.rowCount(); rNum++)
    {
        const Row& row = layout.getRow(rNum);
        for (int i = 0; i < row.itemCount(); i++)
        {
            const RowItem& rowItem = row.getRowItem(i);
            if (!rowItem.isEmpty())
            {
                parent->addAndMakeVisible(rowItem.getComponent());
            }
        }
    }
}


// Arranges the components within a bounding rectangle.
void Layout::Group::Manager::layoutComponents(
        const juce::Rectangle<int>& bounds,
        const Transition::Type transition,
        const unsigned int duration,
        const bool animateUnmoved)
{
    const BoundsGrid boundsGrid = getBoundsGrid(bounds);
    layoutComponents(boundsGrid, transition, duration, animateUnmoved);
}


// Remove all saved component layout parameters
void Layout::Group::Manager::clearLayout(bool removeComponentsFromParent)
{
    if (removeComponentsFromParent)
    {
        for (const Row& row : layout)
        {
            for (const RowItem& rowItem : row)
            {
                if (!rowItem.isEmpty())
                {
                    juce::Component * parent = rowItem.getComponent()
                            ->getParentComponent();
                    if (parent != nullptr)
                    {
                        parent->removeChildComponent(rowItem.getComponent());
                    }
                }
            }
        }
    }
    layout = RelativeLayout();
    xWeightSums.clear();
    yWeightSum = 0;
}


#if JUCE_DEBUG
// Print out the layout to the console for debugging
void Layout::Group::Manager::printLayout()
{
    using juce::String;
    for (int rowNum = 0; rowNum < layout.rowCount(); rowNum++)
    {
        const Row& row = layout.getRow(rowNum);
        DBG(String("Row weight:") + String(row.getWeight()) + String("/")
                + String(yWeightSum));
        String rowStr = "\t";
        for (int i = 0; i < row.itemCount(); i++)
        {
            const RowItem& rowItem = row.getRowItem(i);
            if (rowItem.isEmpty())
            {
                rowStr += "null";
            }
            else
            {
                rowStr += rowItem.getComponent()->getName();
            }
            rowStr += " (";
            rowStr += String(rowItem.getWeight());
            rowStr += "/";
            rowStr += String(xWeightSums[rowNum]);
            rowStr += ") ";
        }
        DBG(rowStr);
    }
    DBG("");
}
#endif


// Finds where the layout manager would place each layout item within a given
// bounding box.
Layout::Group::Manager::BoundsGrid Layout::Group::Manager::getBoundsGrid
(const juce::Rectangle<int>& layoutBounds) const
{
    int xMarginSize = layout.getXMarginFraction() * layoutBounds.getWidth();
    int yMarginSize = layout.getYMarginFraction() * layoutBounds.getHeight();
    int yPaddingSize = layout.getYPaddingFraction() * layoutBounds.getHeight();
    int yPaddingCount = 0;
    bool nonzeroRowFound = false;
    for (const Row& row : layout)
    {
        if (row.getWeight() > 0)
        {
            if (nonzeroRowFound)
            {
                yPaddingCount++;
            }
            else
            {
                nonzeroRowFound = true;
            }
        }
    }
    int weightedHeight = 0;
    if (yWeightSum > 0)
    {
        weightedHeight = layoutBounds.getHeight() - yMarginSize * 2
                - yPaddingSize * yPaddingCount;
    }
    int yStart = layoutBounds.getY() + yMarginSize;
    if (yWeightSum > 0)
    {
        yStart += layout.getYMarginWeight() * weightedHeight / yWeightSum;
    }
    if (yPaddingSize == 0 && yWeightSum > 0)
    {
        yPaddingSize = layout.getYPaddingWeight() * weightedHeight
                / yWeightSum;
    }
    int yPos = yStart;
    BoundsGrid boundsGrid;
    for (int rowNum = 0; rowNum < layout.rowCount(); rowNum++)
    {
        const Row& row = layout.getRow(rowNum);
        if (row.getWeight() > 0 && yPos != yStart)
        {
            yPos += yPaddingSize;
        }
        int height = 0;
        if (yWeightSum > 0)
        {
            height = std::max<int>(0,
                    row.getWeight() * weightedHeight / yWeightSum);
        }
        const int& xWeightSum = xWeightSums[rowNum];
        int xPaddingCount = 0;
        bool nonzeroItemFound = false;
        BoundsList boundsList;
        for (const RowItem& rowItem : row)
        {
            if (rowItem.getWeight() > 0)
            {
                if (nonzeroItemFound)
                {
                    xPaddingCount++;
                }
                else
                {
                    nonzeroItemFound = true;
                }
            }
        }
        int xPaddingSize = layout.getXPaddingFraction()
                 * layoutBounds.getWidth();
        int weightedWidth = layoutBounds.getWidth() - xMarginSize * 2
                - xPaddingSize * xPaddingCount;
        if (xPaddingSize == 0 && xWeightSum > 0)
        {
            xPaddingSize = layout.getXPaddingWeight() * weightedWidth
                    / xWeightSum;
        }
        int xStart = layoutBounds.getX() + xMarginSize;
        int xPos = xStart;
        for (const RowItem& rowItem : row)
        {
            if (rowItem.getWeight() > 0 && xPos != xStart)
            {
                xPos += xPaddingSize;
            }
            int width = 0;
            if (xWeightSum > 0)
            {
                width = std::max<int>(0,
                        rowItem.getWeight() * weightedWidth / xWeightSum);
            }
            boundsList.add(juce::Rectangle<int>(xPos, yPos, width, height));
            xPos += width;
        }
        boundsGrid.add(boundsList);
        yPos += height;
    }
    return boundsGrid;
}


// Updates the positions and sizes of all layout Components using an existing
// set of layout item bounding rectangles.
void Layout::Group::Manager::layoutComponents(
        const BoundsGrid& boundsGrid,
        const Transition::Type transition,
        const unsigned int duration,
        const bool animateUnmoved)
{
    for (int rowIndex = 0; rowIndex < layout.rowCount(); rowIndex++)
    {
        const Row& layoutRow = layout.getRow(rowIndex);
        const BoundsList& rowBounds = boundsGrid.getReference(rowIndex);
        for (int itemIndex = 0; itemIndex < layoutRow.itemCount(); itemIndex++)
        {
            const RowItem& rowItem = layoutRow.getRowItem(itemIndex);
            if (!rowItem.isEmpty())
            {
                const juce::Rectangle<int>& newBounds
                        = rowBounds.getReference(itemIndex);
                const bool boundsChanged
                        = (rowItem.getComponent()->getBounds() != newBounds);
                if (animateUnmoved || boundsChanged)
                {
                    Transition::Animator::transitionIn(rowItem.getComponent(),
                            transition,
                            newBounds,
                            duration);
                }
            }
        }
    }
}
