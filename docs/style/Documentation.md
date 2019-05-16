# Documenting and Commenting within C++ Files
All .cpp and .h files should have complete Javadoc-style Doxygen documentation. When uncertain, it's always better to have too much documentation than to have too little. It's easy to ignore a javadoc tag that just states the obvious, but difficult to track down problems when people have different ideas of what is obvious and documentation isn't provide.

## Documentation blocks:
- Documentation comment blocks should be formatted as follows, indenting the block so that the opening '/' character aligns with the start of the documented section.
```
 /**
  * @brief  Place two spaces after a documentation tag. When tag content uses
  *         multiple lines, keep them aligned.
  *
  *  Additional details may be placed after any tag. align them with the 
  * documentation tags, indenting the first line by one space if multiple lines
  * are needed.
  *
  * @tagExample nextExample  For tags that list a property/parameter name/
  *                          exception type /etc, place one space after the tag
  *                          and two after the property name.
  *
  * @tagExample last         Align all tag contents after the @brief tag. Keep
  *                          one line of empty space between tags.
  */
```
- Documentation blocks should always be placed in the following locations:
  * At the top of any header file.
  * Before all non-empty function declarations.
  * Before all class definitions.

- Include the following tags within documentation blocks:
  * **@file**: At the start of a header file's documentation block.
  * **@brief**: Within all documentation blocks.
  * **@param**: For each parameter in a function.
  * **@tparam**: For each template parameter in a class or function template.
  * **@throws**: For each exception a function may throw.
  * **@return**: For any function that returns a value.

When defining functions that were declared separately, place the contents of the function's @brief tag before the definition within regular line comments.

## Additional comments:
- Place at least one space after starting a comment line with '//'.
- Outside of documentation blocks, always use line comments instead of block comments. This makes it easier to temporarily disable sections of code using block comments.
