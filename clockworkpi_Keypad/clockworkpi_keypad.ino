// CLOCKWORK Keypad Arduino driver
// For more information please visit https://forum.clockworkpi.com
// HAPPY HACKING

#include "UsbKeyboard.h"
#include "USBCodes.h"
#include "PinCodes.h"

#define DEBUG false

// Represents an inactive key:
#define KEY_NULL          0xff

// Standard key bindings:
#define KEYPAD_UP     KEY_UP_ARROW
#define KEYPAD_LEFT   KEY_LEFT_ARROW
#define KEYPAD_DOWN   KEY_DOWN_ARROW
#define KEYPAD_RIGHT  KEY_RIGHT_ARROW
#define KEYPAD_Y      KEY_I
#define KEYPAD_X      KEY_U
#define KEYPAD_A      KEY_J
#define KEYPAD_B      KEY_K
#define KEYPAD_MENU   KEY_ESCAPE
#define KEYPAD_SELECT KEY_SPACE
#define KEYPAD_START  KEY_ENTER
#define KEYPAD_LIGHT1 KEY_L
#define KEYPAD_LIGHT2 KEY_O
#define KEYPAD_LIGHT3 KEY_Z
#define KEYPAD_LIGHT4 KEY_Y
#define KEYPAD_LIGHT5 KEY_H

// Alternate key bindings:
#define KEYPAD_UP_SHIFT     KEY_UP_ARROW
#define KEYPAD_LEFT_SHIFT   KEY_LEFT_ARROW
#define KEYPAD_DOWN_SHIFT   KEY_DOWN_ARROW
#define KEYPAD_RIGHT_SHIFT  KEY_RIGHT_ARROW
#define KEYPAD_Y_SHIFT      KEY_O
#define KEYPAD_X_SHIFT      KEY_Y
#define KEYPAD_A_SHIFT      KEY_H
#define KEYPAD_B_SHIFT      KEY_L
#define KEYPAD_MENU_SHIFT   KEY_BACKSPACE
#define KEYPAD_SELECT_SHIFT KEY_NUM_DASH
#define KEYPAD_START_SHIFT  KEY_NUM_PLUS
#define KEYPAD_LIGHT1_SHIFT KEY_END
#define KEYPAD_LIGHT2_SHIFT KEY_PAGE_DOWN
#define KEYPAD_LIGHT3_SHIFT KEY_F12
#define KEYPAD_LIGHT4_SHIFT KEY_PAGE_UP
#define KEYPAD_LIGHT5_SHIFT KEY_HOME

// Maps pin indices to pin values:
const int pins[KEY_NUM] =
{
    KEYPAD_PIN_UP,
    KEYPAD_PIN_LEFT,
    KEYPAD_PIN_DOWN,
    KEYPAD_PIN_RIGHT,
    KEYPAD_PIN_Y,
    KEYPAD_PIN_X,
    KEYPAD_PIN_A,
    KEYPAD_PIN_B,
    KEYPAD_PIN_START,
    KEYPAD_PIN_MENU,
    KEYPAD_PIN_SELECT,
    KEYPAD_PIN_LIGHT1,
    KEYPAD_PIN_LIGHT2,
    KEYPAD_PIN_LIGHT3,
    KEYPAD_PIN_LIGHT4,
    KEYPAD_PIN_LIGHT5
};

// Maps pin indices to standard key codes:
const int keys[KEY_NUM] = 
{
    KEYPAD_UP,
    KEYPAD_LEFT,
    KEYPAD_DOWN,
    KEYPAD_RIGHT,
    KEYPAD_Y,
    KEYPAD_X,
    KEYPAD_A,
    KEYPAD_B,
    KEYPAD_START,
    KEYPAD_MENU,
    KEYPAD_SELECT,
    KEYPAD_LIGHT1,
    KEYPAD_LIGHT2,
    KEYPAD_LIGHT3,
    KEYPAD_LIGHT4,
    KEYPAD_LIGHT5
};

// Maps pin indices to alternate key codes:
const int shiftKeys[KEY_NUM] = 
{
    KEYPAD_UP_SHIFT,
    KEYPAD_LEFT_SHIFT,
    KEYPAD_DOWN_SHIFT,
    KEYPAD_RIGHT_SHIFT,
    KEYPAD_Y_SHIFT,
    KEYPAD_X_SHIFT,
    KEYPAD_A_SHIFT,
    KEYPAD_B_SHIFT,
    KEYPAD_START_SHIFT,
    KEYPAD_MENU_SHIFT,
    KEYPAD_SELECT_SHIFT,
    KEYPAD_LIGHT1_SHIFT,
    KEYPAD_LIGHT2_SHIFT,
    KEYPAD_LIGHT3_SHIFT,
    KEYPAD_LIGHT4_SHIFT,
    KEYPAD_LIGHT5_SHIFT
};


inline void pressKey(const int keyCode)
{
    if (keyCode != KEY_NULL)
    {
        UsbKeyboard.press(keyCode);
        #if DEBUG
        Serial.print("Pressed key ");
        Serial.println(keyCode);
        #endif
    }
}

inline void releaseKey(const int keyCode)
{
    if (keyCode != KEY_NULL)
    {
        UsbKeyboard.release(keyCode);
        #if DEBUG
        Serial.print("Released key ");
        Serial.println(keyCode);
        #endif
    }
}

/**
 * @brief  Checks if a particular key is pressed.
 *
 * @param keyIndex  The key's index in the list of key pins.
 *
 * @return          Whether the key at the given index is held down.
 */
inline bool keyPressed(const int keyIndex)
{
    if (keyIndex < DIGITAL_KEY_NUM)
    {
      return (digitalRead(pins[keyIndex]) == KEY_ON);
    }
    return ((analogRead(pins[keyIndex]) / ADC_BOUNDARY) == KEY_ON);
}

/**
 * @brief  Checks if the shift key is held down.
 *
 * @return  Whether the shift key is active.
 */
inline bool shiftKeyPressed()
{
  return (analogRead(KEYPAD_PIN_SHIFT) / ADC_BOUNDARY == KEY_ON);
}

// Stores which keys were previously pressed:
int oldKeys[KEY_NUM];

void setup()
{
    // Alter the behavior of timer 0, making delay() run much more quickly:
    TIMSK0 &= !(1 << TOIE0);
    // I have no idea why they did that. Maybe it's a hack to alter the 
    // behavior of the UsbKeyboard library? I'm not going to mess with it
    // without knowing why it's there.
    for (int i = 0; i < KEY_NUM; i++)
    {
        if (i < DIGITAL_KEY_NUM)
        {
            pinMode(pins[i], INPUT);
        }
        oldKeys[i] = KEY_NULL;
    }
    Serial.begin(115200);
}

void loop()
{
    // Stores the key code created by the current checked key:
    int keyCode = KEY_NULL;

    UsbKeyboard.update();
    delay(500);

    for (int i = 0; i < KEY_NUM; i++)
    {
        if (keyPressed(i))
        {
            keyCode = shiftKeyPressed() ? shiftKeys[i] : keys[i];
            if (keyCode != oldKeys[i])
            {
                releaseKey(oldKeys[i]);
                pressKey(keyCode);
                oldKeys[i] = keyCode;
            }
        }
        else
        {
            releaseKey(oldKeys[i]);
            oldKeys[i] = KEY_NULL;
        }
    }
}
