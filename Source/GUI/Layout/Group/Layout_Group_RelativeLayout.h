#pragma once
/**
 * @file  Layout_Group_RelativeLayout.h
 *
 * @brief  Defines the relative layout of a group of juce::Component objects
 *         within a bounding rectangle.
 */
#include "JuceHeader.h"
namespace Layout { namespace Group { class RowItem; } }
namespace Layout { namespace Group { class Row; } }
namespace Layout { namespace Group { class RelativeLayout; } }
/**
 * @brief  Defines the order and relative height of juce::Component rows
 *         arranged in a vertical list.
 *
 *  Components are arranged into rows and resized to fit within any rectangular
 * area, using weight values to determine their sizes in proportion to each
 * other.
 *
 *  Each row in the layout has a vertical weight value, and each component in
 * each row has a horizontal weight value. Rows and their components are
 * arranged in order within the layout bounds, without overlapping. The fraction
 * of the layout to assign to each component is calculated based on the ratio of
 * the item's weight to the sum of weight values. Specifically,
 *
 * (row height) = (layout height) * (row vertical weight)
 *          / (sum of all vertical weights)
 *
 * (component width) = (layout width) * (component weight)
 *          / (sum of all horizontal weights in the row)
 *
 *  Layouts may include margin values, defining how much space to leave above,
 * below, and on both sides of all layout rows when arranging components within
 * a bounding box. Vertical margins may be defined using a weight value, or as a
 * fraction of the total bounding box height. Horizontal margins may only be
 * defined as a fraction of the total bounding box width.
 *
 *  Layouts may also include padding values, defining how much vertical space to
 * leave between layout rows, and how much horizontal space to leave between row
 * items. Padding values may be declared using either weights or fractions of
 * the bounding box size.
 *
 * Layout Declaration Example:
 * -----------------------
 *
 * using Row     = Layout::Relative::Row;
 * using RowItem = Layout::Relative::RowItem;
 * Layout::Relative::RelativeLayout layout(
 * {
 *    Row(1),
 *    Row(3,
 *    {
 *        RowItem(),         // empty space, weight 1
 *        RowItem(cpA, 10),  // Component* cpA, weight 10
 *        RowItem(cpB),      // Component* cpB, weight 1
 *        RowItem(5)         // empty space, weight 5
 *    }),
 *    Row(1, { RowItem(cpC) })
 * });
 * layout.setXPaddingWeight(1);
 * layout.setYPaddingWeight(2);
 * layout.setXMarginFraction(1/20);
 * layout.setYMarginWeight(2);
 *
 * -----------------------
 * Resulting layout:
 * -----------------------
 * A = Component* cpA
 * B = Component* cpB
 * C = Component* cpC
 * . = margin space
 * | = vertical padding
 * - = horizontal padding
 * _ = empty space
 * -----------------------
 *
 *   ....................
 *   ....................
 *   .__________________.
 *   .||||||||||||||||||.
 *   .||||||||||||||||||.
 *   ._AAAAAAAAAA-B_____.
 *   ._AAAAAAAAAA-B_____.
 *   ._AAAAAAAAAA-B_____.
 *   .||||||||||||||||||.
 *   .||||||||||||||||||.
 *   .CCCCCCCCCCCCCCCCCC.
 *   ....................
 *   ....................
 *
 */
class Layout::Group::RelativeLayout
{
public:
    /**
     * @brief  Creates the layout from an initial set of layout Row objects.
     *
     * @param rows  The rows of components arranged in this layout.
     */
    RelativeLayout(std::vector<Row> rows = std::vector<Row>()) :
    rows(rows) { }

    /**
     * @brief  Sets the size of the top and bottom margins of the layout using a
     *         weight value.
     *
     *  Each margin's share of the layout's vertical space will be equal to its
     * weight divided by the sum of all margin and row weights. Setting the
     * Y-margin weight resets the Y-margin fraction to zero. Y-Margins can be
     * defined by a weight or a fraction, but not both.
     *
     * @param weight  Both vertical margins will have this assigned as their
     *                weight.
     */
    void setYMarginWeights(const unsigned int weight);

    /**
     * @brief  Sets the size of the left and right margins of the layout as a
     *         fraction of the total layout width.
     *
     *  Each horizontal margin will have its width set as this fraction of the
     * total layout width.
     *
     * @param fraction  The fraction of the total width to allocate to each
     *                  margin.
     */
    void setXMarginFraction(const float fraction);

    /**
     * @brief  Sets the size of the top and bottom margins of the layout as a
     *         fraction of the total layout height.
     *
     *  Each vertical margin will have its height set as this fraction of the
     * total layout height. Setting the Y-margin fraction resets the Y-margin
     * weight to zero. Y-Margins can be defined by a weight or a fraction, but
     * not both.
     *
     * @param fraction  The fraction of the total height to allocate to each
     *                  margin.
     */
    void setYMarginFraction(const float fraction);

    /**
     * @brief  Sets the fraction of the total layout width to leave empty
     *         between all row items with non-zero weights.
     *
     *  Setting the X-padding fraction resets the X-padding weight to zero.
     * Padding can be defined by a weight or a fraction, but not both.
     *
     * @param fraction  The amount of horizontal space to leave between each
     *                  item in the row with nonzero weight, as a fraction of
     *                  the total layout width.
     */
    void setXPaddingFraction(const float fraction);

    /**
     * @brief  Sets the fraction of the total layout height to leave empty
     *         between all layout rows with non-zero weights.
     *
     *  Setting the Y-padding fraction resets the Y-padding weight to zero.
     * Padding can be defined by a weight or a fraction, but not both.
     *
     * @param fraction  The amount of vertical space to leave between each
     *                  nonzero row in the layout, as a fraction of the total
     *                  layout height.
     */
    void setYPaddingFraction(const float fraction);

    /**
     * @brief  Assigns a weight value to the amount of empty space left between
     *         each row item, ignoring row items with a zero weight value.
     *
     *  Setting the X-padding weight resets the X-padding fraction to zero.
     * Padding can be defined by a weight or a fraction, but not both.
     *
     * @param weight  The weight value to assign to each space between row
     *                items.
     */
    void setXPaddingWeight(const unsigned int weight);

    /**
     * @brief  Assigns a weight value to the amount of empty space left between
     *         each row, ignoring rows with a zero weight value.
     *
     *  Setting the Y-padding weight resets the Y-padding fraction to zero.
     * Padding can be defined by a weight or a fraction, but not both.
     *
     * @param weight  The weight value to assign to each space between rows.
     */
    void setYPaddingWeight(const unsigned int weight);

    /**
     * @brief  Gets the fraction of the width to allocate to each horizontal
     *         margin.
     *
     * @return  The size of each horizontal margin, as a fraction of the total
     *          layout width.
     */
    float getXMarginFraction() const;

    /**
     * @brief  Gets the fraction of the height to allocate to each vertical
     *         margin.
     *
     * @return  The size of each vertical margin, as a fraction of total layout
     *          height, or zero if the vertical margins are defined using a
     *          weight value.
     */
    float getYMarginFraction() const;

    /**
     * @brief  Gets the weight value assigned to each vertical margin.
     *
     * @return  The weight value allocated to each vertical margin, or zero if
     *          margins are explicitly defined as a specific fraction of the
     *          layout height.
     */
    unsigned int getYMarginWeight() const;

    /**
     * @brief  Gets the fraction of the width to allocate to each space between
     *         RowItems.
     *
     * @return  The width fraction to allocate to each padding section, or zero
     *          if horizontal padding is set with a weight value.
     */
    float getXPaddingFraction() const;

    /**
     * @brief  Gets the weight value assigned to each horizontal space between
     *         RowItems.
     *
     * @return  The weight value to allocate to each horizontal padding section.
     */
    unsigned int getXPaddingWeight() const;

    /**
     * @brief  Gets the fraction of the height to allocate to each space between
     *         layout rows.
     *
     * @return  The height fraction to allocate to each padding section, or zero
     *          if vertical padding is set with a weight value.
     */
    float getYPaddingFraction() const;

    /**
     * @brief  Gets the weight value assigned to each vertical space between
     *         layout rows.
     *
     * @return  The weight value to allocate to each padding section
     */
    unsigned int getYPaddingWeight() const;

    /**
     * @brief  Gets the number of rows in the layout.
     *
     * @return  The number of rows.
     */
    unsigned int rowCount() const;

    /**
     * @brief  Checks if the layout contains any rows.
     *
     * @return  Whether the layout contains zero rows. Note that this will
     *          return false even if all rows are empty or have a weight of
     *          zero.
     */
    bool isEmpty() const;

    /**
     * @brief  Gets a row in the layout.
     *
     * @param index               The index of the row to get.
     *
     * @return                    The row at the given index.
     *
     * @throw  std::out_of_range  If index is not a valid row index.
     */
    const Row& getRow(const unsigned int index) const;

    /**
     * @brief  Inserts a new row into the layout at a specific index.
     *
     * @param index  The index where the row will be inserted. Any rows already
     *               located at that index or greater will be pushed forward.
     *               If the index value is invalid, the closest valid index will
     *               be used.
     *
     * @param row    The new layout row to add.
     */
    void insertRow(const unsigned int index, const Row row);

    /**
     * @brief  Replaces a row in the layout.
     *
     * @param index              The index of the row that will be replaced.
     *
     * @param row                The new row to add to the layout.
     *
     * @throw std::out_of_range  If index is not a valid row index.
     */
    void setRow(const unsigned int index, const Row row);

    /**
     * @brief  Adds a new row to the end of the layout.
     *
     * @param row  The new layout row to add.
     */
    void addRow(const Row row);

    /**
     * @brief  Removes a row from the layout.
     *
     * @param rowIndex                    The index of the row to remove. If
     *                                    this is not a valid row index, no
     *                                    action is taken.
     *
     * @param removeComponentsFromParent  Whether components in the removed row
     *                                    should also be removed from their
     *                                    parent component.
     */
    void removeRow(const unsigned int rowIndex,
            const bool removeComponentsFromParent = false);

    /**
     * @brief  Checks if this layout and another are equivalent.
     *
     *  RelativeLayouts are equivalent if they have the same margin and padding
     * values, and for each row index, their rows are equivalent.
     *
     * @param rhs  Another RelativeLayout to compare to this layout.
     *
     * @return     Whether this Layout and rhs are equivalent.
     */
    bool operator== (const RelativeLayout& rhs) const;

    /**
     * @brief  Checks if this layout and another are not equivalent.
     *
     * @param rhs  Another RelativeLayout to compare to this layout.
     *
     * @return     Whether this Layout and rhs are not equivalent.
     */
    bool operator!= (const RelativeLayout& rhs) const;

    // Allow iterating through list rows using range-based for loops:
    typedef std::vector<Row>::iterator iterator;
    typedef std::vector<Row>::const_iterator const_iterator;

    /**
     * @brief  Gets an iterator pointing to the first row in the layout.
     *
     * @return  A pointer to the topmost layout row.
     */
    iterator begin();

    /**
     * @brief  Gets an iterator pointing just after the last row in the layout.
     *
     * @return  A pointer just after the last layout row.
     */
    iterator end();

    /**
     * @brief  Gets a const iterator pointing to the first row in the layout.
     *
     * @return  A constant pointer to the topmost layout row.
     */
    const_iterator begin() const;

    /**
     * @brief  Gets a const iterator pointing just after the last row in the
     *         layout.
     *
     * @return  A constant pointer just after the last layout row.
     */
    const_iterator end() const;

private:
    // Margin Sizes:
    // Horizontal margin cannot be defined by a weight value because each
    // row in the layout can have a different horizontal weight sum.
    float xMarginFraction = 0;
    unsigned int yMarginWeight = 0;
    float yMarginFraction = 0;

    // Padding Sizes:
    // Sets the amount of horizontal space between row items
    unsigned int xPaddingWeight = 0;
    float xPaddingFraction = 0;

    // Sets the amount of vertical space between rows
    unsigned int yPaddingWeight = 0;
    float yPaddingFraction = 0;

    // Holds the layouts of all component rows.
    std::vector<Row> rows;
};

/**
 * @brief  Defines the order and relative widths of juce::Component objects
 *         arranged in a horizontal list.
 */
class Layout::Group::Row
{
public:
    /**
     * @brief  Creates a new layout Row, optionally with an initial weight and
     *         list of row items.
     *
     * @param weight    Sets this row's height relative to other rows in the
     *                  layout.
     *
     * @param rowItems  The list of all RowItems arranged in this row.
     */
    Row(const unsigned int weight = 1, std::vector<RowItem> rowItems = {}) :
    weight(weight), rowItems(rowItems) { }

    /**
     * @brief  Adds a new RowItem to the end of this row.
     *
     * @param rowItem  The RowItem to add to the row.
     */
    void addRowItem(const RowItem rowItem);

    /**
     * @brief  Gets the number of RowItems in the row.
     *
     * @return  The number of components/empty spaces in the row layout.
     */
    unsigned int itemCount() const;

    /**
     * @brief  Checks if this row is empty.
     *
     * @return  Whether this row contains no row items.
     */
    bool isEmpty() const;

    /**
     * @brief  Gets a RowItem from the row.
     *
     * @param index              The index of the row item within the row.
     *
     * @return                   The row item at the given index.
     *
     * @throw std::out_of_range  If the index is invalid.
     */
    const RowItem& getRowItem(const unsigned int index) const;

    /**
     * @brief  Gets the weight assigned to this row.
     *
     * @return  The weight value used to set the row's height.
     */
    unsigned int getWeight() const;

    /**
     * @brief  Checks if this Row and another Row are equivalent.
     *
     *  Rows are equivalent if they have the same vertical weight and each
     * RowItem in this Row is equivalent to another row item at the same index
     * in the other row.
     *
     * @param rhs  The other row being compared to this one.
     *
     * @return     Whether this row and rhs are equivalent.
     */
    bool operator== (const Row& rhs) const;

    /**
     * @brief  Checks if this Row and another Row are not equivalent.
     *
     * @param rhs  The other Row being compared with this row.
     *
     * @return     Whether this Row and rhs are not equivalent.
     */
    bool operator != (const Row& rhs) const;

    // Allow iterating through row items using range-based for loops:
    typedef std::vector<RowItem>::iterator iterator;
    typedef std::vector<RowItem>::const_iterator const_iterator;

    /**
     * @brief  Gets an iterator pointing to the first item in the row.
     *
     * @return  A pointer to the leftmost row item.
     */
    iterator begin();
    /**
     * @brief  Gets an iterator pointing just after the last item in the row.
     *
     * @return  A pointer just after the last row item.
     */
    iterator end();
    /**
     * @brief  Gets a const iterator pointing to the first item in the row.
     *
     * @return  A constant pointer to the leftmost row item.
     */
    const_iterator begin() const;
    /**
     * @brief  Gets a const iterator pointing just after the last item in the
     *         row.
     *
     * @return  A constant pointer just after the last row item
     */
    const_iterator end() const;
private:
    // Row height = total height * weight / yWeightSum
    unsigned int weight;
    // Holds all ComponentLayouts in the row:
    std::vector<RowItem> rowItems;
};
/**
 * @brief  Defines the juce::Component and horizontal weight value of one item
 *         in a Row layout object.
 */
class Layout::Group::RowItem
{
public:
    /**
     * @brief  Creates a RowItem, initializing the item's Component and weight.
     *
     * @param component  A component to place in the layout, or nullptr to add
     *                   an empty space.
     *
     * @param weight     Sets this row item's width relative to the other
     *                   RowItems in the row.
     */
    RowItem(juce::Component* component, const unsigned int weight = 1) :
    component(component), weight(weight) { }
    /**
     * @brief  Creates a RowItem with no Component and a specific weight.
     *
     * @param weight  Sets this row item's width relative to the other RowItems
     *                in the row.
     */
    RowItem(const unsigned int weight = 1) : weight(weight) { }
    /**
     * @brief  Gets the component assigned to this RowItem.
     *
     * @return  The Component pointer assigned to this RowItem on creation.
     *          This value may be null.
     */
    juce::Component* getComponent() const;

    /**
     * @brief  Gets the horizontal weight value assigned to this RowItem.
     *
     * @return  The item's weight value.
     */
    unsigned int getWeight() const;

    /**
     * @brief  Checks if this row item represents an empty space.
     *
     * @return   Whether this row item has no component.
     */
    bool isEmpty() const;

    /**
     * @brief  Checks if this RowItem and another are equivalent. RowItems are
     *         equivalent if they have the same weight and Component.
     *
     * @param rhs  The row item being compared to this one.
     *
     * @return     Whether this row item and rhs are equivalent.
     */
    bool operator== (const RowItem& rhs) const;

    /**
     * @brief  Checks if this RowItem and another are not equivalent.
     *
     * @param rhs  Another RowItem being compared to this one.
     *
     * @return     Whether this row item and rhs are not equivalent.
     */
    bool operator!= (const RowItem& rhs) const;

private:
    // Points to a component in the layout, or nullptr to represent an empty
    // space in the layout.
    juce::Component* component = nullptr;

    // Component width = total width * weight / xWeightSum[rowNumber]
    unsigned int weight;
};
