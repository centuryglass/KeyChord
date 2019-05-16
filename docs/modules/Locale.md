# Locale Module Documentation
The Locale module attempts to provide localized display text for the system's current locale. Supported locales must provide all display text values in a JSON file named after the locale, placed in the locale subdirectory of the application's assets folder.

#### [Locale](../../Source/Framework/Locale/Locale.h)
Locale provides functions for selecting an appropriate localization file.

#### [Locale\::TextUser](../../Source/Framework/Locale/Locale_TextUser.h)
TextUser objects load a set of localized display text strings from the selected locale file.

#### [Locale\::Time](../../Source/Framework/Locale/Locale_Time.h)
Time is a TextUser subclass that generates localized text representing an amount of time that has passed.
