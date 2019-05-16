# Using Localized Text
All classes that display text to the user should load that text using the Locale module. When displayed text is solely managed by this module, alternate translations can be added simply by adding a single translation file to the application's locale directory.

## Adding Localizations
1. Create a copy of `[ProjectDir]/assets/locale/en_US.json` within the same directory. The copy should be named after your language's short locale code.
2. Replace all values in the file with translations in your language. Do not change any object keys. 
3. Make sure your translations are correct within the context where they will be used, and not just literal translations of the exact words used. Do not include automatically generated translations unless someone fluent in the language has verified that they are correct.

## Loading Localized Text with Locale::TextUser
1. Each class that accesses localized text should either inherit from Locale::TextUser, or have an internal object member that inherits from Locale::TextUser.
2. Each class using localized text should include a static, constant juce::Identifier object, set to the full name of the class, including namespace. Use this object for the TextUser constructor's 'className' parameter.
3. Near the top of the class source file, declare an additional static, constant juce::Identifier for each distinct piece of localized text. These should be given brief, descriptive names, and the value they store should match their names. If more than a couple of these key objects are needed, declare them in the TextKey namespace to keep the file organized.
4. In each locale file in `[ProjectDir]/assets/locale`, add a new object value mapped to the same class name used in step two. Within this object, add all localized text strings needed by your class, mapped to the same text keys you defined in step three. When adding text to locale files for languages you do not know, use the english text as a placeholder, preceded by 'TODO:' to make it easier to find and replace missing localization text.
5. Within your class, use the `localeText(const juce::Identifier& textKey)` method to load text from the user's locale.

### Code Example
Here's how this would be used in the hypothetical Example class in the Widgets namespace:

#### Widgets_Example.h:

---
```C++
#pragma once
#include "JuceHeader.h"
#include "Locale_TextUser.h"

namespace Widgets { class Example; }

class Widgets::Example : public juce::Component, public Locale::TextUser
{
public:
    Example();

    virtual ~Example() { }

private:
    void paint(juce::Graphics& g) override;
};
```
---

#### Widgets_Example.cpp:

---
```C++
#include "Widgets_Example.h"

// Localized object class key
static const juce::Identifier localeClassKey("Widgets::Example");

// Localized text value keys:
namespace TextKey
{
    static const juce::Identifier title       = "title";
    static const juce::Identifier description = "description";
}

static const constexpr int xStart = 0;
static const constexpr int titleY = 0;
static const constexpr int descriptionY = 20;

Widgets::Example::Example : Locale::TextUser(localeClassKey) { }

void Widgets::Example::paint(juce::Graphics& g)
{
    g.drawSingleLineText(localeText(TextKey::title), xStart, titleY);
    g.drawSingleLineText(localeText(TextKey::description), xStart,
            descriptionY);
}
```
---

#### en_US.json:

---
After all other localized class JSON objects:
```JSON
    "Widgets::Example" :
    {
        "title"       : "Example Widget",
        "description" : "A minimal component used as a code example.
    }
}
```
---
