## Project Style Guide
To keep this project organized and legible, this style guide provides set of rules to follow when documenting and formatting project files. New project files don't need to immediately follow all of these rules, but should apply them before they are included in any major release.

### Project Modules:
Classes that have similar tasks or that work together to accomplish some larger goal should be grouped together into modules. Modules should be sorted into one of the module categories described in the module documentation page. Module source and header files should all be placed in `[project directory]/Source/[category]/[module name]`, except for test files, which should go in `[project directory]/Tests/[category]/[module name]`. Larger modules should divide their classes into submodules, each with its own directory in the main module directory. Submodules may be further divided recursively into inner submodules up to any depth, as long as doing so makes it easier to keep track of the project's structure and find specific files. See the [module implementation guide](./implementation/NewModules.md) for more information about creating project modules.

### Spacing, Indentation, and Line Breaks
To keep code consistent and legible, this project provides an extensive set of rules for how whitespace, indentation, and line breaks should be used. Although these rules are very specific about exactly how to format documentation and code, treat them as more of a guideline than a strict requirement. A functional, relatively organized class that ignores some of these rules is better than an incomplete class that follows them exactly. If breaking these rules would produce more readable code, always choose readable code over strict adherence to the rules. The project's [CodeFormat script](../project-scripts/CodeFormat.pl) will attempt to automate the process of applying these rules as much as is possible. See the [spacing style guide](./style/Spacing.md) for the complete list of these rules.

### Documentation:
See the [C++ file documentation guide](./style/Documentation.md) for information on documentation within source and header files.
