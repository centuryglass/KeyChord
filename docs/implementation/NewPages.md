# Adding UI Pages using Page::Component (WIP)
Page::Component represents a single screen within the application window. Each page holds child components used to accomplish related tasks. Pages may open and display other pages, or they may close to show the page that opened them.

## When to add Page::Component subclasses
New Page::Component class types should be added to include new content that doesn't fit well within existing pages. Use Page::Component for content that should be represented as a distinct section of the application that may be visited. Don't use Page::Component if Widgets::PopupEditor, juce::AlertWindow, or anything similar would make more sense. Don't use a Page::Component for component sets if you need to control them from within a lower level page.

## Creating a new Page::Component subclass
1. Create a new class in `[ProjectDir]/Source/GUI/PageTypes` that publicly inherits [Page::Component](../../Source/GUI/Page/Page_Component.h).
2. Initialize the page layout in its constructor.
    * Use the Page::Component::setBackButton function to select whether your page should include a back button, and which side it should be on.
    * Create a [Layout::Group::RelativeLayout](../../Source/GUI/Layout/Group/Layout_Group_RelativeLayout.h) object defining how page components should be arranged, and apply it with the Page::Component::setLayout function.
    * Call the Page::Component::addAndShowLayoutComponents function to apply the new layout.
3. Override Page::Component::pageResized to define any tasks that should be performed whenever the page size changes.

## Adding a new Page::Type 
1. Add a value to the [Page::Type enum](../../Source/GUI/Page/Page_Type.h) representing the new Page::Component subclass.
2. Within the createPage function in [Page::Factory](../../Source/GUI/Page/Page_Factory.h), add a new case statement to create an object using your page class when given your page type.

## Adding Page Navigation
To open new pages, Page::Component subclasses may use the Page::Component::pushPageToStack function to select a new page to open by type. This is usually triggered within a juce::Component listener class after receiving some sort of appropriate input.

Pages may close themselves using the Page::Component::removeFromStack function. Page::Component::setBackButton will add a button to the page that automatically handles this. Page classes may override the Page::Component::overrideBackButton function to add to the behavior of the back button or replace its default behavior completely.
