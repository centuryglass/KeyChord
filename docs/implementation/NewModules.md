## Module Implementation Guide
Follow these steps when adding new modules to the project:

1. Figure out where the module fits. What module category does it belong in? Would it make more sense as a submodule of an existing module?
2. Choose a short, descriptive name for the new module.
3. Create module directories in `[ProjectDir]/Source` and `[ProjectDir]/Tests`.
4. Create a makefile for the module at `[ProjectDir]/makefiles/[moduleName].mk`,using the [module makefile template](../templates/ModuleMakefile.mk) as a guide.
5. Create a documentation file for the module at `[ProjectDir]/docs/modules/[ModuleName].md`, using the [module documentation template](../templates/ModuleDoc.md) as a guide.
6. Add a brief description of the module in the [main module documentation](../Modules.md) under the appropriate category, linking to the module's guide document.
7. Add classes/enums/etc. to the module following the [new module class guide](./NewClasses.md).
