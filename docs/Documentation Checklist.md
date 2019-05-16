## Documenting Header Files
  # At the top of the file:
    - List the file name under the @file tag.
    - Describe the file's purpose in one sentence under the @brief tag.

  # Above the file's main class:
    - Describe the class in one sentence under the @brief tag.
    - Explain everything the class can be used for.
        - List the most significant functions.
        - Describe any significant conditions or limitations of those functions.
    - Expand on the class's purpose, if necessary.
        - What is its purpose within the greater project?
        - If other options exist, what advantages does it provide over those?
        - Are there notable alternate reasons to use it?
        - Are there any circumstances where it might seem useful, but definitely should not be used?
    - Explain how it is typically used.
        - Are there any alternate ways it might be useful?
        - Are there any easy mistakes that should be avoided when using it?
    - Describe if and how any classes should be extended.
        - When (if ever) should sub-classes be used?
        - What requirements do subclasses need to meet?
    - Briefly describe the implementation approach.
        - Are there any alternative strategies that should be considered?
        - Are there any parts of this module that could be split into other modules?
    - List any closely related files or resources under the @see tag

## Documenting Functions
    - What is the purpose of the function?
    - Are there any requirements that should be met before the function is called?
    - Are there any side effects that occur when the function is called?
    - List each parameter under a @param tag, and each template parameter under a @tparam tag.
        - What is the purpose of the parameter?
        - Are there any values that cannot be passed to this parameter?
        - If the parameter is a reference or pointer, will the function alter it?
    - List any return value under a @return tag.
        - What is the purpose of this value?
        - Are there circumstances where this value could be invalid?
        - Are there any return values that have special connotations?
    - List each possible exception under a @throws tag.
        - When will the function throw that exception?
