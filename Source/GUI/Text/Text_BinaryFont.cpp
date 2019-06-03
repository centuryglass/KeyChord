#include "Text_BinaryFont.h"

// Encodes all font data. This array is generated from a 160x160 black and white
// image file using project-scripts/ImageBin.pl.
const juce::uint32 fontMap [800] = 
{
    0b00000000001111111111111111111100, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000001000000001111111111100, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000001000000001111111111100, 0b00000000000000000000000000000000, 0b10000000011100000000100000000010, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000001000000001111111111100, 0b00100000001001000000001000000001, 0b11000000011100000001100000000011, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000001000000001111111111100, 0b01100000001001100000001101000011, 0b11100000011100000011111100011111, 0b10000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000001000000001111111111100, 0b11111100001111110001111111000111, 0b11110001111111000111111100011111, 0b11000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000001000000001111111111100, 0b01100000000001100000001101000001, 0b11000000111110000011111100011111, 0b10000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000001000000001111111111100, 0b00100000000001000000001000000001, 0b11000000011100000001100000000011, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000001000000001111111111100, 0b00000000000000000000000000000001, 0b11000000001000000000100000000010, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000001111111111111111111100, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000011111111, 0b11111111111111111111111111111111,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000011111111, 0b11111111111111000000000000000011,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000011111111, 0b11111111111110000000000000000001,
    0b00000000000000000000000000000000, 0b00000000001100000000000000000000, 0b00000000000000000000111001110011, 0b00000000111001110100000011111111, 0b11111111111110000000000000000001,
    0b00001101110110010000000000100100, 0b11100000010001010111011011100000, 0b11010001011000000000100010000100, 0b00000000100101000100000011111111, 0b11111111111110000000000000000001,
    0b00010000100101010000000001010100, 0b01000000001101110010010001000001, 0b00011011010100000000110001100100, 0b00000000100101100100000011111111, 0b11111111111110000000000000000001,
    0b00010000100110010000000001110100, 0b01000000000101010010011001000001, 0b00010101010100000000100000010100, 0b00000000100101000100000011111111, 0b11111111111110000000000000000001,
    0b00001100100101011000000001010110, 0b01000000011101010111010001000000, 0b11010001011000000000111011100011, 0b00000000111001110111000011111111, 0b11111111111110000000000000000001,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000011111111, 0b11111111111111000000000000000011,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000011111111, 0b11111111111111111111111111111111,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000010000000000000000000, 0b00000000000000000000010000000010, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000100000001101100000, 0b01010000000111100000010001000000, 0b11000000110000000000100000000001, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000100000001001000000, 0b11111000001010000000110010000001, 0b00100000100000000001000000000000, 0b10000001010000000010000000000000, 0b00000000000000000000000000001000,
    0b00000000000000100000000000000000, 0b01010000000111000000000100000000, 0b10000000000000000001000000000000, 0b10000000100000000111000000000000, 0b00001111100000000000000000010000,
    0b00000000000000100000000000000000, 0b01010000000010100000001000000001, 0b01010000000000000001000000000000, 0b10000001010000000010000000000000, 0b00000000000000000000000000100000,
    0b00000000000000000000000000000000, 0b11111000001010100000010001000001, 0b00100000000000000001000000000000, 0b10000000000000000000000000001000, 0b00000000000000000000000001000000,
    0b00000000000000100000000000000000, 0b01010000000111000000100011000000, 0b11100000000000000000100000000001, 0b00000000000000000000000000011000, 0b00000000000000001000000010000000,
    0b00000000000000000000000000000000, 0b00000000000010000000000000000000, 0b00000000000000000000010000000010, 0b00000000000000000000000000010000, 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00011110000000010000000011000000, 0b01110000001001000000111100000001, 0b11000000011110000000110000000110, 0b00000000100000000010000000000000, 0b00000000000000000000000001111000,
    0b00100011000000110000000100100000, 0b10001000001001000000100000000010, 0b00100000000010000001001000001001, 0b00000000100000000010000000000100, 0b00000000000000010000000010001000,
    0b00100101000000010000000100100000, 0b00110000001001000000111100000010, 0b00000000000110000000110000001001, 0b00000000000000000000000000001000, 0b00000111100000001000000000001000,
    0b00101001000000010000000001100000, 0b00001000001111100000000110000011, 0b11100000000100000001001000000111, 0b00000000100000000010000000010000, 0b00000000000000000100000000110000,
    0b00110001000000010000000011000000, 0b10001000000001000000100110000010, 0b00100000001100000001001000000001, 0b00000000100000000010000000001000, 0b00000111100000001000000000000000,
    0b00011110000000111000000111100000, 0b01110000000001000000011100000001, 0b11000000001000000000110000000001, 0b00000000000000000100000000000100, 0b00000000000000010000000000100000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00011110000000110000000111100000, 0b01110000000111100000011110000001, 0b11100000011110000001001000000111, 0b10000001111000000100100000010000, 0b00001101100000100010000001111000,
    0b00100001000001001000000100010000, 0b10001000000100010000010000000001, 0b00000000100010000001001000000010, 0b00000000010000000101000000010000, 0b00001010100000110010000010000100,
    0b00101101000001001000000111100000, 0b10000000000100010000011100000001, 0b11000000100000000001001000000010, 0b00000000010000000110000000010000, 0b00001010100000101010000010000100,
    0b00101111000001111000000100010000, 0b10000000000100010000010000000001, 0b00000000100110000001111000000010, 0b00000000010000000101000000010000, 0b00001010100000100110000010000100,
    0b00100000000001001000000100010000, 0b10001000000100010000010000000001, 0b00000000100010000001001000000010, 0b00000010010000000100100000010000, 0b00001010100000100010000010000100,
    0b00011110000001001000000111100000, 0b01110000000111100000011110000001, 0b00000000011110000001001000000111, 0b10000001110000000100100000011110, 0b00001010100000100010000001111000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00111100000001111000000111000000, 0b01111000001111100000100010000010, 0b00010000100001000010001000001000, 0b10000011111000000011000000000000, 0b00000011000000001000000000000000,
    0b00100010000010000100000100100000, 0b10000000000010000000100010000010, 0b00010000100101000001010000000101, 0b00000000010000000010000000100000, 0b00000001000000010100000000000000,
    0b00100010000010000100000100100000, 0b01111000000010000000100010000010, 0b00010000100101000000100000000010, 0b00000000100000000010000000010000, 0b00000001000000100010000000000000,
    0b00111100000010010100000111000000, 0b00000100000010000000100010000001, 0b00100000100101000000100000000010, 0b00000001000000000010000000001000, 0b00000001000000000000000000000000,
    0b00100000000010001000000100100000, 0b10000100000010000000100010000001, 0b00100000100101000001010000000010, 0b00000010000000000010000000000100, 0b00000001000000000000000000000000,
    0b00100000000001110100000100100000, 0b01111000000010000000011100000000, 0b11000000011010000010001000000010, 0b00000011111000000011000000000010, 0b00000011000000000000000001111000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000000000000100000000, 0b00000000000000100000000000000000, 0b11000000000000000001000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00001000000000000000000100000000, 0b00000000000000100000000000000000, 0b10100000000000000001000000000010, 0b00000000010000000100000000001000, 0b00000000000000000000000000000000,
    0b00001100000001111000000111100000, 0b01110000000111100000011100000000, 0b10000000001110000001000000000000, 0b00000000000000000101000000001000, 0b00000101000000011100000000111000,
    0b00000100000010001000000100010000, 0b10001000001000100000100010000000, 0b10000000010010000001110000000010, 0b00000000010000000101000000001000, 0b00001111100000010110000001000100,
    0b00000000000010001000000100010000, 0b10000000001000100000111110000001, 0b11000000010010000001001000000010, 0b00000000010000000110000000001000, 0b00001010100000010010000001000100,
    0b00000000000010011000000100010000, 0b10001000001000100000100000000000, 0b10000000001110000001001000000010, 0b00000000010000000101000000001000, 0b00001010100000010010000001000100,
    0b00000000000001101000000111100000, 0b01110000000111100000011110000000, 0b10000000000010000001001000000010, 0b00000000010000000101000000001000, 0b00001010100000010010000000111000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000001100000000000000000000, 0b00000001110000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000001000000001000, 0b00000010000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000010000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000011000000001000, 0b00000011000000000000000000000000,
    0b00011100000001110000000111100000, 0b00111000000111000000010010000010, 0b00100000100010000010001000000100, 0b10000001111000000010000000001000, 0b00000001000000000000000000000000,
    0b00010010000010010000000100010000, 0b01000000000010000000010010000010, 0b00100000100010000001010000000100, 0b10000000001000000110000000001000, 0b00000001100000011001000000000000,
    0b00010010000010010000000100000000, 0b00110000000010000000010010000010, 0b00100000101010000000100000000011, 0b10000000010000000110000000001000, 0b00000001100000100110000000000000,
    0b00011100000001110000000100000000, 0b00001000000010000000010010000001, 0b01000000101010000001010000000000, 0b10000000100000000010000000001000, 0b00000001000000000000000000000000,
    0b00010000000000010000000100000000, 0b01110000000010000000001110000000, 0b10000000010100000010001000000000, 0b10000001111000000011000000001000, 0b00000011000000000000000000000000,
    0b00010000000000011000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000011, 0b00000000000000000001000000001000, 0b00000010000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00110000000011000000001100000000, 0b11000000001100000000110000000011, 0b00000000110000000011000000011000, 0b00000110000000011000000000000000, 0b00000000000000000000000000000000,
    0b00100110000010011000001001100000, 0b10010100001001110000100010000010, 0b01110000100111000010011100010010, 0b11100100101000010010110000000000, 0b00000000000000000000000000000000,
    0b00110010000011001000001100100000, 0b11011100001101000000110100000011, 0b00010000110101000011010100011010, 0b10100110101000011010010000000000, 0b00000000000000000000000000000000,
    0b00100010000010001000001001100000, 0b10000100001000100000100110000010, 0b00110000100111000010011100010010, 0b10100100101000010010010000000000, 0b00000000000000000000000000000000,
    0b00100010000010010000001000100000, 0b10000100001000010000100101000010, 0b00100000100101000010000100010010, 0b10100100101000010010100000000000, 0b00000000000000000000000000000000,
    0b00100111000010011000001001100000, 0b10000100001001100000100010000010, 0b00100000100111000010011100010010, 0b11100100101000010010110000000000, 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b10000000000100000000000000000000, 0b00000000101000000000000000000000, 0b00000000000000000000000001111000,
    0b00000000000000100000000010000000, 0b00110000000000000000100010000000, 0b10000000001000000001010000000111, 0b10000001011000000000000000111111, 0b00000000000000011110000000000000,
    0b00000000000000000000000111000000, 0b01001000001000100000110110000000, 0b10000000000100000000000000001000, 0b01000000101000000010100000000001, 0b00000000000000100001000000000000,
    0b00000000000000100000001010000000, 0b01000000000111000000011100000000, 0b10000000001010000000000000001011, 0b01000000000000000101000000000000, 0b00001111110000101101000000000000,
    0b00000000000000100000001010000000, 0b11100000000101000000111110000000, 0b00000000010100000000000000001010, 0b01000000000000001010000000000000, 0b00000000000000101001000000000000,
    0b00000000000000100000000111000000, 0b01000000000111000000001000000000, 0b10000000001000000000000000001011, 0b01000000000000000101000000000000, 0b00000000000000101001000000000000,
    0b00000000000000100000000010000000, 0b10111000001000100000111110000000, 0b10000000000100000000000000001000, 0b01000000000000000010100000000000, 0b00000000000000100001000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000001000000000, 0b10000000001000000000000000000111, 0b10000000000000000000000000000000, 0b00000000000000011110000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000000000000111000000, 0b00110000000000100000000000000000, 0b00000000000000000000000000000110, 0b00000000100000000000000000110000, 0b00001100000000110000000000000000,
    0b00011100000000100000000001000000, 0b00010000000001000000000000000001, 0b11000000000000000000000000000010, 0b00000001010000000000000000010001, 0b00000100010000010001000000100000,
    0b00010100000011111000000110000000, 0b00110000000000000000100100000011, 0b01000000000000000000000000000111, 0b00000000100000001010000000111010, 0b00001110100000110010000000000000,
    0b00011100000000100000000111000000, 0b00010000000000000000100100000011, 0b01000000001100000000000000000000, 0b00000000000000000101000000000100, 0b00000001000000010100000001100000,
    0b00000000000000000000000000000000, 0b00110000000000000000100100000001, 0b01000000001100000000000000000000, 0b00000000000000000010100000001000, 0b00000010110000111000000010000000,
    0b00000000000011111000000000000000, 0b00000000000000000000111010000001, 0b01000000000000000000000000000000, 0b00000000000000000101000000010101, 0b00000100010000010101000010001000,
    0b00000000000000000000000000000000, 0b00000000000000000000100000000001, 0b01000000000000000000010000000000, 0b00000000000000001010000000100111, 0b00001000100000100111000011110000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000110000000000, 0b00000000000000000000000000000001, 0b00000000110000000001000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00100000000000001000000010000000, 0b00101000000000000000001100000000, 0b00000000011110000000100000000001, 0b00000000010000000101000000010000, 0b00000001000000001000000001010000,
    0b00010000000000010000000101000000, 0b01010000000100100000001100000000, 0b01110000110011000000010000000010, 0b00000000101000000000000000001000, 0b00000010000000010100000000000000,
    0b00001100000000110000000000000000, 0b00000000000000000000000000000000, 0b11000000100000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00010010000001001000000011000000, 0b00110000000011000000001100000001, 0b01000000100000000001111000000111, 0b10000001111000000111100000011100, 0b00000111000000011100000001110000,
    0b00010010000001001000000100100000, 0b01001000000100100000010010000011, 0b11110000110011000001000000000100, 0b00000001000000000100000000001000, 0b00000010000000001000000000100000,
    0b00011110000001111000000111100000, 0b01111000000111100000011110000010, 0b01000000011110000001110000000111, 0b00000001110000000111000000001000, 0b00000010000000001000000000100000,
    0b00010010000001001000000100100000, 0b01001000000100100000010010000110, 0b01110000000100000001000000000100, 0b00000001000000000100000000001000, 0b00000010000000001000000000100000,
    0b00010010000001001000000100100000, 0b01001000000100100000010010000000, 0b00000000001100000001111000000111, 0b10000001111000000111100000011100, 0b00000111000000011100000001110000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000010100000010000000, 0b00001000000010000000011100000000, 0b00000000000000000000000000000100, 0b00000000010000000010000000010100, 0b00000001000000000000000000000000,
    0b00011110000000101000000001000000, 0b00010000000101000000000000000001, 0b00100000000000000000110100000010, 0b00000000100000000101000000000000, 0b00000010000000010000000000110000,
    0b00010011000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000010100000001001000001000, 0b10000010001000000000000000100010, 0b00001000100000011100000001001000,
    0b00010001000001001000000111100000, 0b01111000000111100000011110000001, 0b11100000001000000001011000001000, 0b10000010001000001000100000100010, 0b00001101100000010010000001010000,
    0b00111001000001101000001100110000, 0b11001100001100110000110011000011, 0b00110000010100000001101000001000, 0b10000010001000001000100000100010, 0b00000111000000010010000001011000,
    0b00010011000001111000001000010000, 0b10000100001000010000100001000010, 0b00010000000000000001001000001000, 0b10000010001000001000100000100010, 0b00000010000000011100000001001000,
    0b00011110000001011000001100110000, 0b11001100001100110000110011000011, 0b00110000000000000010110000001000, 0b10000010001000001000100000100010, 0b00000010000000010000000001111000,
    0b00000000000001001000000111100000, 0b01111000000111100000011110000001, 0b11100000000000000000000000000111, 0b00000001110000000111000000011100, 0b00000010000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00010000000000010000000010000000, 0b00101000000000000000001100000000, 0b00000000000000000001000000000001, 0b00000000100000000000000000000000, 0b00000000000000000000000000000000,
    0b00001000000000100000000101000000, 0b01010000000101000000001100000000, 0b00000000011100000000100000000010, 0b00000001010000000101000000010000, 0b00000001000000001000000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000001, 0b00000000100010000000000000000000, 0b00000000000000000000000000001000, 0b00000010000000010100000001010000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b11100000100000000001100000000110, 0b00000001100000000110000000000000, 0b00000000000000000000000000000000,
    0b00011100000001110000000111000000, 0b01110000000111000000011100000011, 0b10010000100010000010010000001001, 0b00000010010000001001000000001000, 0b00000010000000001000000000100000,
    0b00100100000010010000001001000000, 0b10010000001001000000100100000100, 0b11110000011100000011110000001111, 0b00000011110000001111000000001000, 0b00000010000000001000000000100000,
    0b00100100000010010000001001000000, 0b10010000001001000000100100000100, 0b10000000001000000010000000001000, 0b00000010000000001000000000001000, 0b00000010000000001000000000100000,
    0b00011010000001101000000110100000, 0b01101000000110100000011010000011, 0b01110000011000000001110000000111, 0b00000001110000000111000000001000, 0b00000010000000001000000000100000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000,
    0b00010100000000101000000010000000, 0b00001000000010000000001010000001, 0b00100000000000000000000000000100, 0b00000000010000000010000000000000, 0b00000001000000000000000001010000,
    0b00001000000001010000000001000000, 0b00010000000101000000010100000000, 0b00000000000000000000000000000010, 0b00000000100000000101000000010100, 0b00000010000000010000000000000000,
    0b00010100000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000001000000000110100000000, 0b00000000000000000000000000000000, 0b00000100100000010100000001001000,
    0b00001100000001110000000011000000, 0b00110000000011000000001100000000, 0b11000000000000000001001000000100, 0b10000001001000000100100000010010, 0b00000100100000011010000001001000,
    0b00010010000001011000000100100000, 0b01001000000100100000010010000001, 0b00100000111110000001011000000100, 0b10000001001000000100100000010010, 0b00000011100000011010000000111000,
    0b00010010000001001000000100100000, 0b01001000000100100000010010000001, 0b00100000000000000001101000000100, 0b10000001001000000100100000010010, 0b00000000100000010100000000001000,
    0b00010010000001001000000100100000, 0b01001000000100100000010010000001, 0b00100000001000000001110000000100, 0b10000001001000000100100000010010, 0b00000000100000010000000000001000,
    0b00001100000001001000000011000000, 0b00110000000011000000001100000000, 0b11000000000000000010000000000011, 0b10000000111000000011100000001110, 0b00000011000000000000000000110000,
    0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000, 0b00000000000000000000000000000000
};


// Image row width, in number of characters:
static const constexpr int charRowWidth = 16;
// Character width and height, in number of bits/pixels:
static const constexpr int charSize = 10;
// Array value size in bits:
static const constexpr int arrayValSize = 32;
// Largest valid character value:
static const constexpr int maxChar = 255;

template <typename RowType> 
static RowType getVariableRow(const unsigned int character, const int row,
        const int rowSize)
{
    using juce::uint8;
    using juce::uint16;
    using juce::uint32;
    using juce::uint64;
    if (character > maxChar)
    {
        return 0;
    }
    const uint8 charIndex = (uint8) character;
    const uint8 charColumnIndex = charIndex % charRowWidth;
    const uint8 charRowIndex    = charIndex / charRowWidth;

    const uint16 bitX = charColumnIndex * charSize;
    const uint16 bitY = charRowIndex * charSize + row;
    const uint32 bitIndex = bitX + (bitY * charRowWidth * charSize);

    const uint32 arrayIndex = bitIndex / (uint32) arrayValSize;
    const int indexBitOffset = bitIndex % arrayValSize;
    const int overflow = (indexBitOffset + rowSize) - arrayValSize;
    
    /*
    DBG("Character value: " << (unsigned char ) character 
            << ", arrayIndex: " 
            << (int) arrayIndex << " bitX:" << (int) bitX << ", bitY:" 
            << (int) bitY << " bitIndex:" << (int) bitIndex);
    */
   

    if (overflow > 0)
    {
        uint32 overflowMask = 0;
        for (int i = 0; i < overflow; i++)
        {
            overflowMask |= (1 << i);
        }
        RowType rowValue = (fontMap[arrayIndex] << overflow);
        rowValue += (fontMap[arrayIndex + 1] >> (arrayValSize - overflow));
        return rowValue;
    }
    const int rightShift = arrayValSize - (indexBitOffset + rowSize);
    return fontMap[arrayIndex] >> rightShift;
}

// Gets one row of image data for a specific character.
juce::uint16 Text::BinaryFont::getCharacterRow(const unsigned int character,
        const int row)
{
    return getVariableRow<juce::uint16>(character, row, charSize);
}

// Gets one row of image data for a double-wide character.
juce::uint32 Text::BinaryFont::getDoubleCharRow
(const unsigned int character, const int row)
{
    return getVariableRow<juce::uint32>(character, row, charSize * 2);
}
