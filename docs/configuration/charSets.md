# Custom Character Set Configuration
The [charSets.json](../../assets/configuration/charSets.json) file is used to store custom keyboard character sets. KeyChord has three configurable character sets, called "letter", "number", and "special" by default. This configuration file defines the names of each of these sets, which characters they contain, the order of the contained characters, their shifted values, and the order that chord keys are assigned to them. 

## Character Set Name Keys
These values set what the character sets will be called on the KeyChord help screen.

 Name Key                      | Default Value
------------------------------ | ---
"main character set name"      | "letter"
"alternate character set name" | "number"
"special character set name"   | "special"

## Character Sets
The actual character sets are stored as JSON arrays named `"main character set"`, `"alternate character set"`, and `"special character set"`. These arrays contain JSON objects for each character in the set, listed in display order. Each character definition contains up to four property values, stored under the `"character"`, `"shifted"`, `"chord"`, and `"chord priority"` keys.

### "character" Key
Key objects must define a main character value, or they will be ignored. Character values may be defined in three different ways:
- As a single character string: Any printable character value in the standard ASCII range can be defined with a string containing only that character. This excludes all whitespace characters other than space.
- As a special character name: Most of the character values defined in `specialCharMap` at the top of [Text\::Values](../../Source/GUI/Text/Text_Values.cpp) may be used as a valid character name. `"outline"` and `"fill"` are the only exceptions, as these values are only provided for drawing boxes, and aren't mapped to actual keyboard characters.
- As a numeric character code:  Printable character within the [ISO 8859](https://en.wikipedia.org/wiki/ISO/IEC_8859-1) character set may be selected using their numeric character code, either using decimal values or hexadecimal number strings. This method is required when using characters outside of the standard ASCII range, as JSON files don't allow values outside of the ASCII range. Character codes before the start of the ASCII printable character range (32/0x20) are only supported if they have a special value assigned in Text\::Values.

### "shifted" Key
Character sets switch between their standard and shifted character values when the "Toggle shifted characters" key is pressed. Shifted characters definitions follow the same rules as the "character" key defined above. If no shifted value is provided, the main character value is reused in the shifted character set.

### "chord" Key
Specific chords may be explicitly assigned to character values. Chords should be provided using a five character string, where '#' represents a held chord key, and '_' represents an unheld chord key. For example, `"###__"` is the chord created by holding and releasing the first three chord keys, `"____#"` is the chord created by holding and releasing only the last chord key, and so on. Characters that aren't given a chord value, that have a chord value that's already in use, or have an invalid chord value will be assigned a chord by priority.

### "chord priority" Key
Characters may define a priority number, used to determine the order that chords will be assigned to them. In the chordConvenienceOrder array in [Text\::CharSet\::Cache](../../Source/GUI/Text/CharSet/Text_CharSet_Cache.cpp), all possible chord values are sorted by how hard they are to type. Characters without an explicit chord assignment will be assigned the first unused chord in this list. Characters are assigned chords using their priority value, so that characters with larger priority values receive simpler chords. Characters that share priority values are assigned chords in the order they appear in the character set. If no priority value is assigned, the default character priority is 0. 

