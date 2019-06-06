/**
 * @file  PinCodes.h
 *
 * @brief  Defines KeyPad button pin codes
 */

// Value read when a button is released:
#define KEY_OFF           1
// Value read when a button is pressed:
#define KEY_ON            0
// Threshold to register an analog button press:
#define ADC_BOUNDARY      500
// Number of digital key pins:
#define DIGITAL_KEY_NUM   9
// Number of analog key pins:
#define ANALOG_KEY_NUM    7
// Total pin count:
#define KEY_NUM           (DIGITAL_KEY_NUM + ANALOG_KEY_NUM)

// ### All keypad pins: ###
// Digital Pins:
#define KEYPAD_PIN_UP      3
#define KEYPAD_PIN_LEFT    6
#define KEYPAD_PIN_DOWN    7
#define KEYPAD_PIN_RIGHT   8
#define KEYPAD_PIN_Y       9
#define KEYPAD_PIN_X      10
#define KEYPAD_PIN_A      11
#define KEYPAD_PIN_B      12
#define KEYPAD_PIN_START  13
// Analog Pins:
#define KEYPAD_PIN_MENU    0
#define KEYPAD_PIN_SELECT  2
#define KEYPAD_PIN_LIGHT1  3
#define KEYPAD_PIN_LIGHT2  4
#define KEYPAD_PIN_LIGHT3  5
#define KEYPAD_PIN_LIGHT4  6
#define KEYPAD_PIN_LIGHT5  7
// Shift Modifier Pin (Analog):
#define KEYPAD_PIN_SHIFT   1

// ### Pin indices: ###
// The main loop checks pins in this order.
enum
{
    INDEX_PIN_UP = 0,
    INDEX_PIN_LEFT,
    INDEX_PIN_DOWN,
    INDEX_PIN_RIGHT,
    INDEX_PIN_Y,
    INDEX_PIN_X,
    INDEX_PIN_A,
    INDEX_PIN_B,
    INDEX_PIN_START,
    INDEX_PIN_MENU,
    INDEX_PIN_SELECT,
    INDEX_PIN_LIGHT1,
    INDEX_PIN_LIGHT2,
    INDEX_PIN_LIGHT3,
    INDEX_PIN_LIGHT4,
    INDEX_PIN_LIGHT5
};
