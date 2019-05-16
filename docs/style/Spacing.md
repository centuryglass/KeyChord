# Spacing, Indentation, and Line Breaks
This document describes the preferred project style rules that govern how to use whitespace, indentation, and line breaks. These rules do not need to be strictly applied, and should just be treated as a general guide for keeping C++ code files organized, consistent, and readable. If breaking these rules results in more readable code, always choose readability over strict adherence to rules.

## Horizontal Space
- When empty or used as a call operator, do not put spaces between opening parentheses and the previous token. In any other circumstances, place one space or a line break between an opening parenthesis and the previous token.
- Don't place whitespace characters at the ends of lines.
- Always use four spaces instead of the literal tab character.
- Always place one space after a comma. Never place a space before a comma. Never start a line with a comma.
- Put either one space or a line break between the following tokens and other tokens, taking care to avoid cases where the literal token strings are part of some sort of larger token.
  * +
  * -
  * /
  * !
  * %
  * |
  * ^
  * ~
  * =
  * ==
  * +=
  * -=
  * *=
  * /=
  * %=
  * &&
  * ||
  * & (only as bitwise AND)
  * * (only as multiplication)
- Some tokens are applied to an adjacent token, either the previous token, or the next token, depending on context. Do not put whitespace between these tokens and the token they target. Do put one space or a line break between these tokens and any adjacent tokens that they do not target.
  * ->
  * ++
  * --
  * * (only as the dereference operator)
  * & (only as the reference operator)
- In most circumstances, tokens should only be separated by one space character at most, if on the same line. In order to make code more legible, sequences of similar lines can break this rule, inserting extra whitespace to align relevant tokens.

## Line Breaks
- Curly brackets should always be placed on their own lines, unless the enclosed block is small enough to fit on one line.
- All lines should be no longer than 80 characters. If a statement doesn't fit within 80 characters, it should be divided across multiple lines. Whenever possible, divide lines just before an operator, or just after a comment or open parenthesis. 
 - Place one empty line before each function declaration, and two empty lines before each function definition.
 - Insert additional empty lines dividing lines of function code with distinct purposes, groups of variable declarations, or anywhere else where doing so makes it easier to find and understand distinct sections of code.

## Indentation
- Line indentation within C++ code should always use a multiple of four space characters.
- Code within a given scope should be indented by four spaces more than the scope that contains it.
- When dividing statements across multiple lines, indent lines after the first line by eight spaces more than the first line to make divided statements more visible.
- Indent lambda functions as an inner block, not as a divided line. The curly brackets opening and closing a lambda function should be indented the same amount as the statement they are continuing.
